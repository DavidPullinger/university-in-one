from math import pi
import numpy as np
import cv2 as cv
from geometry import angleBetween, distanceBetween, rotatePointAroundOrigin


class HandAligner:
    def __init__(
        self, image: cv.Mat, hand_centroid, hand_contour, finger_tips: int, defects
    ) -> None:
        self.image = image
        self.hand_centroid = hand_centroid
        self.hand_contour = hand_contour
        self.finger_tips = finger_tips
        self.defects = defects

        # get translation amount
        hand_x = int(hand_centroid[0])
        hand_y = int(hand_centroid[1])
        self.center_x = int(image.shape[1] / 2)
        self.center_y = int(image.shape[0] / 2)
        self.diff_x = self.center_x - hand_x
        self.diff_y = self.center_y - hand_y

    def _get_middle_finger(self):
        # sort finger tips by horizontal distance to center of image coordinate
        self.finger_tips = np.asarray(sorted(self.finger_tips, key=lambda x: x[1]))

        # finger closest to horizontal image center should be middle finger
        return self.finger_tips[0]

    def _get_middle_defects(self, middle_finger):
        # find the defects that correspond to the middle finger meeting the palm
        largeDefects = []

        # loop through defects,
        # calculating the distance between the middle finger and the defect
        for i in range(self.defects.shape[0]):
            s, e, f, d = self.defects[i, 0]
            start = self.hand_contour[s][0]
            end = self.hand_contour[e][0]
            far = self.hand_contour[f][0]
            # if the defect is large
            if d > 10000:
                largeDefects.append((distanceBetween(start, middle_finger), far))
                largeDefects.append((distanceBetween(end, middle_finger), far))
        # find two defects closest to the middle finger
        # we use first element of tuple (distance to middle finger) when sorting
        return np.asarray([i[1] for i in sorted(largeDefects, key=lambda x: x[0])[0:2]])

    def _translate_hand(self):
        # get translation matrix
        M = np.float32([[1, 0, self.diff_x], [0, 1, self.diff_y]])

        # apply translation to image
        self.image = cv.warpAffine(self.image, M, self.image.shape[1::-1])

    def _translate_feature(self, feature):
        return feature + [self.diff_x, self.diff_y]

    def _translate_features(self, middle_finger, middle_defects):
        # translate fingertips
        self.finger_tips = self._translate_feature(self.finger_tips)
        # translate hand contour
        self.hand_contour = self._translate_feature(self.hand_contour)
        # translate middle finger
        middle_finger = self._translate_feature(middle_finger)
        # translate middle defects
        middle_defects = self._translate_feature(middle_defects)

        return middle_finger, middle_defects

    def _rotate_hand(self, angle_radians):
        # rotate image using angle
        angle = angle_radians * 180 / pi
        M = cv.getRotationMatrix2D((self.center_x, self.center_y), -angle, 1)
        self.image = cv.warpAffine(self.image, M, self.image.shape[1::-1])

    def _rotate_feature(self, angle, feature):
        return [
            rotatePointAroundOrigin([self.center_x, self.center_y], f, angle)
            for f in feature
        ]

    def _rotate_features(self, angle, middle_finger, middle_defects):
        # rotate fingertips
        self.finger_tips = self._rotate_feature(angle, self.finger_tips)
        # rotate contour - have to reshape it before and after
        self.hand_contour = self._rotate_feature(
            angle, np.reshape(self.hand_contour, (np.shape(self.hand_contour)[0], 2))
        )
        self.hand_contour = np.reshape(
            self.hand_contour, (np.shape(self.hand_contour)[0], 1, 2)
        )
        # rotate middle finger
        middle_finger = rotatePointAroundOrigin(
            [self.center_x, self.center_y], middle_finger, angle
        )
        # rotate middle defects
        middle_defects = self._rotate_feature(angle, middle_defects)

        return middle_finger, middle_defects

    def align_hand(self):
        # get middle finger and where it meets palm
        middle_finger = self._get_middle_finger()
        middle_defects = self._get_middle_defects(middle_finger)

        # find the midpoint of the middle defects
        # use it to calculate angle between
        # y-axis and line joining middle finger and midpoint
        mid_point_defects = np.int0(tuple(np.mean(middle_defects, axis=0)))
        line = middle_finger - mid_point_defects
        angle_radians = angleBetween(line, (0, -1))

        # if line has positive slope, we want to rotate counter clockwise
        if line[0] > 0:
            angle_radians = -angle_radians

        # translate and rotate hand
        self._translate_hand()
        self._rotate_hand(angle_radians)

        # translate and rotate all hand features
        middle_finger, middle_defects = self._translate_features(
            middle_finger, middle_defects
        )
        middle_finger, middle_defects = self._rotate_features(
            angle_radians, middle_finger, middle_defects
        )
        mid_point_defects = np.int0(tuple(np.mean(middle_defects, axis=0)))

        return (
            self.image,
            self.finger_tips,
            self.hand_contour,
            middle_finger,
            middle_defects,
            mid_point_defects,
        )

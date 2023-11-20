import cv2 as cv
import numpy as np
from geometry import distanceBetween


class HandAnalyzer:
    # default constructor
    def __init__(self, image: cv.Mat, hand_centroid) -> None:
        self.image = image
        self.hand_centroid = hand_centroid

    # finds all contours of image
    def _find_contours(self):
        contours, _ = cv.findContours(
            self.image, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_NONE
        )
        return contours

    # use largest contour to compute convex hull
    def _find_convex_hull(self, contour, with_points: bool):
        hull = cv.convexHull(contour, returnPoints=with_points)
        if with_points:
            return np.reshape(hull, (hull.shape[0], 2))
        return hull

    # use largest contour and convex hull to find defects
    def _find_convexity_defects(self, contour, hull):
        return cv.convexityDefects(contour, hull)

    # utility function to check whether an element is in a list
    def _not_in(self, element, list):
        not_in = True
        for el in list:
            if (el == element).all():
                not_in = False
                return not_in
        return not_in

    # calculate distance between two points on the contour
    def _distance_along_contour(self, i, j, contour):
        # get index of i
        possible = np.where(contour == i)[0]
        for p in possible:
            if (contour[p] == i).all():
                index_i = p
                break

        # get index of j
        possible = np.where(contour == j)[0]
        for p in possible:
            if (contour[p] == j).all():
                index_j = p
                break

        # return difference between indices
        return np.abs(index_i - index_j)

    # clusters points that are close together
    def _cluster_finger_tips(self, finger_tips, hand_contour):
        # remove points that are close together
        new_finger_tips = []
        for i in finger_tips:
            toRemove = np.asarray([i])
            for j in finger_tips:
                # if they are close, add them to be removed
                if (
                    self._distance_along_contour(i, j, hand_contour) < 250
                    or distanceBetween(i, j) < 100
                ):
                    toRemove = np.append(toRemove, [j], axis=0)
            # if we actually found nearby elements
            if len(toRemove) > 1:
                # we add the mean of them and remove them from array
                new_finger_tips.append(np.mean(toRemove, axis=0))
                finger_tips = [
                    finger for finger in finger_tips if self._not_in(finger, toRemove)
                ]
        return np.int0(np.asarray(new_finger_tips))

    # extracts the finger tips from the convex hull
    def _extract_finger_tips(self, hull, hand_contour):
        # extract points from hull if they are above hand center
        finger_tips = [pt for pt in hull if pt[1] < self.hand_centroid[1]]
        # cluster finger prints
        finger_tips = self._cluster_finger_tips(finger_tips, hand_contour)

        return finger_tips

    # main method which handles the pipeline of analyzing the hand
    # including: contour, convex hull and convex defect extraction
    # also uses heuristics to extract the 5 finger tips
    def analyze_hand(self):
        # find contour of hand
        contours = self._find_contours()
        hand_contour = contours[0]

        # get convex hull of contour, with and without points
        hull_no_pts = self._find_convex_hull(hand_contour, False)
        hull_pts = self._find_convex_hull(hand_contour, True)

        # find convexity defects using contour and hull
        defects = self._find_convexity_defects(hand_contour, hull_no_pts)

        # find finger tips using convex hull
        finger_tips = self._extract_finger_tips(hull_pts, hand_contour)

        # if there are too few finger tips, return error
        if len(finger_tips) < 4:
            return False

        return self.image, hand_contour, contours, defects, finger_tips

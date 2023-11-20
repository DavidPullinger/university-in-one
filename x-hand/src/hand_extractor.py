import cv2 as cv
import numpy as np


class HandExtractor:
    # default constructor
    def __init__(self, image: cv.Mat) -> None:
        self.image = image

    # extracts the largest component from the image and returns its label
    def _extract_largest_component(self):
        # extract components
        nb_components, labels, stats, centroids = cv.connectedComponentsWithStats(
            self.image, connectivity=8
        )

        # extract label of largest (area) component
        max_label, max_size = max(
            [
                (i, stats[i, cv.CC_STAT_AREA]) for i in range(1, nb_components)
            ],  # range() starts from 1 since 0 is the background label.
            # we want to find max by looking at STAT_AREA which is
            # the second element of the above iteration
            key=lambda x: x[1],
        )
        return labels, max_label, centroids

    # calculates percentage of white pixels on all sides of image
    def _calc_border_percentage(self):
        height = self.image.shape[0]
        width = self.image.shape[1]

        # horizontal sum
        sum_bottom = 0
        sum_top = 0
        for x in range(width):
            if self.image[height - 1][x] == 255:
                sum_bottom += 1
            if self.image[0][x] == 255:
                sum_top += 1

        # vertical sum
        sum_left = 0
        sum_right = 0
        for y in range(height):
            if self.image[y][0] == 255:
                sum_left += 1
            if self.image[y][width - 1] == 255:
                sum_right += 1

        return [
            sum_top / width,
            sum_bottom / width,
            sum_left / height,
            sum_right / height,
        ]

    # return image of largest component (hand) and its centroid
    def extract_hand(self):
        labels, max_label, centroids = self._extract_largest_component()
        self.image = (labels == max_label).astype("uint8") * 255
        return self.image, self._calc_border_percentage(), centroids[max_label]

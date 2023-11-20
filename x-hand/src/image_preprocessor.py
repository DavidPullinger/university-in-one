import numpy as np
import cv2 as cv


class ImagePreProcessor:
    # defines the ways in which the image can be preprocessed
    NORM_BLUR = 1
    NORM = 2
    BLUR = 3
    NONE = 4

    # default constructor
    def __init__(self, image: cv.Mat) -> None:
        self.image = image

    # applies histogram equalization directly to image
    def _normalize_image(self):
        clahe = cv.createCLAHE(clipLimit=1.0)
        self.image = clahe.apply(self.image)

    # applies gaussian blur directly to image
    def _blur_image(self, kernel_size: tuple, stand_dev: int):
        self.image = cv.GaussianBlur(self.image, kernel_size, stand_dev)

    # applies a binary threshold to image
    def _threshold_image(self):
        # TODO: Find better way of finding threshold
        _, self.image = cv.threshold(
            self.image,
            np.quantile(
                self.image, 0.67
            ),  # finds the 67th percentile of the pixel values
            255,
            cv.THRESH_BINARY,
        )

    # preprocesses image according to the given type
    def preprocess_image(self, type: int, kernel_size: tuple, stand_dev: int):
        # apply various preprocessing methods
        if type == self.NORM_BLUR:
            self._normalize_image()
            self._blur_image(kernel_size, stand_dev)
        elif type == self.NORM:
            self._normalize_image()
        elif type == self.BLUR:
            self._blur_image(kernel_size, stand_dev)

        # apply binary threshold to image
        self._threshold_image()
        return self.image

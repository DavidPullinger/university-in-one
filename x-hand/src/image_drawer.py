import numpy as np
import cv2 as cv


class ImageDrawer:
    RED = (0, 0, 255)
    GREEN = (0, 255, 0)
    BLUE = (255, 0, 0)
    WHITE = (255, 255, 255)
    OFF_WHITE = (254, 254, 254)

    # default constructor
    def __init__(self, image: cv.Mat) -> None:
        if image is None:
            return
        try:
            self.image = self.cvt_color(image)
        except:
            self.image = image
        self.original = self.image.copy()

    def cvt_color(self, image: cv.Mat):
        return cv.cvtColor(image, cv.COLOR_GRAY2RGB)

    def switch_color(self, image: cv.Mat, from_color, to_color):
        image[np.where((image == from_color).all(axis=2))] = [to_color]
        return image

    def draw_contours(self, contours, color):
        cv.drawContours(self.image, contours, -1, color, 5)

    def draw_circles(self, points, color):
        for pt in points:
            cv.circle(self.image, pt, 10, color, -1)

    def draw_line(self, from_pt, to_pt, color):
        cv.line(self.image, from_pt, to_pt, color, 5)

    def get_edited_image(self):
        return self.image

    def get_original_image(self):
        return self.original

    def overlay_images(self, img1, img2, alpha):
        return cv.addWeighted(img1, alpha, img2, 1 - alpha, 0.0)

    def crop_to_fit_dims(self, img1, img2):
        min_height = np.minimum(img1.shape[0], img2.shape[0])
        min_width = np.minimum(img1.shape[1], img2.shape[1])
        return min_width, min_height

    def crop_to_fit(self, img1, img2):
        dims = self.crop_to_fit_dims(img1, img2)
        return cv.resize(img1, dims), cv.resize(img2, dims)

    def write_text(self, text, text2):
        cv.putText(
            self.image, text, (10, 35), cv.FONT_HERSHEY_SIMPLEX, 1, self.WHITE, 2, 2
        )
        cv.putText(
            self.image, text2, (10, 70), cv.FONT_HERSHEY_SIMPLEX, 1, self.WHITE, 2, 2
        )

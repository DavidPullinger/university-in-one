import numpy as np


class ImageProcessingAssessor:
    def __init__(
        self, comp_contour, ideal_contour, comp_fingers, ideal_fingers
    ) -> None:
        # get lengths of contours
        self.comp_length = np.shape(comp_contour)[0]
        self.ideal_length = np.shape(ideal_contour)[0]

        # flatten each point using lengths
        # each element of contour was of form [[x,y]]
        # after reshaping, it is of form [x,y]
        self.comp_contour = np.reshape(comp_contour, (self.comp_length, 2))
        self.ideal_contour = np.reshape(ideal_contour, (self.ideal_length, 2))

        # fingers on each contour
        # we have to find the index of these on the contour
        self.comp_fingers = [
            self.closest_pt_on_contour(x, self.comp_contour)[0] for x in comp_fingers
        ]
        self.comp_fingers[0] = 0
        self.comp_fingers = sorted(self.comp_fingers)

        self.ideal_fingers = [
            self.closest_pt_on_contour(x, self.ideal_contour)[0] for x in ideal_fingers
        ]
        self.ideal_fingers[0] = 0
        self.ideal_fingers = sorted(self.ideal_fingers)

    # gets the index and distance of the closest point on the contour
    def closest_pt_on_contour(self, comp_pt, contour, **kwargs):
        # contour may have a specific window we must search
        start_stop = kwargs.get("start_stop")
        if start_stop:
            contour = contour[np.min(start_stop) : np.max(start_stop)]
        else:
            start_stop = (0, 0)

        # vectorize finding the min distance
        dist = np.sum((contour - comp_pt) ** 2, axis=1)
        min_index = np.argmin(dist)

        # return index of closest pt and distance to closest pt
        # we add start to min_index because the array was potentially sliced
        return min_index + np.min(start_stop), np.sqrt(dist[min_index])

    # calculates errors between two contours
    def tot_distance(self):
        # arrays to return
        dist_arr = []
        comp_pts = []
        ideal_pts = []

        # ensure there are five fingers
        if len(self.comp_fingers) != 5:
            return dist_arr, comp_pts, ideal_pts

        # one loop for each finger
        for finger in range(5):
            # start and end point of search window
            start = self.ideal_fingers[finger]
            stop = self.ideal_length if finger == 4 else self.ideal_fingers[finger + 1]
            window = 100

            for i in range(
                self.comp_fingers[finger],
                self.comp_length if finger == 4 else self.comp_fingers[finger + 1],
            ):
                # simply add point that is on comp_contour
                comp_pt = self.comp_contour[i]

                # find closest point on ideal contour
                last_index, dist = self.closest_pt_on_contour(
                    comp_pt,
                    self.ideal_contour,
                    start_stop=(start, min(start + window, stop)),
                )

                # append ideal point, comp point, and their distance
                dist_arr.append(dist)
                comp_pts.append(comp_pt)
                ideal_pts.append(self.ideal_contour[last_index])

                # if the closest pt is being found again and again,
                # we can expand the window
                if start == last_index:
                    window += 1
                else:
                    window = 100
                start = last_index

        return dist_arr, comp_pts, ideal_pts

import cv2 as cv
import numpy as np


class KMeans:
    # default constructor
    def __init__(self, clustering_factors) -> None:
        self.clustering_factors = np.float32(np.asarray(clustering_factors))

    def cluster_images(self):
        # define criteria, number of clusters(K) and apply kmeans()
        criteria = (cv.TERM_CRITERIA_EPS, 1, 1.0)
        k = min(
            len(self.clustering_factors), 5
        )  # if the number of nodes is >= 5, use 5 clusters
        k = k if k >= 5 else min(2, k)  # if k is < 5, use 2 clusters

        # run k means algorithm
        error, labels, centers = cv.kmeans(
            self.clustering_factors, k, None, criteria, 10, cv.KMEANS_RANDOM_CENTERS
        )

        # return labels of each image
        return labels

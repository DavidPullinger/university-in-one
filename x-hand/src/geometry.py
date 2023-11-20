from math import acos, sqrt, cos, sin
import numpy as np

# finds euclidean distance between two coordinates, which are 1D arrays with 2 elements each
def distanceBetween(ptA: np.ndarray, ptB: np.ndarray):
    return sqrt((ptA[0] - ptB[0]) ** 2 + (ptA[1] - ptB[1]) ** 2)


# finds angle between two vectors, which are 1D arrays with 2 elements each
def angleBetween(ptA: np.ndarray, ptB: np.ndarray):
    dot = np.dot(ptA, ptB)
    magnitude = np.linalg.norm(ptA) * np.linalg.norm(ptB)
    return acos(dot / magnitude)


# rotates point around an origin, by an angle in radians
def rotatePointAroundOrigin(origin: np.ndarray, point: np.ndarray, angle):
    ox, oy = origin
    px, py = point

    qx = ox + cos(angle) * (px - ox) - sin(angle) * (py - oy)
    qy = oy + sin(angle) * (px - ox) + cos(angle) * (py - oy)
    return np.int0([qx, qy])

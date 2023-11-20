from hand_aligner import HandAligner
from hand_analyzer import HandAnalyzer
from image_drawer import ImageDrawer
from hand_extractor import HandExtractor
from image_preprocessor import ImagePreProcessor
from image_processing_assessor import ImageProcessingAssessor
from k_means import KMeans

import cv2 as cv
import numpy as np
import base64
import eel


eel.init("web")  # initialise Eel with web/ directory


@eel.expose
def image_pipeline(file_name, **kwargs):
    # read image from file or from parameter
    if kwargs.get("image") is None:
        img = cv.imread("assets/" + file_name, 0)
    else:
        img = kwargs.get("image")

    # check if passed image is valid
    if img is None:
        FAIL = "\033[91m"
        print(
            FAIL + "Please ensure your input image is located in the assets/ directory."
        )
        exit()

    # If an explicit crop has been provided
    if kwargs.get("crop") is not None:
        img = cv.resize(img, kwargs.get("crop"), interpolation=cv.INTER_NEAREST)

    # create copy of original image for later
    original = img.copy()

    # Preprocess image
    preprocessor = ImagePreProcessor(img)
    img = preprocessor.preprocess_image(ImagePreProcessor.NORM_BLUR, (101, 101), 5)

    # Extract hand
    extractor = HandExtractor(img)
    hand, percentages, hand_centroid = extractor.extract_hand()

    # If the percentage of the hand on any borders is greater than 0.35,
    # we need to crop and reprocess
    top = percentages[0] > 0.35
    bottom = percentages[1] > 0.35
    left = percentages[2] > 0
    right = percentages[3] > 0
    if top or bottom or left or right:
        # reprocess with cropped image
        return image_pipeline(
            None,
            image=original[
                # top : bottom
                15 * top : -15 if bottom else None,
                # left : right
                10 * left : -10 if right else None,
            ],
            crop=kwargs.get("crop"),
        )

    # Analyze hand
    analyzer = HandAnalyzer(hand, hand_centroid)
    result = analyzer.analyze_hand()

    # If hand analysis failed - happens when len(finger_tips) < 4
    # we make hand red to indicate it was unsuccessful
    if not result:
        drawer = ImageDrawer(None)
        part_edited = drawer.cvt_color(hand)
        part_edited = drawer.switch_color(part_edited, drawer.WHITE, drawer.RED)
        original = drawer.cvt_color(original)
        overlay = drawer.overlay_images(original, part_edited, 0.5)
        return (part_edited, overlay, None, None)

    # otherwise, we continue
    hand, hand_contour, contours, defects, finger_tips = result

    # Align hand
    aligner = HandAligner(hand, hand_centroid, hand_contour, finger_tips, defects)
    (
        hand,
        finger_tips,
        hand_contour,
        middle_finger,
        middle_defects,
        mid_point_defects,
    ) = aligner.align_hand()

    # Draw features of hand
    drawer = ImageDrawer(hand)
    drawer.draw_contours(hand_contour, drawer.OFF_WHITE)
    drawer.draw_circles(finger_tips, drawer.BLUE)
    drawer.draw_circles(middle_defects, drawer.BLUE)
    drawer.draw_line(mid_point_defects, middle_finger, drawer.BLUE)

    # Create overlay with original images and edited one
    edited = drawer.get_edited_image()
    original = drawer.cvt_color(original)
    overlay = drawer.overlay_images(original, edited, 0.5)
    return (edited, overlay, hand_contour, finger_tips)


@eel.expose
def process_files(files):
    processed = []
    clustering_factors = []

    for f in files:
        edited, overlay, contour, fingers = image_pipeline(f)

        if fingers is not None and len(fingers) == 5 and contour is not None:
            # to cluster the hands, we use the length of the hand contour
            # as well as the coordinates of the 5 fingers
            clustering_factors.append(
                np.asarray(
                    [
                        cv.arcLength(contour, True),
                        *np.reshape(fingers, (10,)),
                    ],
                )
            )

        # edited image
        # we convert it to base64 to send it to web GUI
        retval, buffer = cv.imencode(".png", edited)
        edited_text = base64.b64encode(buffer).decode("ascii")
        # overlay
        # we convert it to base64 to send it to web GUI
        retval, buffer = cv.imencode(".png", overlay)
        overlay_text = base64.b64encode(buffer).decode("ascii")

        # what we want to return
        processed.append(
            {
                "name": f,
                "edited": edited_text,
                "overlay": overlay_text,
                "group": fingers is not None
                and len(fingers) == 5
                and contour is not None,
            }
        )

    # cluster hand using kmeans
    # the factors used to cluster are described earlier
    clusterer = KMeans(clustering_factors)
    labels = clusterer.cluster_images()

    # go through processed images that were part of clustering
    # and assign their corresponding label
    for i, images in enumerate([x for x in processed if x["group"]]):
        images["label"] = str(labels[i][0])

    # return processed images
    return processed


@eel.expose
def compare_ideal(file):
    # read in ideal and comparison images
    # get the dimensions to crop them to the same size
    ideal_raw = cv.imread("assets/ideal.png", 0)
    comp_raw = cv.imread("assets/" + file, 0)
    crop_dims = ImageDrawer(None).crop_to_fit_dims(ideal_raw, comp_raw)

    # process them, passing the crop explicitly
    comp, _, comp_contour, comp_fingers = image_pipeline(
        None, image=comp_raw, crop=crop_dims
    )
    ideal, _, ideal_contour, ideal_fingers = image_pipeline(
        None, image=ideal_raw, crop=crop_dims
    )

    # if we are not able to assess image due to unprocessed image, return
    if comp_contour is None or comp_fingers is None:
        retval, buffer = cv.imencode(".png", comp)
        return base64.b64encode(buffer).decode("ascii")

    # assess image alignment
    assessor = ImageProcessingAssessor(
        comp_contour, ideal_contour, comp_fingers, ideal_fingers
    )
    dist, other_pts, base_pts = assessor.tot_distance()

    # calculate statistics on array of distances
    avg_dist = int(np.mean(dist)) if dist != [] else 0
    stddev_dist = int(np.std(dist)) if dist != [] else 0
    length = len(dist)

    # prepare comp and ideal images
    drawer = ImageDrawer(None)
    ideal = drawer.switch_color(ideal, ImageDrawer.WHITE, ImageDrawer.GREEN)
    overlay = drawer.overlay_images(ideal, comp, 0.5)  # blend the two images

    # draw error lines on overlay image
    drawer = ImageDrawer(overlay)
    for i in range(0, length, 30):
        drawer.draw_circles([other_pts[i], base_pts[i]], drawer.RED)
        drawer.draw_line(other_pts[i], base_pts[i], drawer.RED)

    # label the average distance and the standard deviation
    drawer.write_text(
        "Average distance: " + str(avg_dist),
        "Deviation of distance: " + str(stddev_dist),
    )

    # return overlay image with error lines
    retval, buffer = cv.imencode(".png", overlay)
    return base64.b64encode(buffer).decode("ascii")


if __name__ == "__main__":
    # start server
    eel.start("index.html", mode="")

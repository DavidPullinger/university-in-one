# X-Hand: An X-Ray Preprocessor

## Installation and Execution

### Installation

To install the required Python packages, run `make setup` from the root directory. This should install NumPy, OpenCV, and any necessary GUI libraries.

### Execution

Before execution, please ensure your input images are located in the `assets/` directory.

To execute the program, run `make` or `make run` from the root directory. Please note that the first time you execute it might take a while to start. This should automatically open a browser where you will be able to interact with the program. If this does not happen, please visit [this link](http://localhost:8000/index.html). Please note that when the browser window is closed, the program will automatically exit.

## Classes

### MainUI

One thread running the GUI. User interacts with this object directly via GUI, e.g. clicking buttons, inputting text, etc., which in turn communicates with the MainDriver for user requested actions.

### MainDriver

Driver class of the application. It calls function in other classes for input, each stage of the processing pipeline, and output. Contains main application loop, together with MainUI.

### ImageIO

Handles the input of images, and stores array of images for later use in image processing pipeline.

### ImagePreProcessor

Handles pre processing stages of image processing pipeline. Aims to output a binary image.

- Histogram Norm
- Binary Threshold
- Adaptive Threshold

### HandExtractor

Handles extraction of hand using connected component analysis. Aims to output a binary image which contains only the hand.

- Connected Component Analysis
  - Find connected components
  - Largest one (besides background) is hand

### HandAnalyzer

Handles analysis of the hand; namely, the extraction of fingertips and convex areas where the finger meets the palm.

- Find contour of hand
- Find convex hull using contour
- Find convexity defects using contour and convex hull
- Use convex hull to extract fingertips
- Use convexity defects to extract where hand meets palm

### HandAligner

Handles alignment of hand to the y-axis using fingertips and convex areas where the finger meets the palm.

- Find middle finger fingertip and the convex areas to the left and right of it
- Use these features to compute the angle that the hand needs to be rotated
- Center the hand via translation
- Align the hand using computed angle

### ImageProcessingAssessor

Assesses the accuracy of the hand extraction and alignment by computing a sum of squared errors across the boundary of a hand, comparing it to a preselected "ideal" hand. It also allows for a visualization of the alignment accuracy by superimposing the source image with the processed image.

- Compute a sum of squared errors for the processed hand across the boundary of the hand, comparing it to a preselected "ideal" hand
- Apply a low alpha value to the source and processed image so that they can be superimposed on top of one another
- Super impose images and send the view to the GUI

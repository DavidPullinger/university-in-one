# Image Processor with Connected Components
## About
In the area of image processing, we often threshold the intensity of pixels in a greyscale image to produce a new “binary” image which has foreground (intensity=255) pixels corresponding to the object(s) of interest (inputs >= threshold value), and background pixels (intensity=0), corresponding to image parts which are not of interest (lower than threshold value input). This serves as the first stage to more complex processing to allow more accurate detection of objects, and allows us to reason about object sizes and to reject erroneous object detections in an image (or video).

## Compilation and Execution
The program can be compiled by running `make` in the root directory.

The program can be executed by running:

``` findcomp [options] <inputPGMfile>```

where `<inputPGMfile>` is a valid PGM image and valid options are:

1) `-s <int> <int>` (-s min max): set the minimum and maximum valid components size (min default = 3, max = image width*img height)
2) `-t <int>`: set the threshold (pixel value) for component detection (default=128, limit to (0...255)).
3) `-p` print out all the component data (entries do not have to be sorted) as well as the total component number and the sizes of the smallest and largest components.
4) `-w <outPGMfile>`: write out all retained components (as foreground/background pixels) to a new PGM file, `<outPGMfile>`

## Mastery Work

### Multiset

I used a multiset as a container for ConnectedComponents, making it faster to access biggest and smallest elements.

### Boundary Pixels

I attempted to add methods to count boundary pixels. This is done via a function in the `ConnectedComponent` class, called `calculateBoundaryPixels`. This boundary count can then be access via a simple accessor method. This functionality is enabled with a `-b` flag.

## Testing

### Compilation

To compile files for testing, run `make test.exe`

### Execution

To execute test file, run `./test.exe`

## Files

### findcomp

This file is the driver file of the application. It uses the `CommandLineUtils` class to parse the command line arguments. These arguments are then used to read in the relevant input file, when creating a `PGMimageProcessor` object. This object is then used to extract the connected components, print them to an output PGM file, and print their details to std::cout.

### CommandLineUtils

This class has a constructor which receives both the `argc` and `argv` variables (count of CL arguments and actual CL arguments resp.) from `findcomp.cpp`. It then loops through these arguments, creating relevant data structures (such as a `pair` for min/max sizes) when it encounters a flag (such as an `-s`).

### ConnectedComponent

This class is a helper class for the `PGMimageProcessor` class. It encapsulates all the data for a ConnectedComponent, namely id, pixels, and pixel count. It contains all the necessary constructors and destructors, as well as all the necessary getters and setters.

### PGMimageProcessor

This class handles most of the logic in the application. It instantiates a 2D dynamic array with an input PGM file, and later uses this 2D array to create a set of ConnectedComponents. If the necessary flags are set, these ConnectedComponents are output to a PGM file (where ConnectedComponent pixels are white, and other pixels are black), and/or their details are output to std::cout.

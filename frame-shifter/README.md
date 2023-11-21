# PGM Frame Extractor

## About
Provided with a very large input image (for example, 16K x 16K “image moscaic” from an astronomy sky survey) we want to produce a video that captures the movement of a much smaller window across this large image. You can think of this as lens that allows us to focus on a trajectory of interest. To do this, we need to position a rectangular window within this large image and extract all the pixels that overlap it to produce one frame. As we shift the window to a new offset position within the large input image, we will produce a sequence of image frames that can be written out and passed into a video creation tool such as FFMPEG to create a video.

## Compilation and Execution
To compile the program, simply run `make` in the root directory of this project. Also, make sure there is a directory called `output` in the root directory.

Then, the program can be executed as follows:

```./extractor myLargeImage.pgm -t 0 10 5000 5000 -s 640 480 -w invert invseq -w none noneseq```

This would read in a PGM image (myLargeImage.pgm) and define a 640 × 480 window, with starting origin (0,10) and final origin (5000,5000) and then extract two sequences from this. The first sequence would apply the invert operator and write the output frames to `output/invseqNNNN.pgm` and the second would simply do the frame extraction with no image operation, and write the frames to `output/noneseqNNNN.pgm`. Note that if the frame ever goes over the image's borders, it will be black (white if the inversion operation is used).

The following operations are available:

1) none: no modification of data
2) invert: each pixel value v become 255-v
3) reverse: reverse output (write frames out from last to first)
4) revinvert: reverse and invert output.

There is a 4th available parameter (in addition to `-t,-s,-w`) which can define a custom path that the frame should follow, and overrules the `-t` flag. This parameter (prefaced by the flag `-p`) is used as follows:

``` -p n x1 y1 x2 y2 ... xn yn ```

This will allow you to specify coordinates that the frame top left corner will move to as it proceeds from the start to end frame coordinates. By default (using only the `-t` flag) it just goes from start to end, in a straight line — corresponding to:

```-p 2 x1 y1 x2 y2```

Using the provided example image, `image.pgm`, the reader may want to try the following command:

```./extractor image.pgm -s 640 480 -w none noneseq -p 4 0 0 2000 0 2000 1000 4000 1900```

The output of this command has also been provided in `output_video.mp4`, for reference.
## Output
The frames are output as described above, and they can be visualized as a video using FFMPEG, as follows:

```ffmpeg -framerate [frame rate] -i output/sequence_name%04d.pgm output_video.mp4```

For a reasonable framerate, you should consider how many frames were generated, and then divide this number by how long (in seconds) you want the video to be. For example, if 5000 frames were generated for a specific sequence, and you wanted the video to 10 seconds long, you should make the framerate 500.
## Files

### extractor

This file is the driver file of the application. It uses the `CommandLineUtils` class to parse the command line arguments. These arguments are then used to read in the relevant input file, using the `FileUtils` class. The coordinates are calculated and frames outputted in the `FrameSequence` class. Finally, these frames are written to the relevant file using the `FileUtils` class.

### CommandLineUtils

This class has a constructor which receives both the `argc` and `argv` variables (count of CL arguments and actual CL arguments resp.) from `extractor.cpp`. It then loops through these arguments, creating relevant data structures (such as a `pair` for dimensions) when it encounters a flag (such as an `-s`).

### FileUtils

This class has methods which write to and read from PGM files. It also has a method for interpreting the user's operations (invert, reverse, revinvert or none) and appropriately outputting the pixels to a file.

### FrameSequence

This class has a constructor which accepts the dimensions of the frame, as well as the points the frame must visit, as defined by the `-t` or `-p` flags. It then calculates a valid (smooth) trajectory between all these points. Finally, it uses this trajectory (set of coordinates) to create a sequence of frames, which `extractor` passes to `FileUtils`, to be output to PGM files.

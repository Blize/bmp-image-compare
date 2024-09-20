# bmp-image comparison library

This library is designed to compare two BMP images and returns an percentage of how similar they are to each other.

## Info

This is a rather small project for me to learn more about C and image processing. As its a lot of fun to write C.
But I think this gives and pretty good insight on how to read BMP files and do different things with them.
If you are interested in more info I have an image in /info about BMP headers.

## Installation

To install the library, simply run the following commands:

Make sure you have an c compiler installed and make (for Makefile).

1. Clone the repository

```bash
git clone https://github.com/Blize/bmp-image-compare.git
```

2. Build the project

```bash
make
```

3. Run the executable

```bash
./bmp_compare static/1.bmp static/1_lines.bmp
```

## Usage / Current Features

This library has a few different ways of comparing the images. For example comparing every RGB to each other
or comparing if there is an brightness shift in the image. And I am also working on comparing shapes in the image using the sobel operator.

An example output could look like this (comparing `static/1.bmp` with `static/1_lines.bmp`):

```
-------------- BMP File Header --------------
File Path: static/1.bmp
File Type: 4d42
File Size: 818058 bytes
Pixel Data Offset: 138 bytes
Image Width: 640 pixels
Image Height: 426 pixels
Bit Count: 24 (should be 24 for 24-bit)
Compression: 0 (should be 0 for no compression)
Image Size: 817920 bytes (biSizeImage)
---------------------------------------------

-------------- BMP File Header --------------
File Path: static/1_lines.bmp
File Type: 4d42
File Size: 818058 bytes
Pixel Data Offset: 138 bytes
Image Width: 640 pixels
Image Height: 426 pixels
Bit Count: 24 (should be 24 for 24-bit)
Compression: 0 (should be 0 for no compression)
Image Size: 817920 bytes (biSizeImage)
---------------------------------------------

---- Image Comparison Results: Every RGB -----
Average Red Similarity: 99.86%
Average Green Similarity: 99.88%
Average Blue Similarity: 99.87%
Average Overall Similarity: 99.87%

----------------------------------------------

---- Image Comparison Results: Color Shift ---
Average Red Brightness Shift: 99.86%
Average Green Brightness Shift: 99.88%
Average Blue Brightness Shift: 99.87%
Average Overall Brightness Shift: 99.87%
----------------------------------------------

------- Image Comparison Results: Shape ------
Number of differing pixels: 1022
Similarity: 99%
----------------------------------------------
```

### Explanation

1. `Every RGB` compares every pixel in the image to each other and calculates the similarity using the `compare_values` function found in [util.c](./src/util/util.c).
2. `Color Shift` calculates the average difference of the R/G/B values and checks if they are increasing or decreasing all by the same amount.
3. `Shape` compares the shapes in the image using the sobel operator. This is still a work in progress and is not fully implemented yet. Currently we just count if
   the pixels that are edges are the same amount in both images. _Note_ though that we also generate a new image with the edges found in the image (Find examples in [examples directory](./static/example/edges/)).

## Future Features

I want to add some more features in the future if I have time and motivation for this.
The main two are following:

- Add different Bitmap file types support -> currently only supports 24-bit BMP files.
- Recognize patterns in binary edges

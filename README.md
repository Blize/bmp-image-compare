# bmp-image comparison library

This library is designed to compare two BMP images and returns an perentage of how similiar they are to each other.
Currently there is only one way of comparing images, but more will be added in the future.

## Info

This is a rather small project for me to learn more about C and image processing. As its a lot of fun to write c.
But I think this gives and pretty good insight on how to read BMP files and do diffrent things with them.

## Installation

To install the library, simply run the following commands:

Make sure you have an c compiler installed and make.

1. Clone the repository

```bash
git clone https://github.com/Blize/bmp-image-comparison.git
```

2. Build the project

```bash
make
```

3. Run the exectuable

```bash
./bmp_compare static/1.bmp static/1_lines.bmp
```

## Usage / Current Features

The library currently only supports comparing two images and returning a percentage of how similiar they are to each other.
That said the comparission is between every R / G / B value of every pixel between the two images. After that it sums then and divides by the total amount of pixels to get a percentage.
With that we get similarity values for R / G / B and overall.

This method is good for comparing images that are the same, but have been modified in some way, like adding lines to the image.
For example comparing this image...
![image](static/1.bmp)
...with this image...
![image](static/1_lines.bmp)

...will gives us an high similarity value, since the only difference is the lines added to the second image.
Example output:

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

Average Red Similarity: 99.86%
Average Green Similarity: 99.88%
Average Blue Similarity: 99.87%
Average Overall Similarity: 99.87%
```

What won't work (atm) is comparing images that are different in some way, like having different colors / contrasts / shadow / brightness but same image as a base.
For example comparing this image...
![image](static/1.bmp)
...with this image...
![image](static/1_brighter.bmp)
...will give us a low similarity value, since the images are different in brightness.

## Future Features

I want to add some more features in the future if I have time and motivation for this.
The main two are following:

- Add more ways of comparing images -> starting with the brightness / contrast / shadow / color difference.
- Add diffrent Bitmap file types support -> currently only supports 24-bit BMP files.
- Add different image resolutions support -> currently only supports the same resolutions of the two images.

#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t
#define LONG int32_t

#pragma pack(push, 1)

// BMP file header (14 bytes)
typedef struct {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAP_FILE_HEADER;

// BMP info header (40 bytes for BITMAPINFOHEADER)
typedef struct {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAP_INFO_HEADER;

typedef struct {
    int *red;
    int *green;
    int *blue;
    int width;
    int height;
} RGB_Array;


typedef struct {
    RGB_Array resized;
    RGB_Array original;
} Resized_Result;

#pragma pack(pop)

bool imageCheck(FILE *file, BITMAP_FILE_HEADER *header, BITMAP_INFO_HEADER *infoHeader, const char *filename);
void writeRGBValuesToTextFile(FILE *bmpFile, BITMAP_FILE_HEADER header, BITMAP_INFO_HEADER infoHeader, const char *outputFilename);
RGB_Array readRGBValues(FILE *bmpFile, BITMAP_FILE_HEADER header, BITMAP_INFO_HEADER infoHeader);
float compare_values(int *a, int *b);
int* rgbToGrayscale(RGB_Array arr);
int* sobelOperator(int width, int height, int *grayscale);
int* thresholdEdges(int width, int height, int *edges, int thresholdValue);
int compareBinaryImages(int width, int height, int *binaryOne, int *binaryTwo);
void writeBMP(const char *filename, int width, int height, int *binaryImage);
Resized_Result setImagesToSameSize(RGB_Array arr1, RGB_Array arr2);


#endif

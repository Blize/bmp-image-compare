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

#pragma pack(pop)

bool imageCheck(FILE *file, BITMAP_FILE_HEADER *header, BITMAP_INFO_HEADER *infoHeader, const char *filename);
float compare_values(int *a, int *b);

#endif

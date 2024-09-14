#include "util.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool imageCheck(FILE *file, BITMAP_FILE_HEADER *header, BITMAP_INFO_HEADER *infoHeader, const char *filename) {
    fread(header, sizeof(BITMAP_FILE_HEADER), 1, file);
    fread(infoHeader, sizeof(BITMAP_INFO_HEADER), 1, file);

    //  -- Check BMP Extension --     -- Check Bit Count --          -- Check if Compression --
    if (header->bfType != 0x4D42 || infoHeader->biBitCount != 24 || infoHeader->biCompression != 0) {
        printf("Invalid BMP file.\n");
        printf("-------------- BMP File Header --------------\n");
        printf("File Path: %s\n", filename);
        printf("File Type: %x\n", header->bfType);
        printf("File Size: %u bytes\n", header->bfSize);
        printf("Pixel Data Offset: %u bytes\n", header->bfOffBits);
        printf("Image Width: %d pixels\n", infoHeader->biWidth);
        printf("Image Height: %d pixels\n", infoHeader->biHeight);
        printf("Bit Count: %d (should be 24 for 24-bit)\n", infoHeader->biBitCount);
        printf("Compression: %u (should be 0 for no compression)\n", infoHeader->biCompression);
        printf("Image Size: %u bytes (biSizeImage)\n", infoHeader->biSizeImage);
        printf("---------------------------------------------\n");
        printf("\n");
        return false;
    }

    printf("-------------- BMP File Header --------------\n");
    printf("File Path: %s\n", filename);
    printf("File Type: %x\n", header->bfType);
    printf("File Size: %u bytes\n", header->bfSize);
    printf("Pixel Data Offset: %u bytes\n", header->bfOffBits);
    printf("Image Width: %d pixels\n", infoHeader->biWidth);
    printf("Image Height: %d pixels\n", infoHeader->biHeight);
    printf("Bit Count: %d (should be 24 for 24-bit)\n", infoHeader->biBitCount);
    printf("Compression: %u (should be 0 for no compression)\n", infoHeader->biCompression);
    printf("Image Size: %u bytes (biSizeImage)\n", infoHeader->biSizeImage);
    printf("---------------------------------------------\n");
    printf("\n");

    return true;
}


float compare_values(int *a, int *b) {
    if (*a < 0 || *a > 255 || *b < 0 || *b > 255) {
        printf("Invalid RGB value: %d, %d\n", *a, *b);
        return 0.0;
    }
    float diff = abs(*a - *b) / 255.0; 
    return 100.0 * (1.0 - pow(diff, 0.5));  // We pow by 0.5 to drop off exponentially
}

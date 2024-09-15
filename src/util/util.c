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
    return 100.0 * (1.0 - pow(diff, 0.5)); 
}


/// Convert an RGB array to a grayscale array
/// Using the NTSC formula: Y = 0.3R + 0.59G + 0.11B
int* rgbToGrayscale(RGB_Array arr) {
    int totalPixels = arr.width * arr.height;
    int* grayscale = (int *)malloc(totalPixels * sizeof(int));
    
    for (int i = 0; i < totalPixels; i++) {
        grayscale[i] = (int)(0.3 * arr.red[i] + 0.59 * arr.green[i] + 0.11 * arr.blue[i]);
    }
    
    return grayscale;  
}


/// Apply the Sobel operator to an image
/// gx = [1 0 -1]   gy = [ 1  2  1]
///      [2 0 -2]        [ 0  0  0]
///      [1 0 -1]        [-1 -2 -1]
int* sobelOperator(int width, int height, int *grayscale) {
    int* edges = (int *)malloc(width * height * sizeof(int));
    int gx, gy, gradient;
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int idx = y * width + x;
            
            gx = (-1 * grayscale[(y - 1) * width + (x - 1)]) + (-2 * grayscale[y * width + (x - 1)]) + (-1 * grayscale[(y + 1) * width + (x - 1)]) +
                 (1 * grayscale[(y - 1) * width + (x + 1)]) + (2 * grayscale[y * width + (x + 1)]) + (1 * grayscale[(y + 1) * width + (x + 1)]);
                 
            gy = (-1 * grayscale[(y - 1) * width + (x - 1)]) + (-2 * grayscale[(y - 1) * width + x]) + (-1 * grayscale[(y - 1) * width + (x + 1)]) +
                 (1 * grayscale[(y + 1) * width + (x - 1)]) + (2 * grayscale[(y + 1) * width + x]) + (1 * grayscale[(y + 1) * width + (x + 1)]);
                 
            gradient = sqrt(gx * gx + gy * gy);
            edges[idx] = gradient > 255 ? 255 : gradient;        
        }
    }
    
    return edges;
}


int* thresholdEdges(int width, int height, int *edges, int thresholdValue) {
    int totalPixels = width * height;
    int* binaryEdges = (int *)malloc(totalPixels * sizeof(int));
    
    for (int i = 0; i < totalPixels; i++) {
        binaryEdges[i] = (edges[i] > thresholdValue) ? 255 : 0; 
    }
    
    return binaryEdges;  
}


int compareBinaryImages(int width, int height, int *binary1, int *binary2) {
    int differences = 0;
    
    for (int i = 0; i < width * height; i++) {
        if (binary1[i] != binary2[i]) {
            differences++;
        }
    }
    
    return differences;
}

void writeBMP(const char *filename, int width, int height, int *binaryImage) {
    BITMAP_FILE_HEADER fileHeader;
    BITMAP_INFO_HEADER infoHeader;

    // Calculate padding (each row must be a multiple of 4 bytes)
    int padding = (4 - (width % 4)) % 4;
    int rowSize = width + padding;
    int imageSize = rowSize * height;

    // BMP File Header
    fileHeader.bfType = 0x4D42;
    fileHeader.bfSize = sizeof(BITMAP_FILE_HEADER) + sizeof(BITMAP_INFO_HEADER) + imageSize;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(BITMAP_FILE_HEADER) + sizeof(BITMAP_INFO_HEADER) + 256 * 4; 

    // BMP Info Header
    infoHeader.biSize = sizeof(BITMAP_INFO_HEADER);
    infoHeader.biWidth = width;
    infoHeader.biHeight = abs(height);
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 8;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = imageSize;
    infoHeader.biXPelsPerMeter = 2835;
    infoHeader.biYPelsPerMeter = 2835;
    infoHeader.biClrUsed = 256;
    infoHeader.biClrImportant = 256;

    // Open the file for writing
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    // Write headers
    fwrite(&fileHeader, sizeof(BITMAP_FILE_HEADER), 1, file);
    fwrite(&infoHeader, sizeof(BITMAP_INFO_HEADER), 1, file);

    for (int i = 0; i < 256; i++) {
        BYTE color[4] = {i, i, i, 0};         
        fwrite(color, sizeof(BYTE), 4, file);  
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BYTE pixelValue = binaryImage[y * width + x];
            fwrite(&pixelValue, sizeof(BYTE), 1, file);
        }

        BYTE paddingBytes[3] = {0, 0, 0};
        fwrite(paddingBytes, sizeof(BYTE), padding, file);
    }

    fclose(file);
}

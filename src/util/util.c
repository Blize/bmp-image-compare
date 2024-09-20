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

void writeRGBValuesToTextFile(FILE *bmpFile, BITMAP_FILE_HEADER header, BITMAP_INFO_HEADER infoHeader, const char *outputFilename) {
    // Calculate row size (with padding)
    int rowSize = (infoHeader.biWidth * 3 + 3) & ~3;

    // Move to pixel data
    fseek(bmpFile, header.bfOffBits, SEEK_SET);

    // Allocate memory for one row of pixel data
    BYTE *pixelRow = malloc(rowSize);
    if (!pixelRow) {
        printf("Memory allocation failed.\n");
        fclose(bmpFile);
        return;
    }

    // Open the output file to write RGB values in human-readable format
    FILE *outputFile = fopen(outputFilename, "w");
    if (!outputFile) {
        printf("Error opening output file: %s\n", outputFilename);
        free(pixelRow);
        fclose(bmpFile);
        return;
    }

    // Read and write pixel data row by row
    for (int i = 0; i < infoHeader.biHeight; i++) {
        fread(pixelRow, rowSize, 1, bmpFile);
        for (int j = 0; j < infoHeader.biWidth; j++) {
            BYTE blue = pixelRow[j * 3];
            BYTE green = pixelRow[j * 3 + 1];
            BYTE red = pixelRow[j * 3 + 2];

            // Write RGB values in text format
            fprintf(outputFile, "%d,%d,%d\n", red, green, blue);
        }
    }

    // Cleanup
    free(pixelRow);
    fclose(outputFile);
}

RGB_Array readRGBValues(FILE *bmpFile, BITMAP_FILE_HEADER header, BITMAP_INFO_HEADER infoHeader) {
    // Create an RGB_Array struct to store the values
    RGB_Array rgbArray;
    rgbArray.width = abs(infoHeader.biWidth);
    rgbArray.height = abs(infoHeader.biHeight);

    // Calculate the number of pixels
    int numPixels = rgbArray.width * rgbArray.height;

    // Allocate memory for red, green, and blue arrays
    rgbArray.red = (int *)malloc(numPixels * sizeof(int));
    rgbArray.green = (int *)malloc(numPixels * sizeof(int));
    rgbArray.blue = (int *)malloc(numPixels * sizeof(int));

    if (!rgbArray.red || !rgbArray.green || !rgbArray.blue) {
        printf("Memory allocation failed.\n");
        // Free any memory already allocated
        free(rgbArray.red);
        free(rgbArray.green);
        free(rgbArray.blue);
        exit(1);
    }

    // Calculate row size (with padding)
    int rowSize = (infoHeader.biWidth * 3 + 3) & ~3;

    // Move to pixel data
    fseek(bmpFile, header.bfOffBits, SEEK_SET);

    // Allocate memory for one row of pixel data
    BYTE *pixelRow = malloc(rowSize);
    if (!pixelRow) {
        printf("Memory allocation failed for pixel row.\n");
        free(rgbArray.red);
        free(rgbArray.green);
        free(rgbArray.blue);
        exit(1);
    }

    // Read pixel data row by row and store in RGB_Array
    int pixelIndex = 0;
    for (int i = 0; i < infoHeader.biHeight; i++) {
        fread(pixelRow, rowSize, 1, bmpFile);
        for (int j = 0; j < infoHeader.biWidth; j++) {
            rgbArray.blue[pixelIndex] = pixelRow[j * 3];
            rgbArray.green[pixelIndex] = pixelRow[j * 3 + 1];
            rgbArray.red[pixelIndex] = pixelRow[j * 3 + 2];
            pixelIndex++;
        }
    }

    // Cleanup
    free(pixelRow);

    return rgbArray;
}

float compare_values(int *a, int *b) {
    if (*a < 0 || *a > 255 || *b < 0 || *b > 255) {
        printf("Invalid RGB value: %d, %d\n", *a, *b);
        return 0.0;
    }
    float diff = abs(*a - *b) / 255.0; 
    return 100.0 * (1.0 - pow(diff, 0.5)); 
}


Resized_Result setImagesToSameSize(RGB_Array arr1, RGB_Array arr2) {

    Resized_Result result;
    RGB_Array *smallerImage, *largerImage;
    if (arr1.width * abs(arr1.height) < arr2.width * abs(arr2.height)) {
        smallerImage = &arr1;
        largerImage = &arr2;
    } else {
        smallerImage = &arr2;
        largerImage = &arr1;
    }

    result.original = *smallerImage;

    RGB_Array newImage;
    newImage.width = smallerImage->width;
    newImage.height = abs(smallerImage->height);

    int totalPixels = newImage.width * newImage.height;
    newImage.red = (int *)malloc(totalPixels * sizeof(int));
    newImage.green = (int *)malloc(totalPixels * sizeof(int));
    newImage.blue = (int *)malloc(totalPixels * sizeof(int));

    if (!newImage.red || !newImage.green || !newImage.blue) {
        printf("Memory allocation failed.\n");
        free(newImage.red);
        free(newImage.green);
        free(newImage.blue);
        exit(1);
    }
    
    double scaleX = (double)largerImage->width / newImage.width;
    double scaleY = (double)largerImage->height / newImage.height;

    for (int y = 0; y < newImage.height; y++) {
        for (int x = 0; x < newImage.width; x++) {

            int srcX = (int)(x * scaleX);
            int srcY = (int)(y * scaleY);

            int srcIndex = srcY * largerImage->width + srcX;
            int destIndex = y * newImage.width + x;

            newImage.red[destIndex] = largerImage->red[srcIndex];
            newImage.green[destIndex] = largerImage->green[srcIndex];
            newImage.blue[destIndex] = largerImage->blue[srcIndex];
        }
    }


    result.resized = newImage;
    return result;
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

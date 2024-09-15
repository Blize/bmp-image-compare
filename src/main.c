#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./util/util.h"
#include "./compare/compare.h"

void freeRGBArray(RGB_Array *arr) {
    free(arr->red);
    free(arr->green);
    free(arr->blue);
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

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 2) {
        printf("Usage: %s <file1.bmp> <file2.bmp>\n", argv[0]);
        printf("For more information, use: %s -h or --help\n", argv[0]);
        return 1;
    }


    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <file1.bmp> <file2.bmp>\n", argv[0]);
        printf("\n");
        printf("This program compares two BMP images and outputs the similarity between them.\n");
        printf("\n");
        printf("1. Compare every RGB Value of the two images\n");
        printf("   Value n1 and n2 will be given into the formula: 100 * (1 - |n1 - n2| / 255)^0.5\n");
        printf("\n");
        printf("2. Compare the Brightness Shift of the two images\n");
        printf("   The average Brightness Shift of the two images will be calculated and then compared to the Brightness Shift of every pixel\n");

        return 1;
    }

    FILE *file1 = fopen(argv[1], "rb");
    if (!file1) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    FILE *file2 = fopen(argv[2], "rb");
    if (!file2) {
        printf("Error opening file: %s\n", argv[2]);
        fclose(file1);
        return 1;
    }

    BITMAP_FILE_HEADER header1, header2;
    BITMAP_INFO_HEADER infoHeader1, infoHeader2;

    // Check both BMP files
    if (!imageCheck(file1, &header1, &infoHeader1, argv[1]) || !imageCheck(file2, &header2, &infoHeader2, argv[2])) {
        fclose(file1);
        fclose(file2);
        return 1;
    }

    // This is another function that writes every RGB Value into an File, you can uncomment this if you want that
    // Write the RGB values of the first image to "image1.txt"
    // writeRGBValuesToTextFile(file1, header1, infoHeader1, "image1.txt");

    // Write the RGB values of the second image to "image2.txt"
    // writeRGBValuesToTextFile(file2, header2, infoHeader2, "image2.txt");

    RGB_Array arr1 = readRGBValues(file1, header1, infoHeader1);
    RGB_Array arr2 = readRGBValues(file2, header2, infoHeader2);
    
    compareRGBArrays(arr1, arr2);
    compareBrightnessShift(arr1, arr2);
    compareShapes(arr1, arr2);

    int *grayscale = rgbToGrayscale(arr1);
    int *edges = sobelOperator(arr1.width, arr1.height, grayscale);
    int *binaryImage = thresholdEdges(arr1.width, arr1.height, edges, 128);

    writeBMP("edges_detected.bmp", arr1.width, arr1.height, binaryImage);

    // Close both BMP files and free Memory
    fclose(file1);
    fclose(file2);
    freeRGBArray(&arr1);
    freeRGBArray(&arr2);

    return 0;
}

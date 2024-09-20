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

    Resized_Result arrs;
    
    arrs.resized = arr1;
    arrs.original = arr2;

    if (arr1.width != arr2.width || arr1.height != arr2.height) {
      arrs = setImagesToSameSize(arr1, arr2);
    } 
     
    compareRGBArrays(arrs.original, arrs.resized);
    compareBrightnessShift(arrs.original, arrs.resized);
    compareShapes(arrs.original, arrs.resized); 

    // Close both BMP files and free Memory
    fclose(file1);
    fclose(file2);
    freeRGBArray(&arrs.original);
    freeRGBArray(&arrs.resized);

    return 0;
}

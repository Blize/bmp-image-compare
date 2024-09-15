#include "../util/util.h"
#include <math.h>
#include <stdlib.h>

void compareRGBArrays(RGB_Array arr1, RGB_Array arr2) {
    if (arr1.width != arr2.width || arr1.height != arr2.height) {
        printf("Arrays have different dimensions, cannot compare.\n");
        return;
    }

    int numPixels = arr1.width * arr1.height;

    float totalRedSimilarity = 0.0;
    float totalGreenSimilarity = 0.0;
    float totalBlueSimilarity = 0.0;

    for (int i = 0; i < numPixels; i++) {
        totalRedSimilarity += compare_values(&arr1.red[i], &arr2.red[i]);
        totalGreenSimilarity += compare_values(&arr1.green[i], &arr2.green[i]);
        totalBlueSimilarity += compare_values(&arr1.blue[i], &arr2.blue[i]);
    }

    // Calculate the average for each color channel
    float avgRed = totalRedSimilarity / numPixels;
    float avgGreen = totalGreenSimilarity / numPixels;
    float avgBlue = totalBlueSimilarity / numPixels;

    // Calculate the overall average similarity
    float overallAvg = (avgRed + avgGreen + avgBlue) / 3.0;


    printf("---- Image Comparison Results every RGB -----\n");
    printf("Average Red Similarity: %.2f%%\n", avgRed);
    printf("Average Green Similarity: %.2f%%\n", avgGreen);
    printf("Average Blue Similarity: %.2f%%\n", avgBlue);
    printf("Average Overall Similarity: %.2f%%\n", overallAvg);
    printf("----------------------------------------------\n");
    printf("\n");
}

void compareBrightnessShift(RGB_Array arr1, RGB_Array arr2) {
    if (arr1.width != arr2.width || arr1.height != arr2.height) {
        printf("Arrays have different dimensions, cannot compare.\n");
        return;
    }
    
    int numPixels = arr1.width * arr1.height;

    float redSimilarities = 0.0;
    int totalShiftRed = 0;
    
    float greenSimilarities = 0.0;
    int totalShiftGreen = 0;

    float blueSimilarities = 0.0;
    int totalShiftBlue = 0;
  
    for (int i = 0; i < numPixels; i++) {
      totalShiftRed += abs(arr1.red[i] - arr2.red[i]);
      totalShiftGreen += abs(arr1.green[i] - arr2.green[i]);
      totalShiftBlue += abs(arr1.blue[i] - arr2.blue[i]);
    }

    int expectedRedShift = totalShiftRed / numPixels;
    int expectedGreenShift = totalShiftGreen / numPixels;
    int expectedBlueShift = totalShiftBlue / numPixels;

    for (int i = 0; i < numPixels; i++) {
      int actualRedShift = abs(arr1.red[i] - arr2.red[i]);
      float similarityRed = compare_values(&actualRedShift, &expectedRedShift);
      redSimilarities += similarityRed;

      int actualGreenShift = abs(arr1.green[i] - arr2.green[i]);
      float similarityGreen = compare_values(&actualGreenShift, &expectedGreenShift);
      greenSimilarities += similarityGreen;

      int actualBlueShift = abs(arr1.blue[i] - arr2.blue[i]);
      float similarityBlue = compare_values(&actualBlueShift, &expectedBlueShift);
      blueSimilarities += similarityBlue;
    }

    float avgRedSimilarity = redSimilarities / numPixels;
    float avgGreenSimilarity = greenSimilarities / numPixels;
    float avgBlueSimilarity = blueSimilarities / numPixels;

    float overallAvg = (avgRedSimilarity + avgGreenSimilarity + avgBlueSimilarity) / 3.0;

    printf("---- Image Comparison Results Color Shift ----\n");
    printf("Average Red Brightness Shift: %.2f%%\n", avgRedSimilarity);
    printf("Average Green Brightness Shift: %.2f%%\n", avgGreenSimilarity);
    printf("Average Blue Brightness Shift: %.2f%%\n", avgBlueSimilarity);
    printf("Average Overall Brightness Shift: %.2f%%\n", overallAvg);
    printf("----------------------------------------------\n");
    printf("\n");
}


void compareShapes(RGB_Array arr1, RGB_Array arr2) {
    if (arr1.width != arr2.width || arr1.height != arr2.height) {
        printf("Images have different dimensions, cannot compare.\n");
        return;
    }

    int width = arr1.width;
    int height = arr1.height;

    int* grayscale1 = rgbToGrayscale(arr1);
    int* grayscale2 = rgbToGrayscale(arr2);

    int* edges1 = sobelOperator(width, height, grayscale1);
    int* edges2 = sobelOperator(width, height, grayscale2);

    int thresholdValue = 128;  
    int* binary1 = thresholdEdges(width, height, edges1, thresholdValue);
    int* binary2 = thresholdEdges(width, height, edges2, thresholdValue);

    
    writeBMP("edges_detected.bmp", arr1.width, arr1.height, binary1);
    writeBMP("edges_detected2.bmp", arr2.width, arr2.height, binary2);

    int differences = compareBinaryImages(width, height, binary1, binary2);

    int similarity = 100 - (int)(pow(((double)differences / (width * height)), 0.75) * 100);

    // Free allocated memory
    free(grayscale1);
    free(grayscale2);
    free(edges1);
    free(edges2);
    free(binary1);
    free(binary2);

    printf("------- Image Comparison Results Shape -------\n");
    printf("Number of differing pixels: %d\n", differences);
    printf("Similarity: %d%%\n", similarity);
    printf("----------------------------------------------\n");
    printf("\n");
}

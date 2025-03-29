#include <stdio.h>
#include <stdlib.h>
#include "operations.h"

RGB **copy(RGB **arr, BmpInfoHead *bmih, int x1, int y1, int x2, int y2, int dx, int dy) {
    if (x1 > x2) {
        int t = x1;
        x1 = x2;
        x2 = t;
    }
    if (y1 > y2) {
        int t = y1;
        y1 = y2;
        y2 = t;
    }
    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    if (x2 > bmih->width)
        x2 = bmih->width;
    if (y2 > bmih->height)
        y2 = bmih->height;
    int sizeX = x2 - x1;
    int sizeY = y2 - y1;
    if (sizeX < 0 || sizeY < 0) {
        printf("Invalid coordinates for copying image.\n");
        exit(40);
    }
    RGB **tempArr = malloc(sizeY * sizeof(RGB *));
    if (!tempArr) {
        printf("Memory allocation failed for temporary image array.\n");
        exit(41);
    }
    for (int i = 0; i < sizeY; i++) {
        tempArr[i] = malloc(sizeX * sizeof(RGB));
        if (!tempArr[i]) {
            printf("Memory allocation failed for temporary image row.\n");
            for (int j = 0; j < i; j++) {
                free(tempArr[j]);
            }
            free(tempArr);
            exit(41);
        }
    }
    for (int i = 0; i < sizeY; i++) {
        for (int j = x1; j < x2; j++) {
            tempArr[i][j - x1] = arr[y1 + i][j];
        }
    }
    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            if (dy + i >= 0 && dy + i < bmih->height && dx + j >= 0 && dx + j < bmih->width) {
                arr[dy + i][dx + j] = tempArr[i][j];
            }
        }
    }
    free(tempArr);
    return arr;
}

void replace_color(RGB ***arr, RGB old_color, RGB new_color, int H, int W) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (((*arr)[i][j].r == old_color.r) &&
                ((*arr)[i][j].g == old_color.g) &&
                ((*arr)[i][j].b == old_color.b))
                (*arr)[i][j] = new_color;
        }
    }
}

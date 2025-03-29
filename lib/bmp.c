#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

void print_file_header(BmpFileHead header) {
    printf("signature:\t%x (%hu)\n", header.signature, header.signature);
    printf("filesize:\t%x (%u)\n", header.filesize, header.filesize);
    printf("reserved1:\t%x (%hu)\n", header.reserved1, header.reserved1);
    printf("reserved2:\t%x (%hu)\n", header.reserved2, header.reserved2);
    printf("pixelArrOffset:\t%x (%u)\n\n", header.pixelArrOffset, header.pixelArrOffset);
}

void print_info_header(BmpInfoHead header) {
    printf("headerSize:\t%x (%u)\n", header.headerSize, header.headerSize);
    printf("width: \t%x (%u)\n", header.width, header.width);
    printf("height: \t%x (%u)\n", header.height, header.height);
    printf("planes: \t%x (%hu)\n", header.planes, header.planes);
    printf("bitsPerPixel:\t%x (%hu)\n", header.bitsPerPixel, header.bitsPerPixel);
    printf("compression:\t%x (%u)\n", header.compression, header.compression);
    printf("imageSize:\t%x (%u)\n", header.imageSize, header.imageSize);
    printf("xPixelsPerMeter:\t%x (%u)\n", header.xPixelsPerMeter, header.xPixelsPerMeter);
    printf("yPixelsPerMeter:\t%x (%u)\n", header.yPixelsPerMeter, header.yPixelsPerMeter);
    printf("colorsInColorTable:\t%x (%u)\n", header.colorsInColorTable, header.colorsInColorTable);
    printf("importantColorCount:\t%x (%u)\n", header.importantColorCount, header.importantColorCount);
}

RGB **read_bmp(char *file_name, BmpFileHead *bmfh, BmpInfoHead *bmif) {
    FILE *f;
    if ((f = fopen(file_name, "rb")) != NULL) {
        if (fread(bmfh, 1, sizeof(BmpFileHead), f) &&
            fread(bmif, 1, sizeof(BmpInfoHead), f)) {
            unsigned int height = bmif->height;
            unsigned int width = bmif->width;
            int pad = (4 - (width * 3) % 4) % 4;
            RGB **arr_pix = malloc(height * sizeof(RGB *));
            fseek(f, bmfh->pixelArrOffset, SEEK_SET);
            for (int i = 0; i < height; i++) {
                arr_pix[height - 1 - i] = malloc(width * sizeof(RGB) + pad);
                fread(arr_pix[height - 1 - i], 1, width * sizeof(RGB) + pad, f);
            }
            fclose(f);
            return arr_pix;
        } else {
            printf("Error (40): Wrong input file option\n");
            exit(40);
        }
    } else {
        printf("Error (40): Wrong input file option\n");
        exit(40);
    }
}

void write_bmp(char *file_name, RGB **arr_pix, int height, int width, BmpFileHead *bmfh, BmpInfoHead *bmif) {
    FILE *f = fopen(file_name, "wb");
    fwrite(bmfh, 1, sizeof(BmpFileHead), f);
    fwrite(bmif, 1, sizeof(BmpInfoHead), f);
    int pad = (4 - (width * 3) % 4) % 4;
    fseek(f, bmfh->pixelArrOffset, SEEK_SET);
    for (int i = 0; i < height; i++) {
        fwrite(arr_pix[height - 1 - i], 1, width * sizeof(RGB) + pad, f);
    }
    fclose(f);
}

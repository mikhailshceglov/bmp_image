#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#pragma pack(push, 1)
typedef struct BmpFileHead {
    unsigned short signature;
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int pixelArrOffset;
} BmpFileHead;

typedef struct BmpInfoHead {
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int xPixelsPerMeter;
    unsigned int yPixelsPerMeter;
    unsigned int colorsInColorTable;
    unsigned int importantColorCount;
} BmpInfoHead;
#pragma pack(pop)

typedef struct RGB {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} RGB;

void print_file_header(BmpFileHead header);
void print_info_header(BmpInfoHead header);

RGB **read_bmp(char *file_name, BmpFileHead *bmfh, BmpInfoHead *bmif);
void write_bmp(char *file_name, RGB **arr_pix, int height, int width, BmpFileHead *bmfh, BmpInfoHead *bmif);

#endif

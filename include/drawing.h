#ifndef DRAWING_H
#define DRAWING_H

#include "bmp.h"

void fill_circle(RGB ***arr, int x0, int y0, int radius, RGB fill_color, int H, int W);
void draw_8pixels(RGB ***arr, int x0, int y0, int x, int y, int thickness, RGB color, int H, int W);
void draw_circle(RGB ***arr, int x0, int y0, int radius, int thickness, RGB color, int is_fill, RGB fill_color, int H, int W);
void line(RGB ***pixPtr, int x0, int y0, int x1, int y1, int t, RGB color, int H, int W);
void rectangle(RGB ***arr, int x0, int y0, int x1, int y1, int thickness, RGB color, int H, int W);

typedef struct {
    int x;
    int y;
} coords_t;

void rect_pattern(RGB ***arr, int thickness, int count, RGB color, int H, int W);
void fill_out_circle(RGB ***arr, int x0, int y0, int radius, RGB fill_color, int H, int W);
void draw_semicircles(RGB ***arr, int count, int thickness, RGB color, int H, int W);
RGB **drawHex(RGB **arr, BmpInfoHead *bmih, int x0, int y0, int radius, int thickness, RGB color, int fill, RGB fillColor);

#endif

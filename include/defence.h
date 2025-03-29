#ifndef DEFENCE_H
#define DEFENCE_H

#include "bmp.h"

void binarize_image(RGB **arr, int H, int W, int threshold);

RGB **compress(RGB **arr, int H, int W, int num);

void diag_mirror(RGB ***arr, BmpInfoHead *bmih, int x0, int y0, int x1, int y1);

void blur(RGB **arr, BmpInfoHead *bmih, int size);



RGB **outside_ornament(RGB **arr, int H, int W, int thickness, RGB color);

void shift_x(RGB **arr, int H, int W, int step);

void shift_y(RGB **arr, int H, int W, int step);

void shift_image(RGB **arr, int H, int W, const char *axis, int step);

#endif

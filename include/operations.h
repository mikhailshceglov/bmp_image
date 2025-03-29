#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "bmp.h"

RGB **copy(RGB **arr, BmpInfoHead *bmih, int x1, int y1, int x2, int y2, int dx, int dy);
void replace_color(RGB ***arr, RGB old_color, RGB new_color, int H, int W);

#endif

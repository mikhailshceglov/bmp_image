#ifndef UTILS_H
#define UTILS_H

#include "bmp.h"

void swap_int(int *a, int *b);
void swap_rgb(RGB *a, RGB *b);
int colorCmp(RGB color1, RGB color2);
int check_color_range(int r, int g, int b); 

#endif

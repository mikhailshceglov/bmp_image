#include "utils.h"

void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_rgb(RGB *a, RGB *b) {
    RGB tmp = *a;
    *a = *b;
    *b = tmp;
}

int colorCmp(RGB color1, RGB color2) {
    if (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b)
        return 1;
    else
        return 0;
}

int check_color_range(int r, int g, int b) {
    return (r >= 0 && r <= 255 &&
            g >= 0 && g <= 255 &&
            b >= 0 && b <= 255);
}
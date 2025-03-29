// options.c
#include <stdio.h>
#include <stdlib.h>
#include "options.h"
#include "utils.h"

RGB parseColorOption(const char *str) {
    int r, g, b;
    if (sscanf(str, "%d.%d.%d", &r, &g, &b) != 3) {
        fprintf(stderr, "Error (41): Unknown color argument\n");
        exit(41);
    }
    if (!check_color_range(r, g, b)) {
        fprintf(stderr, "Error (41): Color values must be in the range 0-255.\n");
        exit(41);
    }
    RGB result;
    result.r = (unsigned char)r;
    result.g = (unsigned char)g;
    result.b = (unsigned char)b;
    return result;
}

RGB parseFillColorOption(const char *str) {
    int r, g, b;
    if (sscanf(str, "%d.%d.%d", &r, &g, &b) != 3) {
        fprintf(stderr, "Error (41): Unknown fill color argument\n");
        exit(41);
    }
    if (!check_color_range(r, g, b)) {
        fprintf(stderr, "Error (41): Fill color values must be in the range 0-255.\n");
        exit(41);
    }
    RGB result;
    result.r = (unsigned char)r;
    result.g = (unsigned char)g;
    result.b = (unsigned char)b;
    return result;
}

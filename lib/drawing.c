#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "drawing.h"
#include "bmp.h"
#include "utils.h"

void fill_circle(RGB ***arr, int x0, int y0, int radius, RGB fill_color, int H, int W) {
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            int x_r = x0 + j;
            int y_r = y0 - i;
            if (i * i + j * j <= radius * radius && x_r >= 0 && x_r < W && y_r >= 0 && y_r < H)
                (*arr)[y_r][x_r] = fill_color;
        }
    }
}

void draw_8pixels(RGB ***arr, int x0, int y0, int x, int y, int thickness, RGB color, int H, int W) {
    fill_circle(arr, x + x0, y + y0, thickness, color, H, W);
    fill_circle(arr, x + x0, -y + y0, thickness, color, H, W);
    fill_circle(arr, -x + x0, -y + y0, thickness, color, H, W);
    fill_circle(arr, -x + x0, y + y0, thickness, color, H, W);
    fill_circle(arr, y + x0, x + y0, thickness, color, H, W);
    fill_circle(arr, y + x0, -x + y0, thickness, color, H, W);
    fill_circle(arr, -y + x0, -x + y0, thickness, color, H, W);
    fill_circle(arr, -y + x0, x + y0, thickness, color, H, W);
}

void draw_circle(RGB ***arr, int x0, int y0, int radius, int thickness, RGB color, int is_fill, RGB fill_color, int H, int W) {
    int x = 0;
    int y = radius;
    if (is_fill) {
        fill_circle(arr, x0, y0, radius, fill_color, H, W);
    }
    int dist = 3 - 2 * y;
    while (x <= y) {
        draw_8pixels(arr, x0, y0, x, y, thickness, color, H, W);
        if (dist < 0)
            dist = dist + 4 * x + 6;
        else {
            dist = dist + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void line(RGB ***pixPtr, int x0, int y0, int x1, int y1, int t, RGB color, int H, int W) {
    if (x0 == x1) {
        if (x0 + t / 2 < 0 || x0 - t / 2 > W) {
            return;
        }
        if (y0 > y1)
            swap_int(&y0, &y1);
        for (int y = y0; y < y1; y++) {
            fill_circle(pixPtr, x0, y, t / 2, color, H, W);
        }
        return;
    }
    const int delta_x = abs(x1 - x0);
    const int delta_y = abs(y1 - y0);
    const int sign_x = x0 < x1 ? 1 : -1;
    const int sign_y = y0 < y1 ? 1 : -1;
    int error = delta_x - delta_y;
    fill_circle(pixPtr, x1, y1, t / 2, color, H, W);
    while (x0 != x1 || y0 != y1) {
        fill_circle(pixPtr, x0, y0, t / 2, color, H, W);
        int error2 = error * 2;
        if (error2 > -delta_y) {
            error -= delta_y;
            x0 += sign_x;
        }
        if (error2 < delta_x) {
            error += delta_x;
            y0 += sign_y;
        }
    }
}

void rectangle(RGB ***arr, int x0, int y0, int x1, int y1, int thickness, RGB color, int H, int W) {
    line(arr, x0, y0, x1, y0, thickness, color, H, W);
    line(arr, x1, y0, x1, y1, thickness, color, H, W);
    line(arr, x1, y1, x0, y1, thickness, color, H, W);
    line(arr, x0, y1, x0, y0, thickness, color, H, W);
}

void rect_pattern(RGB ***arr, int thickness, int count, RGB color, int H, int W) {
    int index = 0;
    int x0 = thickness / 2;
    int y0 = thickness / 2;
    int x1 = (thickness % 2 == 0) ? W - thickness / 2 : W - thickness / 2 - 1;
    int y1 = (thickness % 2 == 0) ? H - thickness / 2 : H - thickness / 2 - 1;
    coords_t first = { x0, y0 };
    coords_t second = { x1, y1 };
    int widthOfSpan = 2 * thickness;
    while (index != count) {
        rectangle(arr, first.x, first.y, second.x, second.y, thickness, color, H, W);
        index++;
        x0 += widthOfSpan;
        y0 += widthOfSpan;
        x1 -= widthOfSpan;
        y1 -= widthOfSpan;
        if (x0 > W / 2 || y0 > H / 2)
            return;
        first.x = x0; first.y = y0;
        second.x = x1; second.y = y1;
    }
}

void fill_out_circle(RGB ***arr, int x0, int y0, int radius, RGB fill_color, int H, int W) {
    if (H > W) {
        radius = W / 2;
    }
    for (int i = -W; i <= W; i++) {
        for (int j = -H; j <= H; j++) {
            int x_r = x0 + j;
            int y_r = y0 - i;
            if (i * i + j * j > radius * radius && x_r >= 0 && x_r < W && y_r >= 0 && y_r < H)
                (*arr)[y_r][x_r] = fill_color;
        }
    }
}

void draw_semicircles(RGB ***arr, int count, int thickness, RGB color, int H, int W) {

    unsigned int radiusForHorizontal = (W + 2 * count - 1) / (2 * count);
    unsigned int radiusForVertical   = (H + 2 * count - 1) / (2 * count);

    // Вывод отладочной информации
    // printf("Image dimensions: width = %d, height = %d\n", W, H);
    // printf("Computed radii: radiusForHorizontal = %u, radiusForVertical = %u\n", radiusForHorizontal, radiusForVertical);

    for (int i = 1; i < 2 * count + 1; i += 2) {
        // Верхний полукруг: центр на верхней границе
        {
            int centerX = i * radiusForHorizontal;
            int centerY = 0;
            // printf("Top semicircle (i=%d): centerX = %d, centerY = %d, radius = %u\n", i, centerX, centerY, radiusForHorizontal);
            draw_circle(arr, centerX, centerY, radiusForHorizontal, thickness / 2, color, 0, color, H, W);
        }
        // Нижний полукруг: центр на нижней границе
        {
            int centerX = i * radiusForHorizontal;
            int centerY = H - 1;
            // printf("Bottom semicircle (i=%d): centerX = %d, centerY = %d, radius = %u\n", i, centerX, centerY, radiusForHorizontal);
            draw_circle(arr, centerX, centerY, radiusForHorizontal, thickness / 2, color, 0, color, H, W);
        }
        // Левый полукруг: центр на левой границе
        {
            int centerX = 0;
            int centerY = i * radiusForVertical;
            // printf("Left semicircle (i=%d): centerX = %d, centerY = %d, radius = %u\n", i, centerX, centerY, radiusForVertical);
            draw_circle(arr, centerX, centerY, radiusForVertical, thickness / 2, color, 0, color, H, W);
        }
        // Правый полукруг: центр на правой границе
        {
            int centerX = W - 1;
            int centerY = i * radiusForVertical;
            // printf("Right semicircle (i=%d): centerX = %d, centerY = %d, radius = %u\n", i, centerX, centerY, radiusForVertical);
            draw_circle(arr, centerX, centerY, radiusForVertical, thickness / 2, color, 0, color, H, W);
        }
    }
}



RGB **drawHex(RGB **arr, BmpInfoHead *bmih, int x0, int y0, int radius, int thickness, RGB color, int fill, RGB fillColor) {
    int h = (int)((sqrt(3) * radius) / 2);
    y0 = bmih->height - y0;
    int coord[7][2] = {
        {x0 - radius, y0},
        {x0 - radius / 2, y0 - h},
        {x0 + radius / 2, y0 - h},
        {x0 + radius, y0},
        {x0 + radius / 2, y0 + h},
        {x0 - radius / 2, y0 + h},
        {x0 - radius, y0}
    };
    if (fill) {
        for (int i = coord[1][1] + thickness / 2 + 1; i < coord[4][1] - thickness / 2; i++) {
            for (int j = coord[1][0]; j <= coord[4][0]; j++) {
                if (i >= 0 && i < bmih->height && j >= 0 && j < bmih->width) {
                    arr[i][j] = fillColor;
                }
            }
        }
        int x1 = coord[1][0], y1 = coord[1][1];
        int x2 = coord[5][0], y2 = coord[5][1];
        int x3 = coord[0][0], y3 = coord[0][1];
        for (int i = y1; i < y2; i++) {
            for (int j = x3; j < x0 - radius / 2; j++) {
                if (i >= 0 && i < bmih->height && j >= 0 && j < bmih->width) {
                    int a = (x1 - j) * (y2 - y1) - (x2 - x1) * (y1 - i);
                    int b = (x2 - j) * (y3 - y2) - (x3 - x2) * (y2 - i);
                    int c = (x3 - j) * (y1 - y3) - (x1 - x3) * (y3 - i);
                    if ((a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0)) {
                        arr[i][j] = fillColor;
                    }
                }
            }
        }
        x1 = coord[2][0], y1 = coord[2][1];
        x2 = coord[4][0], y2 = coord[4][1];
        x3 = coord[3][0], y3 = coord[3][1];
        for (int i = y1; i < y2; i++) {
            for (int j = x0 + radius / 2; j < x3; j++) {
                if (i >= 0 && i < bmih->height && j >= 0 && j < bmih->width) {
                    int a = (x1 - j) * (y2 - y1) - (x2 - x1) * (y1 - i);
                    int b = (x2 - j) * (y3 - y2) - (x3 - x2) * (y2 - i);
                    int c = (x3 - j) * (y1 - y3) - (x1 - x3) * (y3 - i);
                    if ((a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0)) {
                        arr[i][j] = fillColor;
                    }
                }
            }
        }
    }
    for (int i = 1; i < 7; i++) {
        line(&arr, coord[i - 1][0], coord[i - 1][1], coord[i][0], coord[i][1], thickness, color, bmih->height, bmih->width);
    }
    return arr;
}

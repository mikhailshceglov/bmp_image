#include <stdio.h>
#include <stdlib.h>
#include "defence.h"
#include "utils.h"
#include <string.h>


// --binarization
void binarize_image(RGB **arr, int H, int W, int threshold) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int sum = arr[i][j].r + arr[i][j].g + arr[i][j].b;
            if (sum >= threshold) {
                // Белый
                arr[i][j].r = 255;
                arr[i][j].g = 255;
                arr[i][j].b = 255;
            } else {
                // Чёрный
                arr[i][j].r = 0;
                arr[i][j].g = 0;
                arr[i][j].b = 0;
            }
        }
    }
}


// --compress
RGB **compress(RGB **arr, int H, int W, int num) {

    int height = H / num;
    int width  = W / num;

    int pad = (4 - (width * 3) % 4) % 4;

    // Выделяем память
    RGB **temp_arr = malloc(height * sizeof(RGB *));
    for (int i = 0; i < height; i++) {
        temp_arr[height - 1 - i] = malloc(width * sizeof(RGB) + pad);
    }

    // Заполняем новую картинку
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++) {

            int sum_r = 0, sum_g = 0, sum_b = 0;
            for (int k = 0; k < num; k++) {
                for (int l = 0; l < num; l++) {
                    sum_r += arr[i * num + k][j * num + l].r;
                    sum_g += arr[i * num + k][j * num + l].g;
                    sum_b += arr[i * num + k][j * num + l].b;
                }
            }

            double avg_r = (double)sum_r / (num * num);
            double avg_g = (double)sum_g / (num * num);
            double avg_b = (double)sum_b / (num * num);

            temp_arr[i][j].r = (unsigned char)avg_r;
            temp_arr[i][j].g = (unsigned char)avg_g;
            temp_arr[i][j].b = (unsigned char)avg_b;
        }
    }

    return temp_arr;
}


// --diag_mirror
void diag_mirror(RGB ***arr, BmpInfoHead *bmih, int x0, int y0, int x1, int y1) {
    int H = bmih->height;
    int W = bmih->width;
    
    if (x0 < 0)
        x0 = 0;
    if (x0 >= W)
        x0 = W - 1;
    if (x1 < 0)
        x1 = 0;
    if (x1 >= W)
        x1 = W - 1;
    if (y0 < 0)
        y0 = 0;
    if (y0 >= H)
        y0 = H - 1;
    if (y1 < 0)
        y1 = 0;
    if (y1 >= H)
        y1 = H - 1;

    if (x0 == x1 || y0 == y1)
        return;

    if (x0 > x1)
        swap_int(&x0, &x1);
    if (y0 > y1)
        swap_int(&y0, &y1);

    int width = x1 - x0;
    int height = y1 - y0;
    if (width != height) {
        if (width < height) {
            y1 = y0 + width;
        } else {
            x1 = x0 + height;
        }
    }

    // Теперь side = (x1 - x0) == (y1 - y0)
    int side = x1 - x0;

    for (int i = 0; i <= side; i++) {
        for (int j = 0; j <= side; j++) {
            if (i < j) {
                int row1 = y0 + i;
                int col1 = x0 + j;
                int row2 = y0 + j;
                int col2 = x0 + i;

                RGB temp = (*arr)[row1][col1];
                (*arr)[row1][col1] = (*arr)[row2][col2];
                (*arr)[row2][col2] = temp;
            }
        }
    }
}


// --blur
void blur(RGB **arr, BmpInfoHead *bmih, int size) {

    int width = bmih->width;
    int height = bmih->height;

    if (size % 2 == 0) {
        size++;
    }

    RGB **temp_arr = malloc(height * sizeof(RGB*));
    for (int i = 0; i < height; i++) {
        temp_arr[height - 1 - i] = malloc(width * sizeof(RGB));
    }

    int sum_r, sum_g, sum_b, count;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            
            sum_r = 0;
            sum_g = 0;
            sum_b = 0;
            count = 0;
            for (int k = i - size / 2; k <= i + size / 2; k++) {
                for (int l = j - size / 2; l <= j + size / 2; l++) {

                    if (k >= 0 && k < height && l >= 0 && l < width){
                        sum_r += arr[k][l].r;
                        sum_g += arr[k][l].g;
                        sum_b += arr[k][l].b;
                        count++;
                    }
                }
            }

            temp_arr[i][j].r = (unsigned char)(sum_r / count);
            temp_arr[i][j].g = (unsigned char)(sum_g / count);
            temp_arr[i][j].b = (unsigned char)(sum_b / count);
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            arr[i][j] = temp_arr[i][j];
        }
    }

    for (int i = 0; i < height; i++) {
        free(temp_arr[i]);
    }
    free(temp_arr);
}

// void draw_triangle(RGB ***arr, int H, int W, int thickness, RGB color, int x0, int y0, int x1, int y1, int x2, int y2) {

//     line(arr, x0, y0, x1, y1, thickness, color, H, W);
//     line(arr, x1, y1, x2, y2, thickness, color, H, W);
//     line(arr, x2, y2, x0, y0, thickness, color, H, W);

// }








RGB **outside_ornament(RGB **arr, int H, int W, int thickness, RGB color) {

    int width = W + 2 * thickness;
    int height = H + 2 * thickness;
    
    int pad = (4 - (width * 3) % 4) % 4;

    RGB **temp_arr = malloc(height * sizeof(RGB *));
    for (int i = 0; i < height; i++) {
        temp_arr[height - 1 - i] = malloc(width * sizeof(RGB) + pad);
    }

    for (int i = thickness; i < H + thickness; i++) {
        for (int j = thickness; j < W + thickness; j++) {
            temp_arr[i][j] = arr[i - thickness][j - thickness];
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i >= H + thickness || i < thickness || j >= W + thickness || j < thickness) {
                temp_arr[i][j].r = color.r;
                temp_arr[i][j].g = color.g;
                temp_arr[i][j].b = color.b;
            }
        }
    }

    return temp_arr;

}








void shift_x(RGB **arr, int H, int W, int step) {

    step = step % W;

    int pad = (4 - (W * 3) % 4) % 4;

    RGB **temp = malloc(H * sizeof(RGB *));
    for (int i = 0; i < H; i++) {
        temp[i] = malloc(W * sizeof(RGB) + pad);
    }


    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {

            int new_col = (j + step) % W;

            temp[i][new_col] = arr[i][j];

        }
    }

    for(int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            arr[i][j] = temp[i][j];
        }
        free(temp[i]);
    }
    free(temp);

}

void shift_y(RGB **arr, int H, int W, int step) {

    step = step % H;

    int pad = (4 - (W * 3) % 4) % 4;

    RGB **temp = malloc(H * sizeof(RGB *));
    for (int i = 0; i < H; i++) {
        temp[i] = malloc(W * sizeof(RGB) + pad);
    }


    for (int i = 0; i < H; i++) {
        int new_row = (i + step) % H;
        for (int j = 0; j < W; j++) {
            temp[new_row][j] = arr[i][j];

        }
    }

    for(int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            arr[i][j] = temp[i][j];
        }
        free(temp[i]);
    }
    free(temp);

}

void shift_image(RGB **arr, int H, int W, const char *axis, int step) {

    if (!axis) return;

    if (strcmp(axis, "x") == 0) {
        shift_x(arr, H, W, step);
    } else if (strcmp(axis, "y") == 0) {
        shift_y(arr, H, W, step);
    } else if (strcmp(axis, "xy") == 0) {
        shift_x(arr, H, W, step);
        shift_y(arr, H, W, step);
    }
}

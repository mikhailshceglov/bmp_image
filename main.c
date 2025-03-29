#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "bmp.h"
#include "drawing.h"
#include "operations.h"
#include "options.h"
#include "defence.h"

void print_help() {
    printf("Course work for option 5.7, created by Mikhail Shceglov.\n");
    puts("\t--help          -h: \tprint some help for user");
    puts("\t--info          -I: \tinfo about image");
    puts("\t--input         -i: \tname of input file");
    puts("\t--output        -o: \tname of output file\n");
    puts("\t--hexagon       -H: \tdrawing hexagon");
    puts("\t--center        -c: \tcenter of hexagon (format: x.y)");
    puts("\t--radius        -r: \tradius of hexagon");
    puts("\t--thickness     -t: \tthickness of hexagon");
    puts("\t--color         -C: \tcolor of hexagon (format: rrr.ggg.bbb)");
    puts("\t--fill          -f: \thexagon is filled");
    puts("\t--fill_color    -F: \tfill color (format: rrr.ggg.bbb)\n");
    puts("\t--copy          -p: \tcopy part of image");
    puts("\t--left_up       -L: \tleft up point (format: x.y)");
    puts("\t--right_down    -R: \tright down point (format: x.y)");
    puts("\t--dest_left_up  -D: \tdestination left up point (format: x.y)\n");
    puts("\t--color_replace -e: \treplace color in image");
    puts("\t--old_color     -O: \told color (format: rrr.ggg.bbb)");
    puts("\t--new_color     -N: \tnew color (format: rrr.ggg.bbb)\n");
    puts("\t--ornament      -n: \tdrawing ornament");
    puts("\t--pattern       -P: \tpattern (rectangle, circle, semicircles)");
    puts("\t--count         -T: \tcount of pattern\n");

    // defence
    puts("\t--binarization  -B : \tбинаризация изображения");
    puts("\t--threshold     -a : \tпорог для бинаризации (целое число > 0)\n");

    puts("\t--compress      -A : \t...");
    puts("\t--num           -b : \t...\n");

    puts("\t--blur          -Q: \tblur image");
    puts("\t--size          -S: \tsize for blur\n");

    puts("\t--diag_mirror   -d: \tdiagonal mirror of area\n");

    puts("\t--outside_ornament   -G: \tornament outside for image\n");
    
    // J K E e k l
    puts("\t--shift   -K:            \tdefence task\n");
    puts("\t--step   -e:             \tpixel step for shift\n");
    puts("\t--axis   -l:             \t'x', 'y' or 'xy' -> shift OX, OY and merge shift OX and OY\n");

}

int main(int argc, char *argv[]) {
    int optindex = 0;
    int optchar = 0;

    short info_flag = 0;
    short input_flag = 0;
    short output_flag = 0;

    short hexagon_flag = 0;
    short center_flag = 0;
    int center_x = 0;
    int center_y = 0;
    short radius_flag = 0;
    int radius = 0;
    short thickness_flag = 0;
    int thickness = 0;
    short color_flag = 0;
    RGB color;
    short fill_flag = 0;
    short fill_color_flag = 0;
    RGB fill_color;

    short copy_flag = 0;
    short left_up_flag = 0;
    int left = 0;
    int up = 0;
    short right_down_flag = 0;
    int right = 0;
    int down = 0;
    short dest_flag = 0;
    int dest_left = 0;
    int dest_up = 0;

    short color_replace_flag = 0;
    short old_color_flag = 0;
    RGB old_color;
    short new_color_flag = 0;
    RGB new_color;

    short ornament_flag = 0;
    short pattern_flag = 0;
    short pattern = 0;
    short count_flag = 0;
    int count = 0;
    short blur_flag = 0;
    int size = 0;

    int flag_index = 0;

    char *file_name;
    char *file_name_out;
    RGB **arr_pix;
    BmpFileHead bmfh;
    BmpInfoHead bmif;

    // defence
    short binarize_flag = 0;
    int threshold_value = 0;

    short compress_flag = 0;
    int num = 0;

    int diag_mirror_flag = 0;

    int outside_ornament_flag = 0;

    int shift_flag = 0;
    //int step_flag = 0;
    //int axis_flag = 0;

    int step = 0;
    char *axis = NULL;




    struct option long_opts[] = {
        {"input",           required_argument, 0, 'i'},
        {"output",          required_argument, 0, 'o'},
        {"info",            no_argument,       0, 'I'},
        {"help",            no_argument,       0, 'h'},
        {"hexagon",         no_argument,       0, 'H'},
        {"center",          required_argument, 0, 'c'},
        {"radius",          required_argument, 0, 'r'},
        {"thickness",       required_argument, 0, 't'},
        {"color",           required_argument, 0, 'C'},
        {"fill",            no_argument,       0, 'f'},
        {"fill_color",      required_argument, 0, 'F'},
        {"copy",            no_argument,       0, 'p'},
        {"left_up",         required_argument, 0, 'L'},
        {"right_down",      required_argument, 0, 'R'},
        {"dest_left_up",    required_argument, 0, 'D'},
        {"color_replace",   no_argument,       0, 'e'},
        {"old_color",       required_argument, 0, 'O'},
        {"new_color",       required_argument, 0, 'N'},
        {"ornament",        no_argument,       0, 'n'},
        {"pattern",         required_argument, 0, 'P'},
        {"count",           required_argument, 0, 'T'},

        // defence
        {"binarization",    no_argument,       0, 'B'},
        {"threshold",       required_argument, 0, 'a'},

        {"compress",        no_argument,       0, 'A'},
        {"num",             required_argument, 0, 'b'},

        {"diag_mirror",     no_argument,       0, 'd'},

        {"blur",            no_argument,       0, 'Q'},
        {"size",            required_argument, 0, 'S'},



        {"outside_ornament",no_argument,       0, 'G'},

        {"shift",           no_argument,       0, 'K'},
        {"step",            required_argument,  0, 'q'},
        {"axis",            required_argument, 0, 'l'},



        {0, 0, 0, 0}
    };


    while ((optchar = getopt_long(argc, argv, "i:o:IhHc:r:t:C:fF:pL:R:D:eO:N:nP:T:QS:dBa:Ab:GKq:l:", long_opts, &optindex)) != -1) {
        switch (optchar) {
            case 'i':
                file_name = optarg;
                input_flag = 1;
                flag_index++;
                break;
            case 'h':
                print_help();
                flag_index++;
                exit(0);
                break;
            case 'o':
                file_name_out = optarg;
                output_flag = 1;
                flag_index++;
                break;
            case 'I':
                info_flag = 1;
                flag_index++;
                break;
            case 'H':
                hexagon_flag = 1;
                flag_index++;
                break;
            case 'c':
                if (sscanf(optarg, "%d.%d", &center_x, &center_y) != 2) {
                    printf("Error (41): Unknown coords of center argument\n");
                    exit(41);
                }
                center_flag = 1;
                flag_index++;
                break;
            case 'r':
                if (sscanf(optarg, "%d", &radius) <= 0) {
                    printf("Error (41): Incorrect radius argument\n");
                    exit(41);
                }
                radius_flag = 1;
                flag_index++;
                break;
            case 't':
                if (sscanf(optarg, "%d", &thickness) != 1 || thickness < 0) {
                    printf("Error (41): Incorrect thickness argument\n");
                    exit(41);
                }
                thickness_flag = 1;
                flag_index++;
                break;
            case 'C':
                color = parseColorOption(optarg);
                color_flag = 1;
                flag_index++;
                break;
            case 'f':
                fill_flag = 1;
                flag_index++;
                break;
            case 'F':
                fill_color = parseFillColorOption(optarg);
                fill_color_flag = 1;
                flag_index++;
                break;
            case 'p':
                copy_flag = 1;
                flag_index++;
                break;
            case 'L':
                if (sscanf(optarg, "%d.%d", &left, &up) != 2) {
                    printf("Error (41): Unknown coords argument\n");
                    exit(41);
                }
                left_up_flag = 1;
                flag_index++;
                break;
            case 'R':
                if (sscanf(optarg, "%d.%d", &right, &down) != 2) {
                    printf("Error (41): Unknown coords argument\n");
                    exit(41);
                }
                right_down_flag = 1;
                flag_index++;
                break;
            case 'D':
                if (sscanf(optarg, "%d.%d", &dest_left, &dest_up) != 2) {
                    printf("Error (41): Unknown coords argument\n");
                    exit(41);
                }
                dest_flag = 1;
                flag_index++;
                break;
            case 'e':
                color_replace_flag = 1;
                flag_index++;
                break;
            case 'O':
                if (sscanf(optarg, "%hhd.%hhd.%hhd", &old_color.r, &old_color.g, &old_color.b) != 3) {
                    printf("Error (41): Unknown old color argument\n");
                    exit(41);
                }
                old_color_flag = 1;
                flag_index++;
                break;
            case 'N':
                if (sscanf(optarg, "%hhd.%hhd.%hhd", &new_color.r, &new_color.g, &new_color.b) != 3) {
                    printf("Error (41): Unknown new color argument\n");
                    exit(41);
                }
                new_color_flag = 1;
                flag_index++;
                break;
            case 'n':
                ornament_flag = 1;
                flag_index++;
                break;
            case 'P':
                if (strcmp(optarg, "rectangle") == 0)
                    pattern = 0;
                else if (strcmp(optarg, "circle") == 0)
                    pattern = 1;
                else if (strcmp(optarg, "semicircles") == 0)
                    pattern = 2;
                else {
                    printf("Error (41): Unknown ornament argument\n");
                    exit(41);
                }
                pattern_flag = 1;
                flag_index++;
                break;
            case 'T':
                if (sscanf(optarg, "%d", &count) != 1 || count <= 0) {
                    printf("Error (41): Unknown count argument\n");
                    exit(41);
                }
                count_flag = 1;
                flag_index++;
                break;
            case 'Q':
                blur_flag = 1;
                flag_index++;
                break;
            case 'S':
                if (sscanf(optarg, "%d", &size) <= 0) {
                    printf("Error (41): Unknown size argument\n");
                    exit(41);
                }
                flag_index++;
                break;
            case 'd':
                diag_mirror_flag = 1;
                flag_index++;
                break;
            case '?':
                printf("Error (41): Unknown or incomplete option or option argument\n");
                exit(41);
                break;

            // defence
            case 'B':
                binarize_flag = 1;
                flag_index++;
                break;

            case 'a':
                if (sscanf(optarg, "%d", &threshold_value) != 1 || threshold_value <= 0) {
                    fprintf(stderr, "Error(41): Incorrect threshold arg\n");
                    exit(41);
                }
                flag_index++;
                break;

            case 'A':
                compress_flag = 1;
                flag_index++;
                break;

            case 'b':
                if (sscanf(optarg, "%d", &num) != 1 || num <= 0) {
                    fprintf(stderr, "Error(41): Incorrect nums arg\n");
                    exit(41);
                } 
                flag_index++;
                break;

            case 'G':
                outside_ornament_flag = 1;
                flag_index++;
                break;

            case 'K':
                shift_flag = 1;
                flag_index++;
                break;

            case 'q':
                if (sscanf(optarg, "%d", &step) != 1 || step <= 0) {
                    fprintf(stderr, "Error(41): Incorrect step arg\n");
                    exit(41);
                } 
                //step_flag = 1;
                flag_index++;
                break;

            case 'l':
                axis = optarg;
                //axis_flag = 1;
                flag_index++;
                break;


        }
    }

    // arg check
    if (optind < argc) {
        fprintf(stderr, "Error (41): Extra arguments detected:");
        for (int i = optind; i < argc; i++) {
            fprintf(stderr, " '%s'", argv[i]);
        }
        fprintf(stderr, " - these arguments will be ignored because the flag does not accept arguments.\n");
        exit(41);
    }

    // input-output file name check 
    if (strcmp(file_name, file_name_out) == 0) {
        fprintf(stderr, "Error (41): Input and output file names must be different.\n");
        exit(41);
    }

    // only one action check
    int action_count = 0;
    if (hexagon_flag)           action_count++;
    if (copy_flag)              action_count++;
    if (color_replace_flag)     action_count++;
    if (ornament_flag)          action_count++;
    
    // defence
    if (blur_flag)              action_count++;
    if (diag_mirror_flag)       action_count++;
    if (binarize_flag)          action_count++;
    if (compress_flag)          action_count++;

    if (outside_ornament_flag)  action_count++;
    if (shift_flag)             action_count++;


    if (action_count > 1) {
        fprintf(stderr, "Error (41): More than one action specified. Please select only one action.\n");
        exit(41);
    }


    if (!input_flag)
        file_name = argv[argc - 1];
    if (!output_flag)
        file_name_out = "out.bmp";
    if (input_flag || (flag_index != argc - 1)) {
        arr_pix = read_bmp(file_name, &bmfh, &bmif);
        if (thickness > bmif.width || thickness > bmif.height) {
            printf("Error (41): Incorrect thickness argument\n");
            exit(41);
        }
        if (info_flag) {
            print_file_header(bmfh);
            print_info_header(bmif);
        }
        if (hexagon_flag) {
            if ((center_flag && radius_flag && thickness_flag && color_flag &&
                 fill_flag && fill_color_flag) ||
                (center_flag && radius_flag && thickness_flag && color_flag &&
                 !fill_flag && fill_color_flag) ||
                (center_flag && radius_flag && thickness_flag && color_flag &&
                 !fill_flag && !fill_color_flag)) {
                drawHex(arr_pix, &bmif, center_x, bmif.height - center_y, radius,
                        thickness, color, fill_flag, fill_color);
            } else {
                printf("Error(41): error in drawing hexagon\n");
                exit(41);
            }
        } else if (copy_flag) {
            if (left_up_flag && right_down_flag && dest_flag) {
                copy(arr_pix, &bmif, left, up, right, down, dest_left, dest_up);
            } else {
                printf("Error(41): Bad options for this task\n");
                exit(41);
            }
        } else if (color_replace_flag) {
            if (old_color_flag && new_color_flag) {
                replace_color(&arr_pix, old_color, new_color, bmif.height, bmif.width);
            } else {
                printf("Error(41): Bad options for this task\n");
                exit(41);
            }
        } else if (ornament_flag) {
            if (pattern == 0) {
                if (pattern_flag && color_flag && thickness_flag && count_flag) {
                    rect_pattern(&arr_pix, thickness, count, color, bmif.height, bmif.width);
                } else {
                    printf("Error(41): Bad options for rectangle ornament\n");
                    exit(41);
                }
            }
            if (pattern == 1) {
                if (pattern_flag && color_flag) {
                    draw_circle(&arr_pix, bmif.width / 2, bmif.height / 2,
                                bmif.height / 2, 1, color, 0, color, bmif.height, bmif.width);
                    fill_out_circle(&arr_pix, bmif.width / 2, bmif.height / 2,
                                    bmif.height / 2, color, bmif.height, bmif.width);
                } else {
                    printf("Error(41): Bad options for circle ornament\n");
                    exit(41);
                }
            }
            if (pattern == 2) {
                if (pattern_flag && color_flag && thickness_flag && count_flag) {
                    draw_semicircles(&arr_pix, count, thickness, color, bmif.height, bmif.width);
                } else {
                    printf("Error(41): Bad options for semicircles ornament\n");
                    exit(41);
                }
            }
        }


        // defence 

        else if (blur_flag) {
            blur(arr_pix, &bmif, size);
        }
        
        else if (diag_mirror_flag) {
            if (left_up_flag && right_down_flag)
                diag_mirror(&arr_pix, &bmif, left, up, right, down);
            else {
                printf("Error(41): wrong flags for diag_mirror\n");
                exit(41);
            }
        }

        else if (binarize_flag) {
            if (threshold_value <= 0) {
                fprintf(stderr, "Error(41): threshold is not set or invalid\n");
                exit(41);
            }
            binarize_image(arr_pix, bmif.height, bmif.width, threshold_value);
        }

        else if (compress_flag) {

            RGB **new_arr = compress(arr_pix, bmif.height, bmif.width, num);

            for (int i = 0; i < bmif.height; i++) {
                free(arr_pix[i]);
            }
            free(arr_pix);

            bmif.height = bmif.height / num;
            bmif.width = bmif.width / num;

            arr_pix = new_arr;
        }

        else if (outside_ornament_flag) {
            RGB **new_arr = outside_ornament(arr_pix, bmif.height, bmif.width, thickness, color);

            for (int i = 0; i < bmif.height; i++) {
                free(arr_pix[i]);
            }
            free(arr_pix);

            bmif.height = bmif.height + 2 * thickness;
            bmif.width = bmif.width + 2 * thickness;

            arr_pix = new_arr;
        }

        else if (shift_flag) {
            shift_image(arr_pix, bmif.height, bmif.width, axis, step);
        }

        
        
        else {
            printf("Error(41): no mode selected\n");
            exit(41);
        }
        write_bmp(file_name_out, arr_pix, bmif.height, bmif.width, &bmfh, &bmif);
        for (int i = 0; i < bmif.height; i++)
            free(arr_pix[i]);
        free(arr_pix);
    }
    return 0;
}

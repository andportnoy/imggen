#include "image.h"
#include <stdio.h>
#include <stdlib.h>

#define COLOR_RANGE 255

void set_random_pixel(Pixel *pixel) {
    pixel->r = (unsigned char)rand() % (COLOR_RANGE + 1);
    pixel->g = (unsigned char)rand() % (COLOR_RANGE + 1);
    pixel->b = (unsigned char)rand() % (COLOR_RANGE + 1);
}

void print_pixel(Pixel *pixel) {
    printf("%-3d %-3d %-3d\t", pixel->r, pixel->g, pixel->b);
}

void set_random_row(Pixel *row, size_t size) {
    size_t i;
    for (i = 0; i < size; ++i) {
        set_random_pixel(row + i);
    }
}

/**
 * Print image.
 */
void print_image(Pixel *image, size_t width, size_t height) {
    size_t i, j;

    /* Print PPM header. */
    printf("P3\n%lu %lu\n%d\n", width, height, COLOR_RANGE);

    for (j = 0; j < height; ++j) {
        for (i = 0; i < width; ++i) {
            print_pixel(image + j * width + i);
        }
        printf("\n");
    }
}

void set_random_image(Pixel *image, size_t width, size_t height) {
    size_t length;
    size_t i;

    length = width * height;
    for (i = 0; i < length; ++i) {
        set_random_pixel(image + i);
    }
}

Pixel *make_image(size_t width, size_t height) {
    return malloc(width * height * sizeof(Pixel));
}

Pixel *make_random_image(size_t width, size_t height) {
    Pixel *image = make_image(width, height);
    if (image) {
        set_random_image(image, width, height);
    }
    return image;
}

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
 * Print image, assume square size.
 */
void print_image(Pixel *image, size_t size) {
    size_t i, j;

    /* Print PPM header. */
    printf("P3\n%lu %lu\n%d\n", size, size, COLOR_RANGE);

    for (j = 0; j < size; ++j) {
        for (i = 0; i < size; ++i) {
            print_pixel(image + j * size + i);
        }
        printf("\n");
    }
}

void set_random_image(Pixel *image, size_t size) {
    size_t length;
    size_t i;

    length = size * size;
    for (i = 0; i < length; ++i) {
        set_random_pixel(image + i);
    }
}

Pixel *make_image(size_t size) { return malloc(size * size * sizeof(Pixel)); }

Pixel *make_random_image(size_t size) {
    Pixel *image = make_image(size);
    if (image) {
        set_random_image(image, size);
    }
    return image;
}

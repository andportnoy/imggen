#include "image.h"
#include <stdio.h>
#include <stdlib.h>

#define COLOR_RANGE 255

void set_random_pixel(Pixel *pixel) {
    pixel->r = (unsigned char)rand() % (COLOR_RANGE + 1);
    pixel->g = (unsigned char)rand() % (COLOR_RANGE + 1);
    pixel->b = (unsigned char)rand() % (COLOR_RANGE + 1);
}

void print_pixel(const Pixel *pixel) {
    printf("%-3d %-3d %-3d\t", pixel->r, pixel->g, pixel->b);
}

void write_pixel(FILE *file, const Pixel *pixel) {
    fprintf(file, "%-3d %-3d %-3d\t", pixel->r, pixel->g, pixel->b);
}

void set_random_row(Pixel *row, size_t width) {
    size_t i;
    for (i = 0; i < width; ++i) {
        set_random_pixel(row + i);
    }
}

/**
 * Print image.
 */
void print_image(const Image *image) {
    size_t i, j;

    /* Print PPM header. */
    printf("P3\n%lu %lu\n%d\n", image->width, image->height, COLOR_RANGE);

    for (j = 0; j < image->height; ++j) {
        for (i = 0; i < image->width; ++i) {
            print_pixel(image->pixels + j * image->width + i);
        }
        printf("\n");
    }
}

void write_image(FILE *file, const Image *image) {
    size_t i, j;

    /* Print PPM header. */
    fprintf(file, "P3\n%lu %lu\n%d\n", image->width, image->height, COLOR_RANGE);

    for (j = 0; j < image->height; ++j) {
        for (i = 0; i < image->width; ++i) {
            write_pixel(file, image->pixels + j * image->width + i);
        }
        fprintf(file, "\n");
    }
}

void set_random_image(Image *image) {
    size_t length;
    size_t i;

    length = image->width * image->height;

    for (i = 0; i < length; ++i) {
        set_random_pixel(image->pixels + i);
    }
}

Image *make_image(size_t width, size_t height) {
    Image *image = malloc(sizeof(*image));
    image->pixels = malloc(width * height * sizeof(Pixel));
    image->width = width;
    image->height = height;
    return image;
}

Image *make_random_image(size_t width, size_t height) {
    Image *image = make_image(width, height);
    if (image) {
        set_random_image(image);
    }
    return image;
}

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

/**
 * x is column offset from left to right (x = 0 is the first column)
 * y is row offset from top to bottom (y = 0 is the first row)
 */
Pixel *pixel_at(const Image *image, size_t x, size_t y) {
    /* Need to check bounds */
    if (x < image->width && y < image->height) {
        return image->pixels + (image->width) * y + x;
    } else {
        fprintf(stderr,
                "Can't access pixel at (%lu, %lu) in %lu x %lu image.\n",
                x, y, image->width, image->height);
        exit(1);
    }
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

void write_image_P3(FILE *file, const Image *image) {
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

void write_image_P6(FILE *file, const Image *image) {
    /* Print PPM header. */
    fprintf(file, "P6\n%lu %lu\n%d\n", image->width, image->height, COLOR_RANGE);
    fwrite(image->pixels,
           sizeof(*(image->pixels)),
           (image->width)*(image->height),
           file);
}

void set_random_image(Image *image) {
    size_t length;
    size_t i;

    length = image->width * image->height;

    for (i = 0; i < length; ++i) {
        set_random_pixel(image->pixels + i);
    }
}

Image *malloc_image(size_t width, size_t height) {
    Image *image = malloc(sizeof(*image));
    image->pixels = malloc(width * height * sizeof(Pixel));
    image->width = width;
    image->height = height;
    return image;
}

Image *malloc_random_image(size_t width, size_t height) {
    Image *image = malloc_image(width, height);
    if (image) {
        set_random_image(image);
    }
    return image;
}

void free_image(Image *image) {
    free(image->pixels);
    free(image);
}

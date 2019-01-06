#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"

/**
 * A couple of interesting things I want to investigate.
 * 1. What happens if we randomly create top row of pixels, then every pixel
 * below is pixel above summed with random increment of random sign?
 * 2. What if we start in the center?
 * 3. Is unsigned char the best type for RGB stuff? Yes!
 * 4. What if evolve only a randomly chosen component of RGB?
 */

/**
 * Evolve pixel dst_pixel based on src_pixel.
 */
void evolve_pixel(Pixel *dst_pixel, Pixel *src_pixel);

/**
 * Evolve pixel dst_pixel based on two pixels, dad_pixel and mom_pixel.
 */
void evolve_pixel2(Pixel *dst_pixel, Pixel *dad_pixel, Pixel *mom_pixel);

/**
 * Evolve row dst_row based on src_row.
 */
void evolve_row(Pixel *dst_row, Pixel *src_row, size_t size);

/**
 * Evolve row dst_row based on src_row using the mom + dad approach.
 */
void evolve_row2(Pixel *dst_row, Pixel *src_row, size_t size);

unsigned char jitter(unsigned char value) {

}

void evolve_pixel(Pixel *dst_pixel, Pixel *src_pixel) {
    dst_pixel->r = src_pixel->r + rand() % 16;
    dst_pixel->g = src_pixel->g + rand() % 16;
    dst_pixel->b = src_pixel->b + rand() % 16;
}

void evolve_pixel2(Pixel *dst_pixel, Pixel *dad_pixel, Pixel *mom_pixel) {
    if (rand() % 2) {
        dst_pixel->r = dad_pixel->r + rand() % 17 - 8;
    } else {
        dst_pixel->r = mom_pixel->r + rand() % 17 - 8;
    }
    if (rand() % 2) {
        dst_pixel->g = dad_pixel->g + rand() % 17 - 8;
    } else {
        dst_pixel->g = mom_pixel->g + rand() % 17 - 8;
    }
    if (rand() % 2) {
        dst_pixel->b = dad_pixel->b + rand() % 17 - 8;
    } else {
        dst_pixel->b = mom_pixel->b + rand() % 17 - 8;
    }
}

void evolve_row(Pixel *dst_row, Pixel *src_row, size_t size) {
    size_t i;
    for (i = 0; i < size; ++i) {
        evolve_pixel(dst_row + i, src_row + i);
    }
}

void evolve_row2(Pixel *dst_row, Pixel *src_row, size_t size) {
    size_t i;
    Pixel *dad_pixel, *mom_pixel;

    /* Evolve first pixel. */
    dad_pixel = src_row + size - 1;
    mom_pixel = src_row + 1;
    evolve_pixel2(dst_row, dad_pixel, mom_pixel);

    /* Evolve middle pixels (all except first and last). */
    for (i = 1; i < size - 1; ++i) {
        dad_pixel = src_row + i - 1;
        mom_pixel = src_row + i + 1;
        evolve_pixel2(dst_row + i, dad_pixel, mom_pixel);
    }

    /* Evolve last pixel. */
    dad_pixel = src_row + size - 2;
    mom_pixel = src_row;
    evolve_pixel2(dst_row + size - 1, dad_pixel, mom_pixel);
}

int main() {
    size_t image_size;
    size_t j;
    Pixel *image;

    image_size = 100;
    srand((unsigned int)time(NULL));

    image = make_image(image_size);
    if (image) {
        set_random_row(image, image_size);
        for (j = 1; j < image_size; ++j) {
            Pixel *dst_row = image + j * image_size;
            Pixel *src_row = image + (j - 1) * image_size;
            evolve_row2(dst_row, src_row, image_size);
        }
        print_image(image, image_size);
        free(image);
    } else {
        printf("Failed to allocate.\n");
        exit(1);
    }
}

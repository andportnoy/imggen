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
 * Evolve row dst_row based on src_row.
 */
void evolve_row(Pixel *dst_row, Pixel *src_row, size_t size);

void evolve_pixel(Pixel *dst_pixel, Pixel *src_pixel) {
    dst_pixel->r = src_pixel->r + rand() % 16;
    dst_pixel->g = src_pixel->g + rand() % 16;
    dst_pixel->b = src_pixel->b + rand() % 16;
}

void evolve_row(Pixel *dst_row, Pixel *src_row, size_t size) {
    size_t i;
    for (i = 0; i < size; ++i) {
        evolve_pixel(dst_row + i, src_row + i);
    }
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
            evolve_row(dst_row, src_row, image_size);
        }
        print_image(image, image_size);
        free(image);
    } else {
        printf("Failed to allocate.\n");
        exit(1);
    }
}

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
 * Jitter value without underflow/overflow (bounce).
 */
unsigned char jitter(unsigned char value);

/**
 * Evolve pixel dst_pixel based on src_pixel.
 */
void evolve_pixel(Pixel *dst_pixel, Pixel *src_pixel);

/**
 * Evolve pixel dst_pixel based on two pixels, dad_pixel and mom_pixel.
 */
void evolve_pixel2(Pixel *dst_pixel, Pixel *dad_pixel, Pixel *mom_pixel);

/**
 * Evolve pixel dst_pixel based on three parent pixels.
 */
void evolve_pixel3(Pixel *dst_pixel, Pixel *parent_pixel1, Pixel *parent_pixel2,
                   Pixel *parent_pixel3);

/**
 * Evolve pixel dst_pixel based on two pixels, dad_pixel and mom_pixel,
 * averaged.
 */
void evolve_pixel4(Pixel *dst_pixel, Pixel *dad_pixel, Pixel *mom_pixel);

/**
 * Evolve row dst_row based on src_row.
 */
void evolve_row(Pixel *dst_row, Pixel *src_row, size_t size);

/**
 * Evolve row dst_row based on src_row using the mom + dad approach.
 */
void evolve_row2(Pixel *dst_row, Pixel *src_row, size_t size);

/**
 * Evolve row dst_row based on src_row using the three parent approach.
 */
void evolve_row3(Pixel *dst_row, Pixel *src_row, size_t size);

/**
 * Evolve row dst_row based on src_row using the mom + dad averaged approach.
 */
void evolve_row4(Pixel *dst_row, Pixel *src_row, size_t size);

unsigned char jitter(unsigned char value) {
    int jitter_amount = rand() % 17 - 8;
    unsigned char result = (unsigned char) (value + jitter_amount);
    if ((jitter_amount < 0 && value < result) ||
        (jitter_amount > 0 && value > result)) {
        /*
         * Small value, negative jitter results in underflow.
         * Want to bounce up and get a small positive value.
         * Example: value = 4, jitter_amount = -7.
         * Wraps to: 253, instead want: 3.
         *
         * Alternatively, big value, positive jitter results in overflow.
         * Want to bounce down and get a big positive value.
         * Example: value = 250, jitter_amount = 10.
         * Wraps to: 4, instead want: 252.
         */
        return (unsigned char)(-result);
    } else {
        return result;
    }
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

void evolve_pixel3(Pixel *dst_pixel, Pixel *parent_pixel1, Pixel *parent_pixel2,
                   Pixel *parent_pixel3) {
    int rand_r = rand() % 3;
    int rand_g = rand() % 3;
    int rand_b = rand() % 3;

    if (rand_r == 0) {
        dst_pixel->r = parent_pixel1->r + rand() % 17 - 8;
    } else if (rand_r == 1) {
        dst_pixel->r = parent_pixel2->r + rand() % 17 - 8;
    } else {
        dst_pixel->r = parent_pixel3->r + rand() % 17 - 8;
    }
    if (rand_g == 0) {
        dst_pixel->g = parent_pixel1->g + rand() % 17 - 8;
    } else if (rand_g == 1) {
        dst_pixel->g = parent_pixel2->g + rand() % 17 - 8;
    } else {
        dst_pixel->g = parent_pixel3->g + rand() % 17 - 8;
    }
    if (rand_b == 0) {
        dst_pixel->b = parent_pixel1->b + rand() % 17 - 8;
    } else if (rand_b == 1) {
        dst_pixel->b = parent_pixel2->b + rand() % 17 - 8;
    } else {
        dst_pixel->b = parent_pixel3->b + rand() % 17 - 8;
    }
}

void evolve_pixel4(Pixel *dst_pixel, Pixel *dad_pixel, Pixel *mom_pixel) {
    dst_pixel->r = jitter((dad_pixel->r + mom_pixel->r) / 2);
    dst_pixel->g = jitter((dad_pixel->g + mom_pixel->g) / 2);
    dst_pixel->b = jitter((dad_pixel->b + mom_pixel->b) / 2);
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

void evolve_row3(Pixel *dst_row, Pixel *src_row, size_t size) {
    size_t i;
    Pixel *parent_pixel1, *parent_pixel2, *parent_pixel3;

    /* Evolve first pixel. */
    parent_pixel1 = src_row + size - 1;
    parent_pixel2 = src_row;
    parent_pixel3 = src_row + 1;
    evolve_pixel3(dst_row, parent_pixel1, parent_pixel2, parent_pixel3);

    /* Evolve middle pixels (all except first and last). */
    for (i = 1; i < size - 1; ++i) {
        parent_pixel1 = src_row + i - 1;
        parent_pixel2 = src_row + i;
        parent_pixel3 = src_row + i + 1;
        evolve_pixel3(dst_row + i, parent_pixel1, parent_pixel2, parent_pixel3);
    }

    /* Evolve last pixel. */
    parent_pixel1 = src_row + size - 2;
    parent_pixel2 = src_row + size - 1;
    parent_pixel3 = src_row;
    evolve_pixel3(dst_row + size - 1, parent_pixel1, parent_pixel2,
                  parent_pixel3);
}

void evolve_row4(Pixel *dst_row, Pixel *src_row, size_t size) {
    size_t i;
    Pixel *dad_pixel, *mom_pixel;

    /* Evolve first pixel. */
    dad_pixel = src_row + size - 1;
    mom_pixel = src_row + 1;
    evolve_pixel4(dst_row, dad_pixel, mom_pixel);

    /* Evolve middle pixels (all except first and last). */
    for (i = 1; i < size - 1; ++i) {
        dad_pixel = src_row + i - 1;
        mom_pixel = src_row + i + 1;
        evolve_pixel4(dst_row + i, dad_pixel, mom_pixel);
    }

    /* Evolve last pixel. */
    dad_pixel = src_row + size - 2;
    mom_pixel = src_row;
    evolve_pixel4(dst_row + size - 1, dad_pixel, mom_pixel);
}

int main() {
    size_t width, height;
    size_t j;
    Pixel *image;

    srand((unsigned int)time(NULL));
    width = 2880;
    height = 1800;

    image = make_image(width, height);
    if (image) {
        set_random_row(image, width);
        for (j = 1; j < height; ++j) {
            Pixel *dst_row = image + j * width;
            Pixel *src_row = image + (j - 1) * width;
            evolve_row4(dst_row, src_row, width);
        }
        print_image(image, width, height);
        free(image);
    } else {
        printf("Failed to allocate.\n");
        exit(1);
    }
}

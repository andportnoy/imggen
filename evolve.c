#include "evolve.h"

/**
 * Jitter value without underflow/overflow (bounce).
 */
unsigned char jitter(unsigned char value);

/**
 * Evolve pixel dst_pixel based on src_pixel.
 */
void evolve_pixel_single_parent(Pixel *dst_pixel, const Pixel *src_pixel);

/**
 * Evolve pixel dst_pixel based on two pixels, dad_pixel and mom_pixel.
 */
void evolve_pixel_dad_mom_genes(Pixel *dst_pixel, const Pixel *dad_pixel,
                                const Pixel *mom_pixel);

/**
 * Evolve pixel dst_pixel based on three parent pixels.
 */
void evolve_pixel_3_parent_genes(Pixel *dst_pixel, const Pixel *parent_pixel1,
                                 const Pixel *parent_pixel2,
                                 const Pixel *parent_pixel3);

/**
 * Evolve pixel dst_pixel based on two pixels, dad_pixel and mom_pixel,
 * averaged.
 */
void evolve_pixel_dad_mom_average(Pixel *dst_pixel, const Pixel *dad_pixel,
                                  const Pixel *mom_pixel);

unsigned char jitter(unsigned char value) {
    int jitter_amount = rand() % 17 - 8;
    unsigned char result = (unsigned char)(value + jitter_amount);
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

void evolve_pixel_single_parent(Pixel *dst_pixel, const Pixel *src_pixel) {
    dst_pixel->r = src_pixel->r + rand() % 16;
    dst_pixel->g = src_pixel->g + rand() % 16;
    dst_pixel->b = src_pixel->b + rand() % 16;
}

/**
 * Implements evolve_row in evolve.h.
 */
void evolve_row_single_parent(Pixel *dst_row, const Pixel *src_row,
                              const size_t size) {
    size_t i;
    for (i = 0; i < size; ++i) {
        evolve_pixel_single_parent(dst_row + i, src_row + i);
    }
}

void evolve_pixel_dad_mom_genes(Pixel *dst_pixel, const Pixel *dad_pixel,
                                const Pixel *mom_pixel) {
    if (rand() % 2) {
        dst_pixel->r = jitter(dad_pixel->r);
    } else {
        dst_pixel->r = jitter(mom_pixel->r);
    }
    if (rand() % 2) {
        dst_pixel->g = jitter(dad_pixel->g);
    } else {
        dst_pixel->g = jitter(mom_pixel->g);
    }
    if (rand() % 2) {
        dst_pixel->b = jitter(dad_pixel->b);
    } else {
        dst_pixel->b = jitter(mom_pixel->b);
    }
}

void evolve_row_dad_mom_genes(Pixel *dst_row, const Pixel *src_row,
                              const size_t size) {
    size_t i;
    const Pixel *dad_pixel, *mom_pixel;

    /* Evolve first pixel. */
    dad_pixel = src_row + size - 1;
    mom_pixel = src_row + 1;
    evolve_pixel_dad_mom_genes(dst_row, dad_pixel, mom_pixel);

    /* Evolve middle pixels (all except first and last). */
    for (i = 1; i < size - 1; ++i) {
        dad_pixel = src_row + i - 1;
        mom_pixel = src_row + i + 1;
        evolve_pixel_dad_mom_genes(dst_row + i, dad_pixel, mom_pixel);
    }

    /* Evolve last pixel. */
    dad_pixel = src_row + size - 2;
    mom_pixel = src_row;
    evolve_pixel_dad_mom_genes(dst_row + size - 1, dad_pixel, mom_pixel);
}

void evolve_pixel_3_parent_genes(Pixel *dst_pixel, const Pixel *parent_pixel1,
                                 const Pixel *parent_pixel2,
                                 const Pixel *parent_pixel3) {
    int rand_r = rand() % 3;
    int rand_g = rand() % 3;
    int rand_b = rand() % 3;

    if (rand_r == 0) {
        dst_pixel->r = jitter(parent_pixel1->r);
    } else if (rand_r == 1) {
        dst_pixel->r = jitter(parent_pixel2->r);
    } else {
        dst_pixel->r = jitter(parent_pixel3->r);
    }
    if (rand_g == 0) {
        dst_pixel->g = jitter(parent_pixel1->g);
    } else if (rand_g == 1) {
        dst_pixel->g = jitter(parent_pixel2->g);
    } else {
        dst_pixel->g = jitter(parent_pixel3->g);
    }
    if (rand_b == 0) {
        dst_pixel->b = jitter(parent_pixel1->b);
    } else if (rand_b == 1) {
        dst_pixel->b = jitter(parent_pixel2->b);
    } else {
        dst_pixel->b = jitter(parent_pixel3->b);
    }
}

void evolve_row_3_parent_genes(Pixel *dst_row, const Pixel *src_row,
                               const size_t size) {
    size_t i;
    const Pixel *parent_pixel1, *parent_pixel2, *parent_pixel3;

    /* Evolve first pixel. */
    parent_pixel1 = src_row + size - 1;
    parent_pixel2 = src_row;
    parent_pixel3 = src_row + 1;
    evolve_pixel_3_parent_genes(dst_row, parent_pixel1, parent_pixel2,
                                parent_pixel3);

    /* Evolve middle pixels (all except first and last). */
    for (i = 1; i < size - 1; ++i) {
        parent_pixel1 = src_row + i - 1;
        parent_pixel2 = src_row + i;
        parent_pixel3 = src_row + i + 1;
        evolve_pixel_3_parent_genes(dst_row + i, parent_pixel1, parent_pixel2,
                                    parent_pixel3);
    }

    /* Evolve last pixel. */
    parent_pixel1 = src_row + size - 2;
    parent_pixel2 = src_row + size - 1;
    parent_pixel3 = src_row;
    evolve_pixel_3_parent_genes(dst_row + size - 1, parent_pixel1,
                                parent_pixel2, parent_pixel3);
}

void evolve_pixel_dad_mom_average(Pixel *dst_pixel, const Pixel *dad_pixel,
                                  const Pixel *mom_pixel) {
    if (rand() % 1024) {
        dst_pixel->r = jitter((dad_pixel->r + mom_pixel->r) / 2);
        dst_pixel->g = jitter((dad_pixel->g + mom_pixel->g) / 2);
        dst_pixel->b = jitter((dad_pixel->b + mom_pixel->b) / 2);
    } else {
        dst_pixel->r = rand() % 2 ? 255 : 0;
        dst_pixel->g = rand() % 2 ? 255 : 0;
        dst_pixel->b = rand() % 2 ? 255 : 0;
    }
}

void evolve_row_dad_mom_average(Pixel *dst_row, const Pixel *src_row,
                                const size_t size) {
    size_t i;
    const Pixel *dad_pixel, *mom_pixel;

    /* Evolve first pixel. */
    dad_pixel = src_row + size - 1;
    mom_pixel = src_row + 1;
    evolve_pixel_dad_mom_average(dst_row, dad_pixel, mom_pixel);

    /* Evolve middle pixels (all except first and last). */
    for (i = 1; i < size - 1; ++i) {
        dad_pixel = src_row + i - 1;
        mom_pixel = src_row + i + 1;
        evolve_pixel_dad_mom_average(dst_row + i, dad_pixel, mom_pixel);
    }

    /* Evolve last pixel. */
    dad_pixel = src_row + size - 2;
    mom_pixel = src_row;
    evolve_pixel_dad_mom_average(dst_row + size - 1, dad_pixel, mom_pixel);
}

void evolve_row_dad_mom_dad_above(Pixel *dst_row, const Pixel *src_row,
                                  const size_t size) {
    size_t i;
    const Pixel *dad_pixel, *mom_pixel;

    /* Evolve all except last). */
    for (i = 0; i < size - 1; ++i) {
        dad_pixel = src_row + i;
        mom_pixel = src_row + i + 1;
        evolve_pixel_dad_mom_average(dst_row + i, dad_pixel, mom_pixel);
    }

    /* Evolve last pixel. */
    dad_pixel = src_row + size - 1;
    mom_pixel = src_row;
    evolve_pixel_dad_mom_average(dst_row + size - 1, dad_pixel, mom_pixel);
}

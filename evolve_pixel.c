#include <assert.h>
#include <limits.h>
#include "evolve_pixel.h"

size_t wrap(size_t position, int offset, size_t size) {
    /*
     * Want to be sure that position + offset do not underflow or overflow.
     *
     * 0 < position and INT_MIN < offset by definition, so
     *     INT_MIN < position + offset,
     * that is position + offset cannot underflow.
     *
     * Now if we check that position < size < INT_MAX, position + offset can
     * still overflow, but can't overflow more than once, since both
     *     position < INT_MAX and offset < INT_MAX.
     * So if we pass the overflow check on position + offset, we are good.
     */
    int position_plus_offset, result;
    int size_int, position_int;

    assert(size < INT_MAX);
    assert(position < size);

    size_int = (int)size;
    position_int = (int)position;

    assert(INT_MAX - position_int >= offset);

    position_plus_offset = position_int + offset;
    if (position_plus_offset < 0) {
        result = position_plus_offset % size_int + size_int;
    } else {
        result = position_plus_offset % size_int;
    }

    assert(0 <= result);
    assert(result < size_int);

    return (size_t)result;
}

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


void evolve_pixel_dad_or_mom(Pixel *dst_pixel, const Pixel *dad_pixel,
                             const Pixel *mom_pixel) {

    const Pixel *chosen_parent = (rand() % 2) ? dad_pixel : mom_pixel;
    *dst_pixel = *chosen_parent;
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

void evolve_pixel_dad_mom_average(Pixel *dst_pixel, const Pixel *dad_pixel,
                                  const Pixel *mom_pixel) {
    dst_pixel->r = dad_pixel->r / 2 + mom_pixel->r / 2 + rand() % 17 - 8;
    dst_pixel->g = dad_pixel->g / 2 + mom_pixel->g / 2 + rand() % 17 - 8;
    dst_pixel->b = dad_pixel->b / 2 + mom_pixel->b / 2 + rand() % 17 - 8;
}

void evolve_pixel_4_parent_average(Pixel *dst_pixel,
                                   const Pixel *parent_pixel1,
                                   const Pixel *parent_pixel2,
                                   const Pixel *parent_pixel3,
                                   const Pixel *parent_pixel4) {
    dst_pixel->r = (int)(parent_pixel1->r / 4.0) +
                   (int)(parent_pixel2->r / 4.0) +
                   (int)(parent_pixel3->r / 4.0) +
                   (int)(parent_pixel4->r / 4.0) + rand() % 17 - 8;

    dst_pixel->g = (int)(parent_pixel1->g / 4.0) +
                   (int)(parent_pixel2->g / 4.0) +
                   (int)(parent_pixel3->g / 4.0) +
                   (int)(parent_pixel4->g / 4.0) + rand() % 17 - 8;

    dst_pixel->b = (int)(parent_pixel1->b / 4.0) +
                   (int)(parent_pixel2->b / 4.0) +
                   (int)(parent_pixel3->b / 4.0) +
                   (int)(parent_pixel4->b / 4.0) + rand() % 17 - 8;
}

void evolve_pixel_4_parent_genes(Pixel *dst_pixel, const Pixel *parent_pixel1,
                                 const Pixel *parent_pixel2,
                                 const Pixel *parent_pixel3,
                                 const Pixel *parent_pixel4) {
    int rand_r = rand() % 4;
    int rand_g = rand() % 4;
    int rand_b = rand() % 4;

    if (rand_r == 0) {
        dst_pixel->r = jitter(parent_pixel1->r);
    } else if (rand_r == 1) {
        dst_pixel->r = jitter(parent_pixel2->r);
    } else if (rand_r == 2) {
        dst_pixel->r = jitter(parent_pixel3->r);
    } else {
        dst_pixel->r = jitter(parent_pixel4->r);
    }
    if (rand_g == 0) {
        dst_pixel->g = jitter(parent_pixel1->g);
    } else if (rand_g == 1) {
        dst_pixel->g = jitter(parent_pixel2->g);
    } else if (rand_g == 2) {
        dst_pixel->g = jitter(parent_pixel3->g);
    } else {
        dst_pixel->g = jitter(parent_pixel4->g);
    }
    if (rand_b == 0) {
        dst_pixel->b = jitter(parent_pixel1->b);
    } else if (rand_b == 1) {
        dst_pixel->b = jitter(parent_pixel2->b);
    } else if (rand_b == 2) {
        dst_pixel->b = jitter(parent_pixel3->b);
    } else {
        dst_pixel->b = jitter(parent_pixel4->b);
    }
}

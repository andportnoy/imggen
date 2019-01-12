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
    dst_pixel->r = rand() % 2 ? jitter(dad_pixel->r) : jitter(mom_pixel->r);
    dst_pixel->g = rand() % 2 ? jitter(dad_pixel->g) : jitter(mom_pixel->g);
    dst_pixel->b = rand() % 2 ? jitter(dad_pixel->b) : jitter(mom_pixel->b);
}


void evolve_pixel_dad_or_mom(Pixel *dst_pixel, const Pixel *dad_pixel,
                             const Pixel *mom_pixel) {

    *dst_pixel = *(rand() % 2 ? dad_pixel : mom_pixel);
}



void evolve_pixel_3_parent_genes(Pixel *dst_pixel, const Pixel *parent_pixel1,
                                 const Pixel *parent_pixel2,
                                 const Pixel *parent_pixel3) {
    switch(rand() % 3) {
        case 0: dst_pixel->r = jitter(parent_pixel1->r); break;
        case 1: dst_pixel->r = jitter(parent_pixel2->r); break;
        case 2: dst_pixel->r = jitter(parent_pixel3->r); break;
    }
    switch(rand() % 3) {
        case 0: dst_pixel->g = jitter(parent_pixel1->g); break;
        case 1: dst_pixel->g = jitter(parent_pixel2->g); break;
        case 2: dst_pixel->g = jitter(parent_pixel3->g); break;
    }
    switch(rand() % 3) {
        case 0: dst_pixel->b = jitter(parent_pixel1->b); break;
        case 1: dst_pixel->b = jitter(parent_pixel2->b); break;
        case 2: dst_pixel->b = jitter(parent_pixel3->b); break;
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
    switch(rand() % 4) {
        case 0: dst_pixel->r = jitter(parent_pixel1->r); break;
        case 1: dst_pixel->r = jitter(parent_pixel2->r); break;
        case 2: dst_pixel->r = jitter(parent_pixel3->r); break;
        case 3: dst_pixel->r = jitter(parent_pixel4->r); break;
    }
    switch(rand() % 4) {
        case 0: dst_pixel->g = jitter(parent_pixel1->g); break;
        case 1: dst_pixel->g = jitter(parent_pixel2->g); break;
        case 2: dst_pixel->g = jitter(parent_pixel3->g); break;
        case 3: dst_pixel->g = jitter(parent_pixel4->g); break;
    }
    switch(rand() % 4) {
        case 0: dst_pixel->b = jitter(parent_pixel1->b); break;
        case 1: dst_pixel->b = jitter(parent_pixel2->b); break;
        case 2: dst_pixel->b = jitter(parent_pixel3->b); break;
        case 3: dst_pixel->b = jitter(parent_pixel4->b); break;
    }
}

void evolve_pixel_4_parent_pick_one(Pixel *dst_pixel,
                                    const Pixel *parent_pixel1,
                                    const Pixel *parent_pixel2,
                                    const Pixel *parent_pixel3,
                                    const Pixel *parent_pixel4) {
    switch(rand() % 4) {
        case 0: *dst_pixel = *parent_pixel1; break;
        case 1: *dst_pixel = *parent_pixel2; break;
        case 2: *dst_pixel = *parent_pixel3; break;
        case 3: *dst_pixel = *parent_pixel4; break;
    }
}

void evolve_pixel_8_parent_pick_one(Pixel *dst_pixel,
                                    const Pixel *parent_pixel1,
                                    const Pixel *parent_pixel2,
                                    const Pixel *parent_pixel3,
                                    const Pixel *parent_pixel4,
                                    const Pixel *parent_pixel5,
                                    const Pixel *parent_pixel6,
                                    const Pixel *parent_pixel7,
                                    const Pixel *parent_pixel8) {
    switch(rand() % 8) {
        case 0: *dst_pixel = *parent_pixel1; break;
        case 1: *dst_pixel = *parent_pixel2; break;
        case 2: *dst_pixel = *parent_pixel3; break;
        case 3: *dst_pixel = *parent_pixel4; break;
        case 4: *dst_pixel = *parent_pixel5; break;
        case 5: *dst_pixel = *parent_pixel6; break;
        case 6: *dst_pixel = *parent_pixel7; break;
        case 7: *dst_pixel = *parent_pixel8; break;
    }
}


unsigned char extremity(const Pixel *pixel, unsigned char rgb[3]) {

    unsigned char tmp;
    rgb[0] = pixel->r;
    rgb[1] = pixel->g;
    rgb[2] = pixel->b;

    if (rgb[2] < rgb[1]) {
        tmp = rgb[1];
        rgb[1] = rgb[2];
        rgb[2] = tmp;
    }

    if (rgb[1] < rgb[0]) {
        tmp = rgb[0];
        rgb[0] = rgb[1];
        rgb[1] = tmp;
    }

    /* Now rgb[0] is the minimum */

    return rgb[1] < rgb[2] ? rgb[2] - rgb[1] : rgb[1] - rgb[2];

}

Pixel *most_extreme(Pixel **pixels, size_t n_pixels) {

    unsigned char best_extremity, cur_extremity;
    size_t cur_id, best_id;
    /* Used for sorting in extremity */
    unsigned char rgb[3];

    best_id = cur_id = 0;
    best_extremity = extremity(pixels[cur_id], rgb);
    for (cur_id = 1; cur_id < n_pixels; ++cur_id) {
        cur_extremity = extremity(pixels[cur_id], rgb);
        if (cur_extremity > best_extremity) {
            best_id = cur_id;
            best_extremity = cur_extremity;
        }
    }
    return pixels[best_id];
}

void evolve_pixel_8_parent_extreme(Pixel *dst_pixel, Pixel *parents[8]) {
    *dst_pixel = *(most_extreme(parents, 8));
    if (rand() % 10 == 0) {
        dst_pixel->r = jitter(dst_pixel->r);
        dst_pixel->g = jitter(dst_pixel->g);
        dst_pixel->b = jitter(dst_pixel->b);
    }
}

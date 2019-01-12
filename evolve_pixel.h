#ifndef EVOLVE_PIXEL_H
#define EVOLVE_PIXEL_H
#include "image.h"

/**
 * Wrap position + offset to new_position with [0, size-1].
 * Fails if position + offset exceed MAX_INT.
 */
size_t wrap(size_t position, int offset, size_t size);

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
 * Evolve pixel by randomly copying mom or dad.
 */
void evolve_pixel_dad_or_mom(Pixel *dst_pixel, const Pixel *dad_pixel,
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

void evolve_pixel_4_parent_genes(Pixel *dst_pixel, const Pixel *parent_pixel1,
                                 const Pixel *parent_pixel2,
                                 const Pixel *parent_pixel3,
                                 const Pixel *parent_pixel4);

void evolve_pixel_4_parent_average(Pixel *dst_pixel,
                                   const Pixel *parent_pixel1,
                                   const Pixel *parent_pixel2,
                                   const Pixel *parent_pixel3,
                                   const Pixel *parent_pixel4);

void evolve_pixel_4_parent_pick_one(Pixel *dst_pixel,
                                    const Pixel *parent_pixel1,
                                    const Pixel *parent_pixel2,
                                    const Pixel *parent_pixel3,
                                    const Pixel *parent_pixel4);

void evolve_pixel_8_parent_pick_one(Pixel *dst_pixel,
                                    const Pixel *parent_pixel1,
                                    const Pixel *parent_pixel2,
                                    const Pixel *parent_pixel3,
                                    const Pixel *parent_pixel4,
                                    const Pixel *parent_pixel5,
                                    const Pixel *parent_pixel6,
                                    const Pixel *parent_pixel7,
                                    const Pixel *parent_pixel8);

unsigned char extremity(const Pixel *pixel, unsigned char rgb[3]);

/**
 * Among the 8 parents, pick the one with the following maximized:
 *
 *         max(r, g, b) - max2(r, g, b),
 *
 * where max2 means second greatest value.
 */

Pixel *most_extreme(Pixel **pixels, size_t n_pixels);
void evolve_pixel_8_parent_extreme(Pixel *dst_pixel, Pixel *parents[8]);

void evolve_pixel_3_parent_bright(Pixel *dst_pixel,
                                  const Pixel *parent_pixel1,
                                  const Pixel *parent_pixel2,
                                  const Pixel *parent_pixel3);

#endif /* EVOLVE_PIXEL_H */

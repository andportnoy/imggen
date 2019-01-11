#ifndef EVOLVE_H
#define EVOLVE_H
#include "image.h"
/**
 * Evolve row dst_row based on src_row.
 */
void evolve_row_single_parent(Pixel *dst_row, const Pixel *src_row,
                              const size_t size);

/**
 * Evolve row dst_row based on src_row using the mom + dad genetic approach.
 */
void evolve_row_dad_mom_genes(Pixel *dst_row, const Pixel *src_row,
                              const size_t size);

/**
 * Evolve row dst_row based on src_row using the mom + dad random choice
 * approach.
 */
void evolve_row_dad_or_mom(Pixel *dst_row, const Pixel *src_row,
                           const size_t size);

/**
 * Evolve row dst_row based on src_row using the three parent approach.
 */
void evolve_row_3_parent_genes(Pixel *dst_row, const Pixel *src_row,
                               const size_t size);

/**
 * Evolve row dst_row based on src_row using the mom + dad averaged approach.
 */
void evolve_row_dad_mom_average(Pixel *dst_row, const Pixel *src_row,
                                const size_t size);

/**
 * Evolve row dst_row based on src_row using the mom + dad averaged approach,
 * with dad now above.
 */
void evolve_row_dad_mom_dad_above(Pixel *dst_row, const Pixel *src_row,
                                  const size_t size);

/**
 * Evolve image dst_image based on src_image, using pixels up, down, left,
 * and right, genetic approach.
 */
void evolve_image_4_parent_genes(Image *dst_image, const Image *src_image);

/**
 * Evolve image dst_image based on src_image, using pixels up, down, left,
 * and right, averaging approach.
 */
void evolve_image_4_parent_average(Image *dst_image, const Image *src_image);

#endif /* EVOLVE_H */

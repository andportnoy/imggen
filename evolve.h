#ifndef EVOLVE_H
#define EVOLVE_H
#include "image.h"
/**
 * Evolve row dst_row based on src_row.
 */
void evolve_row_single_parent(Pixel *dst_row, const Pixel *src_row,
                              const size_t size);

/**
 * Evolve row dst_row based on src_row using the mom + dad approach.
 */
void evolve_row_dad_mom_genes(Pixel *dst_row, const Pixel *src_row,
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

#endif /* EVOLVE_H */

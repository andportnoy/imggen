#ifndef EVOLVE_ROW_H
#define EVOLVE_ROW_H
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
 * Function pointer for a row evolver.
 */
typedef void (*Row_evolver)(Pixel *, const Pixel *, size_t);

/**
 * Generate image of required width and height in pixels using the supplied
 * row_evolver (function pointer).
 * Caller responsible for freeing image memory.
 */
Image *generate_image(size_t width, size_t height, Row_evolver row_evolver);

/**
 * Command line interface to image generation.
 */
void main_row_generation(int argc, char *argv[]);

#endif /* EVOLVE_ROW_H */

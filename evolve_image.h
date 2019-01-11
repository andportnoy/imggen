#ifndef EVOLVE_IMAGE_H
#define EVOLVE_IMAGE_H
#include "image.h"

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

void evolve_image_4_parent_pick_one(Image *dst_image, const Image *src_image);

void evolve_image_8_parent_pick_one(Image *dst_image, const Image *src_image);

Image **generate_images(size_t n_images, size_t width, size_t height);

void write_images(Image **images, size_t n_images);

void free_images(Image **images, size_t n_images);

void main_image_generation(size_t n_images, size_t width, size_t height);

#endif /* EVOLVE_IMAGE_H */

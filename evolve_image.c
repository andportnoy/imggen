#include <assert.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "image.h"
#include "evolve_pixel.h"
#include "evolve_image.h"

#define MAX_FILENAME_LENGTH 100

void evolve_image_4_parent_genes(Image *dst_image, const Image *src_image) {
    size_t i, j;
    size_t width, height;
    assert(dst_image->width == src_image->width);
    assert(dst_image->height == src_image->height);
    width = dst_image->width;
    height = dst_image->height;

    /* Need to evolve every pixel of dst_image based on src_image. */
    for (j = 0; j < height; ++j) {
        for (i = 0; i < width; ++i) {
            evolve_pixel_4_parent_genes(
                /* destination pixel */
                pixel_at(dst_image, i, j),
                /* source pixel below */
                pixel_at(src_image, i, wrap(j, -1, height)),
                /* source pixel above */
                pixel_at(src_image, i, wrap(j, 1, height)),
                /* source pixel left */
                pixel_at(src_image, wrap(i, -1, width), j),
                /* source pixel right */
                pixel_at(src_image, wrap(i, 1, width), j));
        }
    }
}

void evolve_image_4_parent_average(Image *dst_image, const Image *src_image) {
    size_t i, j;
    size_t width, height;
    assert(dst_image->width == src_image->width);
    assert(dst_image->height == src_image->height);
    width = dst_image->width;
    height = dst_image->height;

    /* Need to evolve every pixel of dst_image based on src_image. */
    for (j = 0; j < height; ++j) {
        for (i = 0; i < width; ++i) {
            evolve_pixel_4_parent_average(
                /* destination pixel */
                pixel_at(dst_image, i, j),
                /* source pixel below */
                pixel_at(src_image, i, wrap(j, -1, height)),
                /* source pixel above */
                pixel_at(src_image, i, wrap(j, 1, height)),
                /* source pixel left */
                pixel_at(src_image, wrap(i, -1, width), j),
                /* source pixel right */
                pixel_at(src_image, wrap(i, 1, width), j));
        }
    }
}

Image **generate_images(size_t n_images, size_t width, size_t height) {
    Image **images;
    size_t i;

    images = malloc(n_images * sizeof(*images));

    images[0] = malloc_random_image(width, height);
    for (i = 1; i < n_images; ++i) {
        images[i] = malloc_image(width, height);
        evolve_image_4_parent_genes(images[i], images[i-1]);
        printf("\33[2K\rGenerated image %lu...", i);
        fflush(stdout);
    }
    printf("\33[2K\rDone generating.\n");
    fflush(stdout);
    return images;
}

void write_images(Image **images, size_t n_images) {
    size_t i;

    for (i = 0; i < n_images; ++i) {
        char filename[MAX_FILENAME_LENGTH];
        FILE *file;
        sprintf(filename, "images/random%lu.ppm", i);
        file = fopen(filename, "w");
        if (file) {
            write_image(file, images[i]);
            fclose(file);
            printf("\33[2K\rSaved image %lu...", i);
            fflush(stdout);
        } else {
            fprintf(stderr, "Failed to open file %s\n", filename);
            exit(1);
        }
    }
    printf("\33[2K\rDone saving.\n");
}

void free_images(Image **images, size_t n_images) {
    size_t i;
    for (i = 0; i < n_images; ++i) {
        free_image(images[i]);
    }
    free(images);
}

void main_image_generation(size_t n_images, size_t width, size_t height) {
    Image **images = generate_images(n_images, width, height);
    write_images(images, n_images);
    free_images(images, n_images);
}

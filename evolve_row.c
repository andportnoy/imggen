#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "evolve_pixel.h"
#include "evolve_row.h"
#include "image.h"

void evolve_row_single_parent(Pixel *dst_row, const Pixel *src_row,
                              const size_t size) {
    size_t i;
    for (i = 0; i < size; ++i) {
        evolve_pixel_single_parent(dst_row + i, src_row + i);
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

void evolve_row_dad_or_mom(Pixel *dst_row, const Pixel *src_row,
                           const size_t size) {
    size_t i;
    const Pixel *dad_pixel, *mom_pixel;

    /* Evolve first pixel. */
    dad_pixel = src_row + size - 1;
    mom_pixel = src_row + 1;
    evolve_pixel_dad_or_mom(dst_row, dad_pixel, mom_pixel);

    /* Evolve middle pixels (all except first and last). */
    for (i = 1; i < size - 1; ++i) {
        dad_pixel = src_row + i - 1;
        mom_pixel = src_row + i + 1;
        evolve_pixel_dad_or_mom(dst_row + i, dad_pixel, mom_pixel);
    }

    /* Evolve last pixel. */
    dad_pixel = src_row + size - 2;
    mom_pixel = src_row;
    evolve_pixel_dad_or_mom(dst_row + size - 1, dad_pixel, mom_pixel);
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

Image *generate_image(size_t width, size_t height, Row_evolver row_evolver) {
    size_t j;
    Image *image;

    image = malloc_image(width, height);

    assert(image->width == width);
    assert(image->height == height);

    if (image) {
        const Pixel *src_row;
        Pixel *dst_row;

        /* Set the first row to random RGB values. */
        set_random_row(image->pixels, image->width);

        /*
         * Evolve all other rows iteratively, starting with second row based on
         * the first row.
         */
        for (j = 1; j < image->height; ++j) {
            dst_row = image->pixels + j * image->width;
            src_row = image->pixels + (j - 1) * image->width;
            (*row_evolver)(dst_row, src_row, image->width);
        }
        return image;
    } else {
        fprintf(stderr, "Failed to allocate image.\n");
        exit(1);
    }
}

void main_row_generation(int argc, char *argv[]) {
    unsigned long width, height;
    int strategy;
    Row_evolver chosen_row_evolver;
    Image *image;
    FILE *file;
    Row_evolver row_evolvers[6] = {
        &evolve_row_single_parent,   &evolve_row_dad_mom_genes,
        &evolve_row_dad_or_mom,      &evolve_row_3_parent_genes,
        &evolve_row_dad_mom_average, &evolve_row_dad_mom_dad_above};

    if (argc != 5) {
        fprintf(stderr,
                "Got %d arguments, need 4: width, height, strategy index, file"
                " name.\n",
                argc - 1);
        exit(1);
    }
    if (1 != sscanf(argv[1], "%lu", &width)) {
        fprintf(stderr, "Enter width as a positive integer.\n");
        exit(1);
    }
    if (1 != sscanf(argv[2], "%lu", &height)) {
        fprintf(stderr, "Enter height as a positive integer.\n");
        exit(1);
    }
    strategy = atoi(argv[3]);

    if (1 <= strategy && strategy <= 6) {
        chosen_row_evolver = row_evolvers[strategy - 1];
    } else {
        fprintf(stderr, "You entered %d, which is invalid.\n", strategy);
        fprintf(stderr, "Available strategies include:\n");
        fprintf(stderr, "\t1. evolve_row_single_parent\n");
        fprintf(stderr, "\t2. evolve_row_dad_mom_genes\n");
        fprintf(stderr, "\t3. evolve_row_dad_or_mom\n");
        fprintf(stderr, "\t4. evolve_row_3_parent_genes\n");
        fprintf(stderr, "\t5. evolve_row_dad_mom_average\n");
        fprintf(stderr, "\t6. evolve_row_dad_mom_dad_above\n");
        exit(1);
    }

    srand((unsigned int)time(NULL));
    image = generate_image((size_t)width, (size_t)height, chosen_row_evolver);
    file = fopen(argv[4], "w");
    if (file) {
        write_image(file, image);
        fclose(file);
    } else {
        fprintf(stderr, "Failed to open file.\n");
        exit(1);
    }
    free_image(image);
}

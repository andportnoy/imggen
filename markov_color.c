#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "evolve.h"
#include "image.h"

/**
 * A couple of interesting things I want to investigate.
 * 1. What happens if we randomly create top row of pixels, then every pixel
 * below is pixel above summed with random increment of random sign?
 * 2. What if we start in the center?
 * 3. Is unsigned char the best type for RGB stuff? Yes!
 * 4. What if evolve only a randomly chosen component of RGB?
 * 5. RGB space is a cube. We can send vectors in brownian motion inside that
 *    box. But colors pleasant to the eye may form a boxy connected subset of
 *    RGB space. Can we send a vector in brownian motion inside such a subset?
 * 6. Can we make GIFs by evolving not rows but whole images? So a pixel has 8
 *    parents, a kernel?
 * 7. Can we try growing trees starting from the center?
 */

/**
 * Function pointer for a row evolver.
 */
typedef void (*Row_evolver)(Pixel *, const Pixel *, size_t);

/**
 * Generate and print image of required width and height in pixels using the
 * supplied row_evolver (function pointer).
 * Handles allocation and freeing of required memory.
 */
Image *generate_image(size_t width, size_t height, Row_evolver row_evolver);

Image *generate_image(size_t width, size_t height, Row_evolver row_evolver) {
    size_t j;
    Image *image;

    image = make_image(width, height);

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

int main(int argc, char *argv[]) {
    unsigned long width, height;
    int strategy;
    Row_evolver chosen_row_evolver;
    Image *image;
    Row_evolver row_evolvers[5] = {
        &evolve_row_single_parent,
        &evolve_row_dad_mom_genes,
        &evolve_row_3_parent_genes,
        &evolve_row_dad_mom_average,
        &evolve_row_dad_mom_dad_above
    };

    if (argc != 4) {
        fprintf(stderr,
                "Got %d arguments, need 3: width, height, strategy index.\n",
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

    if (1 <= strategy && strategy <= 5) {
        chosen_row_evolver = row_evolvers[strategy - 1];
    } else {
        fprintf(stderr, "You entered %d, which is invalid.\n", strategy);
        fprintf(stderr, "Available strategies include:\n");
        fprintf(stderr, "\t1. evolve_row_single_parent\n");
        fprintf(stderr, "\t2. evolve_row_dad_mom_genes\n");
        fprintf(stderr, "\t3. evolve_row_3_parent_genes\n");
        fprintf(stderr, "\t4. evolve_row_dad_mom_average\n");
        fprintf(stderr, "\t5. evolve_row_dad_mom_dad_above\n");
        exit(1);
    }

    srand((unsigned int)time(NULL));
    image = generate_image((size_t)width, (size_t)height, chosen_row_evolver);
    print_image(image);
    free(image);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
void generate_image(size_t width, size_t height, Row_evolver row_evolver);

void generate_image(size_t width, size_t height, Row_evolver row_evolver) {
    size_t j;
    Pixel *image = make_image(width, height);

    if (image) {
        set_random_row(image, width);
        for (j = 1; j < height; ++j) {
            Pixel *dst_row = image + j * width;
            Pixel *src_row = image + (j - 1) * width;
            (*row_evolver)(dst_row, src_row, width);
        }
        print_image(image, width, height);
        free(image);
    } else {
        fprintf(stderr, "Failed to allocate.\n");
        exit(1);
    }
}

int main() {
    srand((unsigned int)time(NULL));
    generate_image(2880, 1800, &evolve_row_dad_mom_genes);
}

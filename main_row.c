#include "evolve_row.h"

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

int main(int argc, char *argv[]) {
    main_row_generation(argc, argv);
    return 0;
}

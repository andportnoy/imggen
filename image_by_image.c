#include <assert.h>
#include <stdio.h>
#include "image.h"

#define N_IMAGES 100
#define WIDTH 1000
#define HEIGHT 1000
#define FILENAME_LENGTH 100

void write_images(Image **images, size_t n);

void write_images(Image **images, size_t n) {
    size_t i;

    for (i = 0; i < n; ++i) {
        char filename[FILENAME_LENGTH];
        FILE *file;
        sprintf(filename,  "images/random%lu.ppm", i);
        file = fopen(filename, "w");
        if (file) {
            write_image(file, images[i]);
            fclose(file);
        } else {
            fprintf(stderr, "Failed to open file %s\n", filename);
            exit(1);
        }
    }
}

int main() {
    Image *images[N_IMAGES];
    int i;

    for (i = 0; i < N_IMAGES; ++i) {
        images[i] = make_random_image(WIDTH, HEIGHT);
    }

    write_images(images, N_IMAGES);
}

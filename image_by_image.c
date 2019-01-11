#include "image.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "evolve.h"

#define N_IMAGES 200
#define WIDTH 200
#define HEIGHT 200
#define MAX_FILENAME_LENGTH 100

Image **generate_images(void);
void write_images(Image **images, int n);
void free_images(Image **images, int n);

Image **generate_images() {
    Image **images;
    int i;

    images = malloc(N_IMAGES * sizeof(*images));

    images[0] = malloc_random_image(WIDTH, HEIGHT);
    for (i = 1; i < N_IMAGES; ++i) {
        images[i] = malloc_image(WIDTH, HEIGHT);
        evolve_image_4_parent_genes(images[i], images[i-1]);
        printf("\33[2K\rGenerated image %d...", i);
        fflush(stdout);
    }
    printf("\33[2K\rDone generating.\n");
    fflush(stdout);
    return images;
}

void write_images(Image **images, int n) {
    int i;

    for (i = 0; i < n; ++i) {
        char filename[MAX_FILENAME_LENGTH];
        FILE *file;
        sprintf(filename, "images/random%d.ppm", i);
        file = fopen(filename, "w");
        if (file) {
            write_image(file, images[i]);
            fclose(file);
            printf("\33[2K\rSaved image %d...", i);
            fflush(stdout);
        } else {
            fprintf(stderr, "Failed to open file %s\n", filename);
            exit(1);
        }
    }
    printf("\33[2K\rDone saving.\n");
}

void free_images(Image **images, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        free_image(images[i]);
    }
    free(images);
}

int main() {
    Image **images = generate_images();
    write_images(images, N_IMAGES);
    free_images(images, N_IMAGES);
}

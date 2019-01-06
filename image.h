#ifndef IMAGE_H
#define IMAGE_H
#include <stdlib.h>

typedef struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

void set_random_pixel(Pixel *pixel);
void print_pixel(Pixel *pixel);
void set_random_row(Pixel *row, size_t size);
void print_image(Pixel *image, size_t width, size_t height);
void set_random_image(Pixel *image, size_t width, size_t height);
Pixel *make_image(size_t width, size_t height);
Pixel *make_random_image(size_t width, size_t height);
#endif /* IMAGE_H */

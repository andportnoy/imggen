#ifndef IMAGE_H
#define IMAGE_H
#include <stdlib.h>
#include <stdio.h>

typedef struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

typedef struct Image {
    Pixel *pixels;
    size_t width;
    size_t height;
} Image;

void set_random_pixel(Pixel *pixel);
void print_pixel(const Pixel *pixel);
void write_pixel(FILE *file, const Pixel *pixel);
void set_random_row(Pixel *row, size_t width);
void print_image(const Image *image);
void write_image(FILE *file, const Image *image);
void set_random_image(Image *image);
Image *make_image(size_t width, size_t height);
Image *make_random_image(size_t width, size_t height);
#endif /* IMAGE_H */

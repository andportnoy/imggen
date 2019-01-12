#ifndef IMAGE_H
#define IMAGE_H
#include <stdio.h>
#include <stdlib.h>

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
Pixel *pixel_at(const Image *image, size_t x, size_t y);
void set_random_row(Pixel *row, size_t width);
void print_image(const Image *image);
void write_image_P3(FILE *file, const Image *image);
void write_image_P6(FILE *file, const Image *image);
void set_random_image(Image *image);
Image *malloc_image(size_t width, size_t height);
Image *malloc_random_image(size_t width, size_t height);
void free_image(Image *image);
#endif /* IMAGE_H */

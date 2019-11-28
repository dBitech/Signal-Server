#ifndef _IMAGE_PPM_HH
#define _IMAGE_PPM_HH

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "image.hh"

int ppm_init(image_ctx_t *ctx);
int ppm_add_pixel(image_ctx_t *ctx, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
int ppm_get_pixel(image_ctx_t *ctx, size_t x, size_t y, const uint8_t *r, const uint8_t *g, const uint8_t *b, const uint8_t *a);
int ppm_write(image_ctx_t *ctx, FILE *fd);

image_dispatch_table_t ppm_dt = {.init = ppm_init,
                                 .add_pixel = ppm_add_pixel,
                                 .set_pixel = nullptr,
                                 .get_pixel = ppm_get_pixel,
                                 .write = ppm_write,
                                 .free = nullptr};

#endif
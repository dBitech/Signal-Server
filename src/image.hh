#ifndef _IMAGE_HH_
#define _IMAGE_HH_

#include <cstddef>
#include <cstdint>
#include <cstdio>

#define RGB_SIZE 3

enum _image_format { IMAGE_DEFAULT = 0, IMAGE_PPM, IMAGE_LIBRARY, IMAGE_FORMAT_MAX };

enum _image_model { IMAGE_RGB, IMAGE_MODEL_MAX };

typedef struct _image_ctx {
  std::size_t width;
  std::size_t height;
  int model;
  int format;
  uint8_t *canvas;
  uint8_t *next_pixel;
  uint32_t initialized;
  char *extension;
  void *_dt;
} image_ctx_t;

typedef int _init(image_ctx_t *);
typedef int _add_pixel(image_ctx_t *, const uint8_t, const uint8_t, const uint8_t, const uint8_t);
typedef int _set_pixel(image_ctx_t *, const std::size_t, const std::size_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t);
typedef int _get_pixel(image_ctx_t *, const std::size_t, const std::size_t, const uint8_t *, const uint8_t *, const uint8_t *,
                       const uint8_t *);
typedef int _write(image_ctx_t *, FILE *);
typedef void _free(image_ctx_t *);

typedef struct _image_dispatch_table {
  _init *init;
  _add_pixel *add_pixel;
  _set_pixel *set_pixel;
  _get_pixel *get_pixel;
  _write *write;
  _free *free;
} image_dispatch_table_t;

int image_init(image_ctx_t *, std::size_t, std::size_t, int, int);
int image_add_pixel(image_ctx_t *ctx, uint8_t, uint8_t, uint8_t, uint8_t);
int image_get_filename(image_ctx_t *, char *, std::size_t, char *);
int image_write(image_ctx_t *, FILE *);
void image_free(image_ctx_t *);
int image_set_library(char *);

#define ADD_PIXEL(ctx, r, g, b) image_add_pixel((ctx), (r), (g), (b), 0xff)

#endif
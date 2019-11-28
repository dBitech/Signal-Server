/*
 * Generic image output handling. This feature allows
 * for extensible image output formats and permits for
 * cleaner image rendering code in the model generation
 * routines by moving all of the file-format specific
 * logic to be handled here.
 */
#include "image.hh"

#include <dlfcn.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

extern image_dispatch_table_t ppm_dt;  // In image-ppm.{cc,hh}

int get_dt(image_dispatch_table_t *dt, int format);
int load_library(image_dispatch_table_t *dt);

#define DISPATCH_TABLE(ctx) ((image_dispatch_table_t *)(ctx)->_dt)

static int default_format = IMAGE_PPM;
char *dynamic_backend = nullptr;

/*
 * image_init
 * Initialize an image context. Must be called
 * before attempting to write any image data
 */
int image_init(image_ctx_t *ctx, const std::size_t width, const std::size_t height, const int model, const int format) {
  int success = 0;
  image_dispatch_table_t *dt;

  /* Perform some sanity checking on provided arguments */
  if (ctx == nullptr) return EINVAL;
  if (width == 0 || height == 0) return EINVAL;
  if (model < 0 || model > IMAGE_MODEL_MAX) return EINVAL;
  if (format >= IMAGE_FORMAT_MAX || (format == IMAGE_LIBRARY && dynamic_backend == nullptr)) return EINVAL;

  memset(ctx, 0x00, sizeof(image_ctx_t));

  /* Assign the initialize values to the processing context */
  ctx->width = width;
  ctx->height = height;
  ctx->model = model;
  if (format == IMAGE_DEFAULT)
    ctx->format = default_format;
  else
    ctx->format = format;

  /* Get the dispatch table for this image format */
  dt = (image_dispatch_table_t *)calloc(1, sizeof(image_dispatch_table_t));
  if (dt == nullptr) {
    fprintf(stderr, "Error allocating dispatch table\n");
    return ENOMEM;
  }
  success = get_dt(dt, ctx->format);
  if (success != 0) {
    fprintf(stderr, "Error locating dispatch table\n");
    free(dt);
    return success;
  }
  ctx->_dt = (void *)dt;

  /* Call the format-specific initialization function */
  success = dt->init(ctx);
  if (success != 0) {
    fprintf(stderr, "Error initializing image context\n");
    free(dt);
    return success;
  }

  ctx->initialized = 1;

  return success;
}
/*
 * image_add_pixel, image_write, image_free
 * Various setters ad getters for assigning pixel data. image_write
 * takes an open file handle and writes image data to it.
 * These functions simply wrap the underlying format-specific functions
 */
int image_add_pixel(image_ctx_t *ctx, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
  if (ctx->initialized != 1) return EINVAL;
  return DISPATCH_TABLE(ctx)->add_pixel(ctx, r, g, b, a);
}
int image_write(image_ctx_t *ctx, FILE *fd) {
  if (ctx->initialized != 1) return EINVAL;
  return DISPATCH_TABLE(ctx)->write(ctx, fd);
}
void image_free(image_ctx_t *ctx) {
  if (ctx->initialized != 1) return;
  if (DISPATCH_TABLE(ctx)->free != nullptr) {
    DISPATCH_TABLE(ctx)->free(ctx);
  }
  if (ctx->canvas != nullptr) free(ctx->canvas);
}

/*
 * image_get_filename
 * Creates an appropriate file name using data supplied
 * by the user. If the extension is already correct, return
 * that; if not append if there is space
 */
int image_get_filename(image_ctx_t *ctx, char *out, std::size_t len_out, char *in) {
  std::size_t len_src;
  std::size_t len_ext;
  int success = 0;

  if (ctx->initialized != 1) return EINVAL;

  /* Get various lengths */
  len_src = strlen(in);
  len_ext = strlen(ctx->extension);

  if (len_src == 0) {
    in = (char *)"output";
    len_src = 6;
  }

  if (len_src > len_ext && strcmp(in + len_src - len_ext, ctx->extension) == 0) {
    /* Already has correct extension and fits in buffer */
    if (len_src < len_out)
      strncpy(in, out, len_out);
    else
      success = ENOMEM;
  } else if (len_src > len_ext) {
    /* Doesn't have correct extension and fits */
    if (len_src + len_ext < len_out) {
      strncpy(out, in, len_out);
      strncat(out, ctx->extension, len_out);
    } else
      success = ENOMEM;
  } else {
    /* The input buffer plus an extension cannot fit in the output buffer */
    fprintf(stderr, "Error building image output filename\n");
    success = ENOMEM;
  }
  return success;
}

/*
 * get_dt
 * Load the dispatch table for the specified image
 * format. Currently only pixmap supported.
 */
int get_dt(image_dispatch_table_t *dt, int format) {
  int success = 0;

  memset((void *)dt, 0x00, sizeof(image_dispatch_table_t));
  switch (format) {
    case IMAGE_PPM:
      *dt = ppm_dt;
      break;
    case IMAGE_LIBRARY:
      success = load_library(dt);
      break;
    default:
      success = EINVAL;
  }
  return success;
}

/*
 * ==WARNING==: Here be dragons!
 * Experimantal features beyond this point.
 * Only use if you are sure you know what you
 * are doing!
 */

/*
 * image_set_library
 * Set the library to use for generating images
 */
int image_set_library(char *library) {
  char *libname;
  std::size_t length;

  length = strlen(library) + 1;
  libname = (char *)calloc(length, sizeof(char));
  if (libname == nullptr) return ENOMEM;
  strncpy(libname, library, length);

  dynamic_backend = libname;
  default_format = IMAGE_LIBRARY;
  return 0;
}

/*
 * load_library
 * External image processing: experimental feature
 * Load an external library to perform image processing
 * It must be a custom compatible library
 */
int load_library(image_dispatch_table_t *dt) {
  void *hndl;
  int success = 0;

  /* Validate object file */
  if (dynamic_backend == nullptr || strlen(dynamic_backend) == 0) {
    fprintf(stderr, "Custom image processor requested without specification\n");
    return EINVAL;
  }

  /* Load shared object and locate required exports */
  hndl = dlopen(dynamic_backend, RTLD_LAZY);
  if (hndl == nullptr) {
    fprintf(stderr, "Error loading shared object\n");
    return EINVAL;
  }
  /* Perform symbol lookup */
  if ((dt->init = (_init *)dlsym(hndl, "lib_init")) == nullptr ||
      (dt->add_pixel = (_add_pixel *)dlsym(hndl, "lib_add_pixel")) == nullptr ||
      (dt->write = (_write *)dlsym(hndl, "lib_write")) == nullptr) {
    fprintf(stderr, "Invalid image processing module specified\n\t%s", dlerror());
    success = EINVAL;
    (void)dlclose(hndl);
  }
  /* Lookup optional symbols, these can return nullptr */
  if (success == 0) {
    dt->get_pixel = (_get_pixel *)dlsym(hndl, "lib_get_pixel");
    dt->set_pixel = (_set_pixel *)dlsym(hndl, "lib_set_pixel");
    dt->free = (_free *)dlsym(hndl, "lib_free");
  }

  return success;
}
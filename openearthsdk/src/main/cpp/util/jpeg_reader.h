
#ifndef  INCLUDE_IMAGE
#include "image.h"
#define INCLUDE_IMAGE
#endif

RawImageData get_raw_image_data_from_jpeg(const void *data, size_t len);
RawImageData get_raw_image_data_from_jpeg_file(const char *path);
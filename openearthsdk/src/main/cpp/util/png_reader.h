#ifndef  INCLUDE_IMAGE
#include "image.h"
#define INCLUDE_IMAGE
#endif

/* Returns the decoded image data, or aborts if there's an error during decoding. */
RawImageData get_raw_image_data_from_png(const void* png_data, const int png_data_size);
void release_png_raw_image_data(const RawImageData *data);

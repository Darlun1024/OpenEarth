#include <GLES2/gl2.h>

#ifndef  OPENEARTH_IMAGE_H
#define  OPENEARTH_IMAGE_H

typedef struct {
	const int width;
	const int height;
	const int size;
	const GLenum gl_color_format;
	const void* data;
}RawImageData;

void release_raw_image_data(const RawImageData *data);
RawImageData get_raw_image_data(const void* img_data, const int img_data_size);

#endif
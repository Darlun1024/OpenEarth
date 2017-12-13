#include <GLES2/gl2.h>

typedef struct {
	const int width;
	const int height;
	const int size;
	const GLenum gl_color_format;
	const void* data;
}RawImageData;

void release_raw_image_data(const RawImageData *data);
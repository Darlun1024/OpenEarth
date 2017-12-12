
#ifndef  INCLUDE_IMAGE
#include "image.h"
#define INCLUDE_IMAGE
#endif

RawImageData decompressJpeg(const void* data, size_t len);
RawImageData decompressJpegFromFile(const char* path);
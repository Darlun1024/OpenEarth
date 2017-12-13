
#include <assert.h>
#include <malloc.h>

#ifndef  INCLUDE_IMAGE
#include "image.h"
#define INCLUDE_IMAGE
#endif

void release_raw_image_data(const RawImageData *data){
    assert(data != NULL);
    free((void*)data->data);
}
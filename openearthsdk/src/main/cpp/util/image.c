
#include <assert.h>
#include <malloc.h>
#include "png_reader.h"
#include "jpeg_reader.h"
#ifndef  INCLUDE_IMAGE
#include "image.h"
#define INCLUDE_IMAGE
#endif

void release_raw_image_data(const RawImageData *data){
    assert(data != NULL);
    free((void*)data->data);
}

RawImageData get_raw_image_data(const void* img_data, const int img_data_size){
    uint8_t* data = img_data;
    //判断文件是png还是jpeg
    //png 文件 以 89 50 4E 47 开头
    //C 语言中 U标志无符号整形 L 表示Long  F 表示浮点型
    if (img_data_size >= 4) {
        uint32_t magic = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        if (magic == 0x89504E47U) {
            return get_raw_image_data_from_png(img_data,img_data_size);
        }
    }
    //jpeg 文件以 ff d8 开头
    if (img_data_size >= 2) {
        uint16_t magic = ((data[0] << 8) | data[1]) & 0xffff;
        if (magic == 0xFFD8) {
            return get_raw_image_data_from_jpeg(data,img_data_size);
        }
    }
}
#include "jpeg_reader.h"
#include <string.h>
#include <stdlib.h>
#include <libjpeg/jpeglib.h>
#include <stdio.h>


RawImageData get_raw_image_data_from_jpeg(const void *data, size_t len) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo,data,len);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	int row_width = cinfo.output_width * cinfo.output_components;
	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);
	unsigned char * output_buffer = (unsigned char *)malloc(row_width * cinfo.output_height);
	memset(output_buffer, 0, row_width * cinfo.output_height);
	unsigned char * tmp = output_buffer;
	/* Process data */
	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(tmp, *buffer, row_width);
		tmp += row_width;
	}

	jpeg_destroy_decompress(&cinfo);
	return (RawImageData){
			cinfo.image_width,
			cinfo.image_height,
			row_width*cinfo.image_height,
			GL_RGB,
			output_buffer
	};



}

RawImageData get_raw_image_data_from_jpeg_file(const char *path) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // Setup decompression structure
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    FILE *file = fopen(path, "r");
    jpeg_stdio_src(&cinfo, file);

    // read info from header.
    //TODO fix it
    int r = jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int row_width = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);
    unsigned char *output_buffer = (unsigned char *) malloc(row_width * cinfo.output_height);
    memset(output_buffer, 0, row_width * cinfo.output_height);
    unsigned char *tmp = output_buffer;
    /* Process data */
    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(tmp, *buffer, row_width);
        tmp += row_width;
    }

    jpeg_destroy_decompress(&cinfo);
    fclose(file);
    return (RawImageData) {
            cinfo.output_width,
            cinfo.output_height,
            row_width * cinfo.image_height,
            GL_RGB,
            output_buffer
    };
}





#include "png_reader.h"
#include <png.h>
#include <string.h>
#include <stdlib.h>
#include <pngconf.h>
#include <jpeglib.h>


const static JOCTET EOI_BUFFER[1] = { JPEG_EOI };

typedef struct  {
    struct jpeg_source_mgr pub;
    const JOCTET *data;
    size_t       len;
}my_source_mgr;

static void my_init_source(j_decompress_ptr cinfo) {}

static boolean my_fill_input_buffer(j_decompress_ptr cinfo) {
	my_source_mgr* src = (my_source_mgr*)cinfo->src;
	// No more data.  Probably an incomplete image;  just output EOI.
	src->pub.next_input_byte = EOI_BUFFER;
	src->pub.bytes_in_buffer = 1;
	return TRUE;
}
static void my_skip_input_data(j_decompress_ptr cinfo, long num_bytes) {
	my_source_mgr* src = (my_source_mgr*)cinfo->src;
	if (src->pub.bytes_in_buffer < num_bytes) {
		// Skipping over all of remaining data;  output EOI.
		src->pub.next_input_byte = EOI_BUFFER;
		src->pub.bytes_in_buffer = 1;
	} else {
		// Skipping over only some of the remaining data.
		src->pub.next_input_byte += num_bytes;
		src->pub.bytes_in_buffer -= num_bytes;
	}
}
static void my_term_source(j_decompress_ptr cinfo) {}

static void my_set_source_mgr(j_decompress_ptr cinfo, const char* data, size_t len) {
	my_source_mgr* src;
	if (cinfo->src == 0) { // if this is first time;  allocate memory
		cinfo->src = (struct jpeg_source_mgr *)(*cinfo->mem->alloc_small)
				((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_source_mgr));
	}
	src = (my_source_mgr*) cinfo->src;
	src->pub.init_source = my_init_source;
	src->pub.fill_input_buffer = my_fill_input_buffer;
	src->pub.skip_input_data = my_skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; // default
	src->pub.term_source = my_term_source;
	// fill the buffers
	src->data = (const JOCTET *)data;
	src->len = len;
	src->pub.bytes_in_buffer = len;
	src->pub.next_input_byte = src->data;
}

RawImageData decompressJpeg(const void* data, size_t len) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	// Setup decompression structure
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	my_set_source_mgr(&cinfo, data,len);

	// read info from header.
    //TODO fix it
	int r = jpeg_read_header(&cinfo, TRUE);
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

RawImageData decompressJpegFromFile(const char* path) {
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





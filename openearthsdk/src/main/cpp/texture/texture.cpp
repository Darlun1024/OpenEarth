#include <string>
#include "texture.hpp"
#include "../util/assets_file_reader.hpp"

extern "C" {
#include "../util/image.h"
#include "../util/jpeg_reader.h"
#include "../util/png_reader.h"
}


GLuint OpenEarth::Texture::loadTextureId(AAssetManager *amgr, const char *path) {
    FileData fileData = OpenEarth::util::AssetsFileReader::get_asset_data(path, amgr);
    RawImageData* data;

    std::string spath = path;
    int extIndex = spath.find(".");
    std::string ext = spath.substr(extIndex + 1, spath.length() - extIndex - 1);
    if (ext.compare("png") == 0) {
        RawImageData dataPng = get_raw_image_data_from_png(fileData.data, (int) fileData.data_length);
        data = &dataPng;
    } else if(ext.compare("jpg") == 0 || ext.compare("jpeg") == 0) {
        RawImageData dataJpg = get_raw_image_data_from_jpeg(fileData.data, (int) fileData.data_length);
        data = &dataJpg;
    } else{
        return 0;
    }

    OpenEarth::util::AssetsFileReader::release_asset_data(&fileData);
    GLuint textureId;
    glGenTextures(1, &textureId);
    assert(textureId != 0);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//超出图片范围，不重复
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
            GL_TEXTURE_2D, 0, data->gl_color_format, data->width, data->height, 0,
            data->gl_color_format, GL_UNSIGNED_BYTE, data->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    release_raw_image_data(data);
    return textureId;
}

GLuint OpenEarth::Texture::loadTextureId(const char *filePath) {
//        RawImageData data = decompressJpegFromFile("/storage/emulated/0/west.jpeg");
}
//
// Created by GXSN-Pro on 2017/11/29.
//

#include "tile.hpp"
#include <math.h>
#include "constants.hpp"
#include "../util/assets_file_reader.hpp"
#include <GLES2/gl2.h>

extern "C" {
    #include "../util/png_reader.h"
    #include "../util/jpeg_reader.h"
}
GLuint loadTexture(AAssetManager *amgr, const char *path);


OpenEarth::Tile::Tile(uint32_t x, uint32_t y, uint32_t z) {
    this->x = x;
    this->y = y;
    this->z = z;

    double degreeOfEachTile = 360 / pow(2, z);
    double left = x * degreeOfEachTile;
    double right = left + degreeOfEachTile;
    double top = 90 - (y * degreeOfEachTile);
    double bottom = top - degreeOfEachTile;
    bounds[0] = left;
    bounds[1] = top;
    bounds[2] = right;
    bounds[3] = bottom;
    genVertexArray();
}

OpenEarth::Tile::~Tile() {
    delete[] this->vertexArray;
    delete[] this->bounds;
    delete this->stripes;
}


void OpenEarth::Tile::draw(GLuint aPostionLocaiton, GLuint aTextureLocation, AAssetManager *amgr,
                      const char *path) {
    GLuint textureId = loadTexture(amgr, path);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    for (int i = 0; i < rows; i++) {
        GLfloat *vertexArray = stripes[i];
        glVertexAttribPointer(aPostionLocaiton, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                              vertexArray);
        glEnableVertexAttribArray(aPostionLocaiton);
//        (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
        glVertexAttribPointer(aTextureLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                              vertexArray + 3);
        glEnableVertexAttribArray(aPostionLocaiton);
        glEnableVertexAttribArray(aTextureLocation);

        int pointCount = cols * 2;
        glDrawArrays(GL_TRIANGLE_STRIP, 0, pointCount);

    }
    glDeleteTextures(1,&textureId);  //需要及时清理纹理，否则每次都增加新的纹理到内存中，以后可以考虑使用缓存，不必每次都重新加载纹理
}

static float dtor(float d) {
    return M_PI * d / 180;
}

/**
 *用绘制球体的方法，绘制瓦片是不行的，在右边界的点会左边界的点连接起来
 */
void OpenEarth::Tile::genVertexArray() {
    float R = OpenEarth::RADIUS;
    float width = bounds[2] - bounds[0];
    float height = bounds[1] - bounds[3];

    rows = height / step;
    cols = width / step +1;

    uint32_t size = cols * rows;  //后期考虑不能整除的情况

    vertexSize = size;  //x,y,z,s,t
    vertexArray = new GLfloat[size * 2 * 5];
    stripes = new GLfloat *[rows];
    uint32_t index = 0;
    float x1, x2, y1, y2, z1, z2;
    float imgXStep = 1.0f/(cols-1);
    float imgYStep = 1.0f/rows;
    float imageX;
    float imageY = 0.0f;
    int row = 0;
    for (float lat = bounds[1]; lat > bounds[3]; lat -= step) {
        float latR1 = dtor(lat);
        float latR2 = dtor(lat - step);
        y1 = (float) (R * sin(latR1));
        y2 = (float) (R * sin(latR2));
        imageX = 0.0f;
        GLfloat *vertexs = new GLfloat[cols * 2 * 5];
        index = 0;
        float lon;
        for (lon = bounds[0]; lon <= bounds[2]; lon += step) {
//            if(lon==bounds[2]) lon = lon - (lon/100000000);
            float lonR1 = dtor(lon);
            x1 = (float) (R * cos(latR1) * sin(lonR1));
            z1 = (float) (R * cos(latR1) * cos(lonR1));
            x2 = (float) (R * cos(latR2) * sin(lonR1));
            z2 = (float) (R * cos(latR2) * cos(lonR1));

            vertexs[index++] = x1;
            vertexs[index++] = y1;
            vertexs[index++] = z1;
            vertexs[index++] = imageX;
            vertexs[index++] = imageY;
            vertexs[index++] = x2;
            vertexs[index++] = y2;
            vertexs[index++] = z2;
            vertexs[index++] = imageX;
            vertexs[index++] = imageY + imgYStep;
            imageX += imgXStep;
        }
//        LOGE("Tile", "%d,%f,%f,%f",index,lon-step,imageX-imgXStep, imageY);
        imageY += imgYStep;
        stripes[row++] = vertexs;
    }
}

    /**
     * 加载纹理
     */
    GLuint loadTexture(AAssetManager *amgr, const char *path) {
        FileData fileData = OpenEarth::util::AssetsFileReader::get_asset_data(path, amgr);
        RawImageData data = get_raw_image_data_from_jpeg(fileData.data, (int) fileData.data_length);
//        RawImageData data = decompressJpegFromFile("/storage/emulated/0/west.jpeg");
//        RawImageData data = get_raw_image_data_from_png(fileData.data,(int) fileData.data_length);

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
                GL_TEXTURE_2D, 0, data.gl_color_format, data.width, data.height, 0,
                data.gl_color_format, GL_UNSIGNED_BYTE, data.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        release_raw_image_data(&data);
        return textureId;
    }


//
// Created by GXSN-Pro on 2017/11/29.
//

#include "tile.hpp"
#include <math.h>
#include "../util/assets_file_reader.hpp"
#include "earth.hpp"
#include <GLES2/gl2.h>
#include <sstream>

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

void OpenEarth::Tile::reset() {
    delete[] this->vertexArray;
    delete  this->stripes;
    genVertexArray();
}



void OpenEarth::Tile::draw(GLuint aPostionLocaiton, GLuint aTextureLocation,GLuint textureId){
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
    glDeleteTextures(1, &textureId);
}

static float dtor(float d) {
    return M_PI * d / 180;
}

/**
 *用绘制球体的方法，绘制瓦片是不行的，在右边界的点会左边界的点连接起来
 */
void OpenEarth::Tile::genVertexArray() {
    float R = OpenEarth::Earth::getRadius();
    float width  = bounds[2] - bounds[0];
    float height = bounds[1] - bounds[3];

    if(std::fmod(width,step)>0){
       step = width / 4;
    }


    rows = height / step;
    cols = width / step + 1;

    uint32_t size = cols * rows;  //后期考虑不能整除的情况

    vertexSize = size;  //x,y,z,s,t
    vertexArray = new GLfloat[size * 2 * 5];
    stripes = new GLfloat *[rows];
    uint32_t index = 0;
    float x1, x2, y1, y2, z1, z2;
    float imgXStep = 1.0f / (cols - 1);
    float imgYStep = 1.0f / rows;
    float imageX;
    float imageY = 0.0f;
    int row = 0;
    for (float lat = bounds[1]; lat > bounds[3]; lat -= step) {
        float latR1 = dtor(lat);
        float latR2 = dtor(lat - step);
        y1 = (float) (R * sin(latR1));
        y2 = (float) (R * sin(latR2));
        float cosLat1 = R * cos(latR1);
        float cosLat2 = R * cos(latR2);
        imageX = 0.0f;
        GLfloat *vertexs = new GLfloat[cols * 2 * 5];
        index = 0;
        float lon;
        for (lon = bounds[0]; lon <= bounds[2]; lon += step) {
            float lonR1 = dtor(lon);
            x1 = (float) (cosLat1 * sin(lonR1));
            z1 = (float) (cosLat1 * cos(lonR1));
            x2 = (float) (cosLat2 * sin(lonR1));
            z2 = (float) (cosLat2 * cos(lonR1));

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




std::string OpenEarth::Tile::genUniqueCode(int zoom,int x,int y){
    std::stringstream stringStream;
    stringStream << zoom << "_" << x << "_" << y << std::endl;
    return stringStream.str();
}




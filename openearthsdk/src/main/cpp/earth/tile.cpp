//
// Created by GXSN-Pro on 2017/11/29.
//

#include "tile.hpp"
#include <math.h>
#include "constants.hpp"
#include "../logging.hpp"
#include <android/log.h>


GLfloat *OpenEarth::Tile::getVertexArray() {
    return vertexArray;
}


int OpenEarth::Tile::getVertexArraySize() {
    return vertexSize * 2;
}

OpenEarth::Tile::Tile(int x, int y, int z) {
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
    delete this->vertexArray;
    delete this->bounds;
    delete this->stripes;
}


void OpenEarth::Tile::draw(GLuint aPostionLocaiton, GLuint aTextureLocation) {

    for (int i = 0; i < rows; i++) {
        GLfloat *vertexArray = stripes[i];
        glVertexAttribPointer(aPostionLocaiton, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                              vertexArray);
        glEnableVertexAttribArray(aPostionLocaiton);
//        (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
        glVertexAttribPointer(aTextureLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                              vertexArray+3);
        glEnableVertexAttribArray(aPostionLocaiton);
        glEnableVertexAttribArray(aTextureLocation);

        int pointCount = cols*2;
        glDrawArrays(GL_TRIANGLE_STRIP, 0, pointCount);

    }
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
     cols = width / step;

    int size = cols * rows;  //后期考虑不能整除的情况

    vertexSize = size;  //x,y,z,s,t
    vertexArray = new GLfloat[size * 2 * 5];
    stripes = new GLfloat *[rows];
    int index = 0;
    float x1, x2, y1, y2, z1, z2;
    float imgXStep = step / width;
    float imgYStep = step / height;
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
        for (float lon = bounds[0]; lon < bounds[2]; lon += step) {
            float lonR1 = dtor(lon);
            float lonR2 = dtor(lon + step);
            x1 = (float) (R * cos(latR1) * sin(lonR1));
            z1 = (float) (R * cos(latR1) * cos(lonR1));
            x2 = (float) (R * cos(latR2) * sin(lonR2));
            z2 = (float) (R * cos(latR2) * cos(lonR2));
            vertexs[index++] = x1;
            vertexs[index++] = y1;
            vertexs[index++] = z1;
            vertexs[index++] = imageX;
            vertexs[index++] = imageY;
            vertexs[index++] = x2;
            vertexs[index++] = y2;
            vertexs[index++] = z2;
            vertexs[index++] = imageX + imgXStep;
            vertexs[index++] = imageY + imgYStep;
            LOGE("sphere", "%d,%f,%f,%f,%f,%f", index, x1, y1, z1, imageX, imageY);
            imageX += imgXStep;
        }
        imageY += imgYStep;
        stripes[row++] = vertexs;
    }


}


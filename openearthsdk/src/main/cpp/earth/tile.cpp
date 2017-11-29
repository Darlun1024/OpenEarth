//
// Created by GXSN-Pro on 2017/11/29.
//

#include "tile.hpp"
#include <math.h>
#include "constants.hpp"
#include "../logging.hpp"
#include <android/log.h>


GLfloat* OpenEarth::Tile::getVertexArray(){
    return vertexArray;
}

GLfloat* OpenEarth::Tile::getTextureVertexArray(){
    return textureVertexArray;
}

int OpenEarth::Tile::getVertexArraySize(){
    return vertexSize;
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
    delete  this->vertexArray;
    delete  this->bounds;
}


void OpenEarth::Tile::draw() {

}

static float dtor(float d) {
    return M_PI * d / 180;
}

/**
 *
 */
void OpenEarth::Tile::genVertexArray() {
    float R = OpenEarth::RADIUS;
    float width  = bounds[2] - bounds[0];
    float height = bounds[1] - bounds[3];
    int size = (int) ((height / step) * (width / step));  //后期考虑不能整除的情况
    vertexSize = size;  //x,y,z,s,t
    vertexArray = new GLfloat[size*2*3];
    textureVertexArray = new GLfloat[size*2*2];
    int index = 0;
    int texIndex = 0;
    float x1, x2, y1, y2, z1, z2;
    float imgXStep = step / width;
    float imgYStep = step / height;
    float imageX;
    float imageY = 0.0f;
    for (float lat = bounds[1]; lat > bounds[3]; lat -= step) {
        float latR1 = dtor(lat);
        float latR2 = dtor(lat - step);
        y1 = (float) (R * sin(latR1));
        y2 = (float) (R * sin(latR2));
        imageX = 0.0f;
        for (float lon = bounds[0]; lon < bounds[2]; lon += step) {
            float lonR1 = dtor(lon);
            float lonR2 = dtor(lon + step);
            x1 = (float) (R * cos(latR1) * sin(lonR1));
            z1 = (float) (R * cos(latR1) * cos(lonR1));
            x2 = (float) (R * cos(latR2) * sin(lonR2));
            z2 = (float) (R * cos(latR2) * cos(lonR2));
            vertexArray[index++] = x1;
            vertexArray[index++] = y1;
            vertexArray[index++] = z1;
            textureVertexArray[texIndex++] = imageX; //s
            textureVertexArray[texIndex++] = imageY; //t
            vertexArray[index++] = x2;
            vertexArray[index++] = y2;
            vertexArray[index++] = z2;
            textureVertexArray[texIndex++] = imageX + imgXStep; //s
            textureVertexArray[texIndex++] = imageY + imgYStep; //t
            LOGE("sphere","%f,%f,%f,%f,%f",x1,y1,z1,imageX,imageY);
            imageX += imgXStep;
        }
        imageY += imgYStep;
    }
}


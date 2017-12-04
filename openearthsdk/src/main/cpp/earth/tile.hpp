//
// Created by GXSN-Pro on 2017/11/29.
//

#ifndef OPENEARTH_TILE_HPP
#define OPENEARTH_TILE_HPP

#include <GLES2/gl2.h>
#include <memory>

namespace OpenEarth {
    class Tile {
    public:
        int x,y,z;
        int cols,rows;
        GLfloat* getVertexArray();
        int getVertexArraySize();
        Tile(int x,int y,int z);
        ~Tile();
        void draw(GLuint aPostionLocaiton, GLuint aTextureLocation);
    private:
        double *bounds = new double[4];//left,top,right,bottom
        float step = 3.0f;
        int vertexSize = 0;
        GLfloat *vertexArray;
        GLfloat **stripes;
        void genVertexArray();

    };
}

#endif //OPENEARTH_TILE_HPP

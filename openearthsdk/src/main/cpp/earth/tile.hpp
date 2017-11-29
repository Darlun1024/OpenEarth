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
        GLfloat* getVertexArray();
        GLfloat* getTextureVertexArray();
        int getVertexArraySize();
        Tile(int x,int y,int z);
        ~Tile();
        void draw();
    private:
        double *bounds = new double[4];//left,top,right,bottom
        float step = 1.0f;
        int vertexSize = 0;
        GLfloat *vertexArray;
        GLfloat *textureVertexArray;
        void genVertexArray();

    };
}

#endif //OPENEARTH_TILE_HPP

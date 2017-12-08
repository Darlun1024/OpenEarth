//
// Created by GXSN-Pro on 2017/11/29.
//

#ifndef OPENEARTH_TILE_HPP
#define OPENEARTH_TILE_HPP

#include <GLES2/gl2.h>
#include <memory>
#include <android/asset_manager.h>

namespace OpenEarth {
    class Tile {
    public:
        uint32_t x,y,z;
        uint32_t cols,rows;
        Tile(uint32_t x,uint32_t y,uint32_t z);
        ~Tile();
        void draw(GLuint aPostionLocaiton, GLuint aTextureLocation,AAssetManager *amgr,
        const char *path);
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

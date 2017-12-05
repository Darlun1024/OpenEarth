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
        int x,y,z;
        int cols,rows;
        Tile(int x,int y,int z);
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

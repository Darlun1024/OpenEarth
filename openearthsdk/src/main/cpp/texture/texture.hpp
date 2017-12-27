

#ifndef OPENEARTH_TEXTURE_HPP
#define OPENEARTH_TEXTURE_HPP

#include <GLES2/gl2.h>
#include <android/asset_manager.h>

namespace OpenEarth{
    class Texture{

    public:
        static GLuint loadTextureId(AAssetManager* amgr,const char* path);
        static GLuint loadTextureId(const char* filePath);

    };
}


#endif //OPENEARTH_TEXTURE_HPP


#ifndef OPENEARTH_TEXTURE_HPP
#define OPENEARTH_TEXTURE_HPP

#include <GLES2/gl2.h>
#include <android/asset_manager.h>
#include "../source/http_data_source.hpp"

namespace OpenEarth {
using namespace OpenEarth::DataSource;
    class Texture : public HttpDataSourceCallback{
    private:
        GLuint loadFromCache();
    public:
        Texture();
        ~Texture();
         GLuint loadFromAssets(AAssetManager *amgr, const char *path);
         GLuint loadFormFile(const char* filePath);
         GLuint loadFromNet(JNIEnv* env, const char* url);

         void onResponse(HttpResponse response);
         void onFailure(int code,string message);
    private:
        OpenEarth::DataSource::HttpDataSource* mHttpDataSource;
    };
}


#endif //OPENEARTH_TEXTURE_HPP
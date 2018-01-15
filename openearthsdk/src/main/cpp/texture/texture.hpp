

#ifndef OPENEARTH_TEXTURE_HPP
#define OPENEARTH_TEXTURE_HPP

#include <GLES2/gl2.h>
#include <android/asset_manager.h>
#include "../storage/http_data_source.hpp"
#include <map>
#include <memory>
#include <vector>
#include <list>

extern "C"{
#include "../util/image.h"
};



namespace OpenEarth {
    using namespace OpenEarth::Storage;

    class Texture : public HttpDataSourceCallback {
    private:
        static const int MAX_HTTP_REQUEST_SIZE = 10;
        JNIEnv* mEnv;
        GLuint loadFromCache();
        std::unique_ptr<std::map<string,RawImageData>> mMap; //缓存
        std::unique_ptr<std::vector<string>> mRequestQueue; //正在请求的队列
        std::unique_ptr<std::vector<string>> mWaitingRequestQueue; //等待队列
        void next();
    public:
        Texture();

        ~Texture();

        GLuint loadFromAssets(AAssetManager *amgr, const char *path);

        GLuint loadFormFile(const char *filePath);

        GLuint loadFromNet(JNIEnv *env, const char *url);

        void setJEnv(JNIEnv* env);
        void cancel(const char* url);

        void onResponse(HttpResponse response);

        void onFailure(int code,string url, string message);


    private:
        OpenEarth::Storage::HttpDataSource *mHttpDataSource;
    };
}


#endif //OPENEARTH_TEXTURE_HPP
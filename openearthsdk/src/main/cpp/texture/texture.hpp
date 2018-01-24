

#ifndef OPENEARTH_TEXTURE_HPP
#define OPENEARTH_TEXTURE_HPP

#include <GLES2/gl2.h>
#include <android/asset_manager.h>
#include "../storage/http_data_source.hpp"
#include "../storage/mbtile_data_source.hpp"
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
        std::unique_ptr<MBTileDataSource> mMBTileSource; //等待队列
        void next();
    public:
        Texture();

        ~Texture();

        GLuint loadFromAssets(AAssetManager *amgr, const  string path);

        GLuint loadFormFile(const string filePath);

        GLuint loadFromNet(JNIEnv *env,const  string url);

        //离线数据库
        GLuint loadFromDatabase(const string url);

        void cancel(const string url);

        void onResponse(HttpResponse response);

        void onFailure(const int code,const string url, string message);


    private:
        OpenEarth::Storage::HttpDataSource *mHttpDataSource;
    };
}


#endif //OPENEARTH_TEXTURE_HPP
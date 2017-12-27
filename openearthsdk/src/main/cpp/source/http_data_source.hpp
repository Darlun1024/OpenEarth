//
// Created by GXSN-Pro on 2017/12/25.
//

#ifndef OPENEARTH_HTTP_DATA_SOURCE_HPP
#define OPENEARTH_HTTP_DATA_SOURCE_HPP


#include <jni.h>
#include <string>

namespace OpenEarth {
    namespace DataSource {
        using namespace std;
        typedef struct {
            std::string url;
            char *byteArray;
            long length;
        } HttpResponse;

        class HttpDataSourceCallback {
        public:
            virtual void onResponse(HttpResponse response)=0;
            virtual void onFailure(int code, string message)=0;
        };

        class HttpDataSource {
        private:
            static constexpr char *const JavaClassName = "com/geocompass/openearth/sdk/http/HttpRequest";
            static constexpr char *const TAG = "HttpDataSource";
            HttpDataSourceCallback *mCallBack;
        public:
            static void request(JNIEnv *env, string url, HttpDataSourceCallback *callback);
            HttpDataSource();
            ~HttpDataSource();
        public:
            static void registerNative(JNIEnv *env);

            void setCallback(HttpDataSourceCallback *callback);

            HttpDataSourceCallback *getCallback();
        };


    }
}


#endif //OPENEARTH_HTTP_DATA_SOURCE_HPP

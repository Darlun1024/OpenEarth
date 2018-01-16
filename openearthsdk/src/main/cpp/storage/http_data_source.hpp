//
// Created by GXSN-Pro on 2017/12/25.
//

#ifndef OPENEARTH_HTTP_DATA_SOURCE_HPP
#define OPENEARTH_HTTP_DATA_SOURCE_HPP


#include <jni.h>
#include <string>
#include <memory>
#include <vector>

namespace OpenEarth {
    namespace Storage {
        using namespace std;
        typedef struct {
            std::string url;
            char *byteArray;
            long length;
        } HttpResponse;

        class HttpDataSourceCallback {
        public:
            virtual void onResponse(HttpResponse response)=0;
            virtual void onFailure(int code, string  url, string message)=0;
        };

        class HttpDataSource {
        private:
            static constexpr const char * JavaClassName = "com/geocompass/openearth/sdk/http/HttpRequest";
            static constexpr const char * TAG = "HttpDataSource";
            HttpDataSourceCallback *mCallBack;
            static jobject  localHttpObject;
            static jmethodID requestMethodId;
        public:
            static void request( JNIEnv *env, string url, HttpDataSourceCallback *callback);
            HttpDataSource();
            ~HttpDataSource();
            static void registerNative(JNIEnv *env);
            void setCallback(HttpDataSourceCallback *callback);
            HttpDataSourceCallback *getCallback();
        private:

//            static unique_ptr<vector<unique_ptr<HttpDataSource>>> sourceVector; //请求对象
//            static unique_ptr<vector<>>
        };


    }
}


#endif //OPENEARTH_HTTP_DATA_SOURCE_HPP

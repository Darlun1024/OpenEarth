//
// Created by GXSN-Pro on 2017/12/25.
//

#ifndef OPENEARTH_HTTP_DATA_SOURCE_HPP
#define OPENEARTH_HTTP_DATA_SOURCE_HPP


#include <jni.h>

namespace OpenEarth{
    namespace DataSource{
        class HttpDataSource {
        private:
            static constexpr char *const JavaClassName = "com/geocompass/openearth/sdk/http/HttpRequest";
            static constexpr char *const TAG = "HttpDataSource";
        public:
            static void registerNative(JNIEnv *env);
        };
    }
}



#endif //OPENEARTH_HTTP_DATA_SOURCE_HPP

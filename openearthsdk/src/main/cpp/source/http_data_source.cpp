//
// Created by GXSN-Pro on 2017/12/25.
//

#include "http_data_source.hpp"

namespace OpenEarth{
    namespace DataSource{

        void onResponse(JNIEnv* env,jobject* obj,jint code,jbyteArray body){

        }

        void onFailure(JNIEnv* env,jobject* obj,jstring message){

        }


        static JNINativeMethod gMethods[] = {
                {"nativeOnFailure", "(Ljava/lang/String;)V",   (void *) onFailure},
                {"nativeOnResponse", "(I[B)V", (void *) onResponse}

        };

//注册native 方法
        static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods,
                                         int numMethods) {
            jclass clazz;
            clazz = env->FindClass(className);
            if (clazz == NULL) {
                return JNI_FALSE;
            }
            if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
                return JNI_FALSE;
            }
            return JNI_TRUE;
        }

        void HttpDataSource::registerNative(JNIEnv *env) {
            registerNativeMethods(env, JavaClassName, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
        }
    }
}
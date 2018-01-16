//
// Created by GXSN-Pro on 2017/12/25.
//

#include "http_data_source.hpp"
#include "../jni.hpp"
#include <thread>
#include <cstdlib>
#include <jni.h>

namespace OpenEarth{
    namespace Storage{
        void HttpDataSource::request( JNIEnv* env,string url,HttpDataSourceCallback* callback){
            JNIEnv* newEnv;
            jni::getJVM()->AttachCurrentThread(&newEnv, NULL);
            jclass  clazz = newEnv->FindClass(JavaClassName);
            if(clazz == NULL) {
                return;
            }
//            构造函数名称统一为<init>
            jmethodID  initMethodId = newEnv->GetMethodID(clazz,"<init>","(JLjava/lang/String;)V");
            const char* chardata = url.c_str();
            jstring jstr = newEnv->NewStringUTF(chardata);
            jobject  object = newEnv->NewObject(clazz,initMethodId,callback,jstr);
            newEnv->DeleteLocalRef(jstr); //http://blog.csdn.net/xyang81/article/details/44873769
            newEnv->DeleteLocalRef(object);
////            jni::getJVM()->DetachCurrentThread();
        }

        HttpDataSource::HttpDataSource(){

        }

        HttpDataSource::~HttpDataSource(){
            mCallBack = nullptr;
        }


        void HttpDataSource::setCallback(HttpDataSourceCallback* callback){
            mCallBack = std::move(callback);
        }



        HttpDataSourceCallback* HttpDataSource::getCallback(){
            return mCallBack;
        }


        void onResponse(JNIEnv* env,jobject obj,jstring jurl,jbyteArray body){
            jclass clazz  = env->GetObjectClass(obj);
            jfieldID fieldId = env->GetFieldID(clazz,"mNativePtr","J");
            long nativePtr = env->GetLongField(obj,fieldId);
            HttpDataSourceCallback* callback = (HttpDataSourceCallback*)nativePtr;
            if(callback == nullptr)
                return;
            jboolean isCopy = true;
            const char* url =  env->GetStringUTFChars(jurl,&isCopy);
//            拷贝一份数据
            jsize len  = env->GetArrayLength(body);
            jbyte *jbarray = (jbyte *)malloc(len * sizeof(jbyte));
            env->GetByteArrayRegion(body,0,len,jbarray);

            char *data = (char *)jbarray;
            HttpResponse response {
                url,
                data,
                len
            };
            callback->onResponse(response);
            env->ReleaseStringUTFChars(jurl,url);
            env->ReleaseByteArrayElements(body,jbarray,JNI_COMMIT);
        }

        void onFailure(JNIEnv* env,jobject obj,jstring jurl,jstring jmessage){
            jclass clazz  = env->GetObjectClass(obj);
            jfieldID fieldId = env->GetFieldID(clazz,"mNativePtr","J");
            long nativePtr = env->GetLongField(obj,fieldId);
            HttpDataSourceCallback* callback = (HttpDataSourceCallback*)nativePtr;
            if(callback == nullptr)
                return;
            jboolean isCopy = true;
            const char* url =  env->GetStringUTFChars(jurl,&isCopy);
            const char* message =  env->GetStringUTFChars(jmessage,&isCopy);
            callback->onFailure(0,url,message);
            env->ReleaseStringUTFChars(jmessage,message);
            env->ReleaseStringUTFChars(jurl,url);
        }


        static JNINativeMethod gMethods[] = {
                {"nativeOnFailure", "(Ljava/lang/String;Ljava/lang/String;)V",   (void *) onFailure},
                {"nativeOnResponse", "(Ljava/lang/String;[B)V", (void *) onResponse}
        };

//注册native 方法
        static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods,
                                         int numMethods) {
            jclass clazz = env->FindClass(className);
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
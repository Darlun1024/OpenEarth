//
// Created by GXSN-Pro on 2017/12/25.
//

#include "http_data_source.hpp"
#include "../jni.hpp"
#include <thread>
#include <cstdlib>

namespace OpenEarth{
    namespace Storage{
        void HttpDataSource::request(JNIEnv* env,string url,HttpDataSourceCallback* callback){
            HttpDataSource* dataSource = new HttpDataSource();
            dataSource->setCallback(callback);
            jclass clazz;
            clazz = env->FindClass(JavaClassName);
            if(clazz == NULL) {
                return;
            }
//            构造函数名称统一为<init>
            jmethodID  initMethodId = env->GetMethodID(clazz,"<init>","(JLjava/lang/String;)V");
            const char* chardata = url.c_str();
            jstring jstr = jni::char2JString(env,chardata);
            jobject  object = env->NewObject(clazz,initMethodId,dataSource,jstr);
            env->DeleteLocalRef(jstr); //http://blog.csdn.net/xyang81/article/details/44873769
            env->DeleteLocalRef(object);
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
            HttpDataSource* dataSource = (HttpDataSource*)nativePtr;
            if(dataSource == nullptr)
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
            dataSource->getCallback()->onResponse(response);
            env->ReleaseStringUTFChars(jurl,url);
        }

        void onFailure(JNIEnv* env,jobject obj,jstring jurl,jstring jmessage){
            jclass clazz  = env->GetObjectClass(obj);
            jfieldID fieldId = env->GetFieldID(clazz,"mNativePtr","J");
            long nativePtr = env->GetLongField(obj,fieldId);
            HttpDataSource* dataSource = (HttpDataSource*)nativePtr;
            if(dataSource== nullptr)
                return;
            jboolean isCopy = true;
            const char* url =  env->GetStringUTFChars(jurl,&isCopy);
            const char* message =  env->GetStringUTFChars(jmessage,&isCopy);
            dataSource->getCallback()->onFailure(0,url,message);
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
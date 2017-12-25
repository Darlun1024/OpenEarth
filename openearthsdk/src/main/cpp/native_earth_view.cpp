//
// Created by GXSN-Pro on 2017/11/14.
//

#include <cstddef>
#include "native_earth_view.hpp"


//Java 类名
static const char* const JavaClassName="com/geocompass/openearth/sdk/earth/NativeEarthView";


OpenEarth::NativeEarthView::NativeEarthView(){

}

OpenEarth::NativeEarthView::~NativeEarthView(){

}

void initializeEarth(JNIEnv *env,jobject instance,jobject obj) {
}


jstring getString(JNIEnv *env,jobject instance) {
    const char *returnValue = "hello";
    return env->NewStringUTF(returnValue);
}


// {"getStringFromJNI", "()Ljava/lang/String;", (jstring*)getString}
//三个参数分别为 Java中的方法名，参数和返回类型，C++中的函数名
static JNINativeMethod gMethods[] = {
        {"getStringFromJNI", "()Ljava/lang/String;", (jstring*)getString},
        {"nativeInitializeEarth","(Lcom/geocompass/openearth/sdk/earth/EarthRenderer;)V",(void*)initializeEarth}
};

//注册native 方法
static int registerNativeMethods(JNIEnv* env
        , const char* className
        , JNINativeMethod* gMethods, int numMethods) {
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

void OpenEarth::NativeEarthView::registerNative(JNIEnv *env){
    registerNativeMethods(env,JavaClassName,gMethods, sizeof(gMethods)/ sizeof(gMethods[0]));
}





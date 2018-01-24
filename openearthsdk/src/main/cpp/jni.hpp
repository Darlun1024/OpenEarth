//
// Created by GXSN-Pro on 2017/11/14.
//

#ifndef OPENEARTH_JNI_HPP
#define OPENEARTH_JNI_HPP


#include <jni.h>
#include <memory>
#include <jni.h>

class jni {
public:
    static char* jString2Char(JNIEnv* env,jstring string);
    static jstring  char2JString(JNIEnv* env,const char * c);
    static void setJVM(JavaVM* jvm);
    static JavaVM* getJVM();
};


#endif //OPENEARTH_JNI_HPP

//
// Created by GXSN-Pro on 2017/11/14.
//

#include <cstdlib>
#include <string>
#include "jni.hpp"

char *jni::jString2Char(JNIEnv *env, jstring jstr) {
    char* result = NULL;
    jclass stringClass = env->FindClass("java/lang/String");
    jstring encoding = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray jbArr = (jbyteArray) env->CallObjectMethod(jstr, mid, encoding);
    jsize arrayLength = env->GetArrayLength(jbArr);
    jbyte* ba = env->GetByteArrayElements(jbArr, JNI_FALSE);
    if (arrayLength > 0) {
        result = (char*) malloc(arrayLength + 1);
        memcpy(result, ba, arrayLength);
        result[arrayLength] = 0;
    }
    env->ReleaseByteArrayElements(jbArr, ba, 0);
    return result;
}

jstring jni::char2JString(JNIEnv *env, const char *c) {
    jclass stringClass = (env)->FindClass("java/lang/String");
    jmethodID constructId = (env)->GetMethodID(stringClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = (env)->NewByteArray(strlen(c));
    (env)->SetByteArrayRegion(bytes, 0, strlen(c), (jbyte*) c);
    jstring encoding = (env)->NewStringUTF("GB2312");
    return (jstring) (env)->NewObject(stringClass, constructId, bytes, encoding);
}
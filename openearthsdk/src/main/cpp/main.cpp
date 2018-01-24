
#include <jni.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "jni.hpp"
#include "native_earth_view.hpp"
#include "earth/earth_renderer.hpp"
#include "storage/http_data_source.hpp"

//LoadLibrary后，首先调用该方法
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    assert(env != NULL);

    OpenEarth::NativeEarthView::registerNative(env);
    OpenEarth::EarthRenderer::registerNative(env);
    OpenEarth::Storage::HttpDataSource::registerNative(env);

    result = JNI_VERSION_1_6;
    jni::setJVM(vm);
    return result;
}




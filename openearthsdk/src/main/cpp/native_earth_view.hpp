//
// Created by GXSN-Pro on 2017/11/14.
//
#include <jni.h>

#ifndef OPENEARTH_NATIVE_EARTH_VIEW_HPP
#define OPENEARTH_NATIVE_EARTH_VIEW_HPP

namespace OpenEarth {

    class NativeEarthView {


    public:
        static void registerNative(JNIEnv *env);


        NativeEarthView();

        ~NativeEarthView();

    private:

    };
}


#endif //OPENEARTH_NATIVE_EARTH_VIEW_HPP

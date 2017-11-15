//
// Created by GXSN-Pro on 2017/11/15.
//

#ifndef OPENEARTH_EARTH_RENDERER_HPP
#define OPENEARTH_EARTH_RENDERER_HPP

#include <jni.h>
#include <GLES2/gl2.h>
namespace OpenEarth {
    class EarthRenderer {

    public:
        static void registerNative(JNIEnv *env);


        EarthRenderer();

        ~EarthRenderer();

    };
}

#endif //OPENEARTH_EARTH_RENDERER_HPP

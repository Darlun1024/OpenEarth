//
// Created by GXSN-Pro on 2017/11/15.
//

#ifndef OPENEARTH_EARTH_RENDERER_HPP
#define OPENEARTH_EARTH_RENDERER_HPP

#include <jni.h>
namespace OpenEarth {
    class EarthRenderer {

    public:
        static void registerNative(JNIEnv *env);


        EarthRenderer();

        ~EarthRenderer();

    private:
    };
}

#endif //OPENEARTH_EARTH_RENDERER_HPP

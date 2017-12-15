//
// Created by GXSN-Pro on 2017/12/14.
//

#include "earth.hpp"

namespace OpenEarth {
    static float radius = DEFAULT_RADIUS;
    static float scale = DEFAULT_SCALE;

    void OpenEarth::Earth::setRadius(float r) {
        radius = r;
    }

    float OpenEarth::Earth::getRadius() {
        return radius;
    }

    void OpenEarth::Earth::setScale(float scale) {
        scale = scale;
    }

    float OpenEarth::Earth::getScale() {
        return scale;
    }
}
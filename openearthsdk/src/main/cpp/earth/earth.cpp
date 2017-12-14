//
// Created by GXSN-Pro on 2017/12/14.
//

#include <cmath>
#include "earth.hpp"

namespace OpenEarth {
    static float radius = DEFAULT_RADIUS;

    void OpenEarth::Earth::setRadius(float r) {
        radius = r;
    }

    float OpenEarth::Earth::getRadius() {
        return radius;
    }
}
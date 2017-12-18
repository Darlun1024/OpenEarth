//
// Created by GXSN-Pro on 2017/12/14.
//

#include <glm/vec3.hpp>
#include "earth.hpp"

namespace OpenEarth {
    static float radius = DEFAULT_RADIUS;
    static float scale = DEFAULT_SCALE;
    static glm::vec3 center = glm::vec3(0, 0, 0); //球心

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

    glm::vec3 OpenEarth::Earth::getCenter() {
        return center;
    }
}
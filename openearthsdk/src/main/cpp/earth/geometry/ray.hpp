//
// Created by GXSN-Pro on 2017/12/18.
//

#ifndef OPENEARTH_RAY_HPP
#define OPENEARTH_RAY_HPP

#include <glm/vec3.hpp>

namespace OpenEarth {
    namespace Geometry {
        class Ray {
        public:
            glm::vec3 mPoint; //起点
            glm::vec3 mVector; //向量
            Ray(glm::vec3 point,glm::vec3 vector);
            ~Ray();
            float length();
        };
    }

}
#endif //OPENEARTH_RAY_HPP

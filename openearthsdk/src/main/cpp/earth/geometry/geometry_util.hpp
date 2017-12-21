//
// Created by GXSN-Pro on 2017/12/21.
//

#ifndef OPENEARTH_GEOMETRY_UTIL_HPP
#define OPENEARTH_GEOMETRY_UTIL_HPP

#include <glm/vec3.hpp>

namespace OpenEarth {
    namespace Geometry {
        class GeometryUtil {
        public:
            /**
             * 计算一个向量投影到平面后的向量
             * @param vector  投影的向量
             * @param plane   平面的法向量
             * @return
             */
            static glm::vec3 projectToPlane(glm::vec3 vector, glm::vec3 plane);
        };
    }
}

#endif //OPENEARTH_GEOMETRY_UTIL_HPP

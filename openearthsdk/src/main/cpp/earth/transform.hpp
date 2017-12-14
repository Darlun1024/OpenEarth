//
// 球体大小，位置，倾斜度变换
// reated by GXSN-Pro on 2017/12/13.
//

#ifndef OPENEARTH_TRANSFORM_HPP
#define OPENEARTH_TRANSFORM_HPP

#include <glm/vec2.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "geometry/latlng.hpp"

namespace OpenEarth {
        class Transform {
        private:
            glm::mat4 mModelMatrix;
            glm::mat4 mViewMatrix;
            glm::mat4 mProjectMatrix;

            glm::mat4 inverseVPProject;
        public:
            Transform(glm::mat4 modelMatrix,glm::mat4 viewMatrix,glm::mat4 projMatrix);
            ~Transform();
            /**
             * 地图坐标转屏幕坐标
             * @param latLng
             * @return
             */
             glm::vec2 latLngToScreenPoint(LatLng* latLng);

            //屏幕坐标转地图坐标
             LatLng* screenPointToLatlng(glm::vec2 point);

        };
}

#endif //OPENEARTH_TRANSFORM_HPP

//
// 球体大小，位置，倾斜度变换
// reated by GXSN-Pro on 2017/12/13.
//

#ifndef OPENEARTH_TRANSFORM_HPP
#define OPENEARTH_TRANSFORM_HPP

#include <glm/vec2.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "geometry/latlng.hpp"
#include "opengl_project.hpp"
#include <memory>

namespace OpenEarth {
        class Transform {
        private:
            glm::mat4 mModelMatrix;
            glm::mat4 mInverseModelMatrix;
            std::shared_ptr<OpenEarth::OpenGLProject> mProject;
            glm::vec2 mScreenSize;
        public:
            Transform(glm::mat4 modelMatrix,std::shared_ptr<OpenEarth::OpenGLProject> glProject);
            ~Transform();
            void setModelMatrix(glm::mat4 modelMatrix);
            void setProject(std::shared_ptr<OpenEarth::OpenGLProject> glProject);
            /**
             * 地图坐标转屏幕坐标
             * @param latLng
             * @return
             */
             glm::vec2 latLngToScreenPoint(LatLng* latLng);

            //屏幕坐标转地图坐标
            glm::vec2 screenPointToLatlng(glm::vec2 point);
            //屏幕坐标转到球面上的世界坐标
            glm::vec3 screenPointToWorld(glm::vec2 point);
            //球面上的世界坐标转为经纬度
            glm::vec2 worldToLatlng(glm::vec3 world);

            float distanceBetween(Ray* ray,glm::vec3 point);

            static bool isValidWorldCoordinate(glm::vec3 world);
            static bool isValidLatlng(glm::vec2 latlng);



        };
}

#endif //OPENEARTH_TRANSFORM_HPP

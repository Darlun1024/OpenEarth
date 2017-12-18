//
// Created by GXSN-Pro on 2017/12/14.
//

#ifndef OPENEARTH_OPENGL_PROJECT_HPP
#define OPENEARTH_OPENGL_PROJECT_HPP

#include <cstdint>
#include <glm/vec2.hpp>
#include <bits/unique_ptr.h>
#include <glm/matrix.hpp>
#include "geometry/ray.hpp"

using namespace OpenEarth::Geometry;
namespace OpenEarth {
    class OpenGLProject {
    private:
        glm::vec2  mScreenSize;
        glm::mat4 mViewMatrix;
        glm::mat4 mProjectMatrix;
        glm::mat4 mVPMatrix;
        glm::mat4 mInserseVPMatrix;
        glm::vec4 mViewPort;
    public:
        OpenGLProject(glm::mat4 viewMat,glm::mat4 projMat,glm::vec2 screenSize);
        ~OpenGLProject();


        void setProjectMatrix(glm::mat4 projMat);
        void setViewMatrix(glm::mat4 viewMat);
        //设置屏幕大小
        void setScreenSize(glm::vec2 size);
        //获取屏幕大小
        glm::vec2 getScreenSize();

        //世界坐标转屏幕坐标
        glm::vec2 project(glm::vec3);

        //屏幕坐标转在某个深度的世界坐标
        glm::vec3 unProject(glm::vec2 screenPoint, float depth);

        Ray* screen2Ray(glm::vec2 screenPoint);

    private:

    };

}
#endif //OPENEARTH_OPENGL_PROJECT_HPP

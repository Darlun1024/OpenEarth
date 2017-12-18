//
// Created by GXSN-Pro on 2017/12/14.
//

#include "opengl_project.hpp"
#include <glm/ext.hpp>
#include "../logging.hpp"

OpenEarth::OpenGLProject::OpenGLProject(glm::mat4 viewMat, glm::mat4 projMat,
                                        glm::vec2 screenSize) {
    this->mViewMatrix      = viewMat;
    this->mProjectMatrix   = projMat;
    this->mScreenSize      = screenSize;
    this->mVPMatrix        = mProjectMatrix * mViewMatrix;
    this->mInserseVPMatrix = glm::inverse(mVPMatrix);
    this->mScreenSize      = screenSize;
    this->mViewPort        = glm::vec4(0, 0, screenSize);
}

OpenEarth::OpenGLProject::~OpenGLProject() {}
void OpenEarth::OpenGLProject::setScreenSize(glm::vec2 size) {
    this->mScreenSize = size;
    this->mViewPort = glm::vec4(0, 0, size);
}

void OpenEarth::OpenGLProject::setProjectMatrix(glm::mat4 projMat){
    this->mProjectMatrix = projMat;
    this->mVPMatrix        = mProjectMatrix * mViewMatrix;
    this->mInserseVPMatrix = glm::inverse(mVPMatrix);
}

void OpenEarth::OpenGLProject::setViewMatrix(glm::mat4 viewMat){
    this->mViewMatrix      = viewMat;
    this->mVPMatrix        = mProjectMatrix * mViewMatrix;
    this->mInserseVPMatrix = glm::inverse(mVPMatrix);
}

glm::vec2 OpenEarth::OpenGLProject::getScreenSize() {
    return mScreenSize;

}

//世界坐标转屏幕坐标
glm::vec2 OpenEarth::OpenGLProject::project(glm::vec3 worldPoint) {
    glm::vec4 worldPoint_w = glm::vec4(worldPoint, 1.0f);
    glm::vec4 normalizeScreenPoint = mVPMatrix * worldPoint_w;
    glm::vec2 screenPoint;
    screenPoint[0] = (normalizeScreenPoint[0]+1)*mScreenSize[0]*0.5;
    screenPoint[1] = (1-normalizeScreenPoint[1])*mScreenSize[1]*0.5;
    return glm::vec2(screenPoint);
}


glm::vec3 OpenEarth::OpenGLProject::unProject(glm::vec2 screenPoint, float depth) {
    return glm::unProject(glm::vec3(screenPoint[0],mScreenSize[1]-screenPoint[1], depth), this->mViewMatrix, this->mProjectMatrix,this->mViewPort);
}

//屏幕坐标转为一条射线，世界坐标是三维的，而屏幕坐标是二维的，因此屏幕上的一个点，在世界坐标系中代表一条线
Ray* OpenEarth::OpenGLProject::screen2Ray(glm::vec2 screenPoint){
    //先计算归一化设备坐标
    glm::vec3 vecNear =  glm::unProject(glm::vec3(screenPoint[0],mScreenSize[1] -screenPoint[1], -1.0f), this->mViewMatrix, this->mProjectMatrix,this->mViewPort);
    glm::vec3 vecFar  =  glm::unProject(glm::vec3(screenPoint[0],mScreenSize[1]-screenPoint[1], 1.0f), this->mViewMatrix, this->mProjectMatrix,this->mViewPort);
    //屏幕的y轴正方向和世界坐标的y轴正方向是相反的

    glm::vec3 vect = vecFar - vecNear;
    Ray* ray = new Ray(vecNear,vect);
    return  ray;
}
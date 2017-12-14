//
// Created by GXSN-Pro on 2017/12/14.
//

#include "opengl_project.hpp"

namespace OpenEarth{

    void OpenEarth::OpenGLProject::setScreenSize(glm::vec2 size){
        this->mScreenSize = size;
    }

    glm::vec2 OpenEarth::OpenGLProject::getScreenSize(){
        return mScreenSize;
    }

    //世界坐标转屏幕坐标
    glm::vec2 OpenEarth::OpenGLProject::project(glm::vec3){

    }

    //屏幕坐标转世界坐标
    glm::vec3 OpenEarth::OpenGLProject::unProject(glm::vec2 ,float depth){

    }

}
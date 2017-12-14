//
// Created by GXSN-Pro on 2017/12/13.
//

#include "transform.hpp"
#include "opengl_project.hpp"


OpenEarth::Transform::Transform(glm::mat4 modelMatrix,glm::mat4 viewMatrix,glm::mat4 projMatrix){
    this->mModelMatrix   = modelMatrix;
    this->mViewMatrix    = viewMatrix;
    this->mProjectMatrix = projMatrix;
}

OpenEarth::Transform::~Transform(){

}

glm::vec2 OpenEarth::Transform::latLngToScreenPoint(LatLng* latLng){


}


LatLng* OpenEarth::Transform::screenPointToLatlng(glm::vec2 point){
    glm::vec3 ray;

}



//
// Created by GXSN-Pro on 2017/12/13.
//

#include "transform.hpp"
#include "opengl_project.hpp"
#include "earth.hpp"


OpenEarth::Transform::Transform(glm::mat4 modelMatrix,glm::mat4 viewMatrix,glm::mat4 projMatrix){
    this->mModelMatrix   = modelMatrix;
    this->mViewMatrix    = viewMatrix;
    this->mProjectMatrix = projMatrix;
}

OpenEarth::Transform::~Transform(){

}

glm::vec2 OpenEarth::Transform::latLngToScreenPoint(LatLng* latLng){
    float R = OpenEarth::Earth::getRadius();
    float scale  = OpenEarth::Earth::getScale();
    //先转为地球坐标
    float y = (float) (R * sin(latLng->lat));
    float x = (float) (R * cos(latLng->lat) * sin(latLng->lon));
    float z = (float) (R * cos(latLng->lat) * cos(latLng->lon));

    glm::vec4 origin = glm::vec4(x,y,z,1.0f);
    glm::vec4 world  = this->mModelMatrix * origin;
    //TODO
}


LatLng* OpenEarth::Transform::screenPointToLatlng(glm::vec2 point){
    glm::vec3 ray;

}



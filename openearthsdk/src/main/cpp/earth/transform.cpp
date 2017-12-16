//
// Created by GXSN-Pro on 2017/12/13.
//

#include "transform.hpp"
#include "earth.hpp"


OpenEarth::Transform::Transform(glm::mat4 modelMatrix,std::shared_ptr<OpenEarth::OpenGLProject> glProject){
    this->mModelMatrix   = modelMatrix;
    this->mProject = glProject;
}

OpenEarth::Transform::~Transform(){

}

glm::vec2 OpenEarth::Transform::latLngToScreenPoint(LatLng* latLng){
    float R      = OpenEarth::Earth::getRadius();
//    float scale  = OpenEarth::Earth::getScale(); //scale
    //先转为地球坐标
    float y = (float) (R * sin(latLng->lat));
    float x = (float) (R * cos(latLng->lat) * sin(latLng->lon));
    float z = (float) (R * cos(latLng->lat) * cos(latLng->lon));

    glm::vec4 origin = glm::vec4(x,y,z,1.0f);
    glm::vec4 world  = this->mModelMatrix * origin; //通过模型矩阵，转为世界坐标
    //TODO
    glm::vec2 screen = mProject->project(glm::vec3(world));
}


glm::vec2 OpenEarth::Transform::screenPointToLatlng(glm::vec2 point){
    glm::vec3 ray;

}



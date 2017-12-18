//
// Created by GXSN-Pro on 2017/12/18.
//

#include "ray.hpp"
#include <math.h>

OpenEarth::Geometry::Ray::Ray(glm::vec3 point,glm::vec3 vector){

}

OpenEarth::Geometry::Ray::~Ray(){

}

float OpenEarth::Geometry::Ray::length(){
    return sqrt(mVector[0]*mVector[0]+mVector[1]*mVector[1]+mVector[2]*mVector[2]);
}
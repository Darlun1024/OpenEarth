//
// Created by GXSN-Pro on 2017/12/21.
//

#include <glm/detail/func_geometric.inl>
#include "geometry_util.hpp"
using namespace OpenEarth::Geometry;

 glm::vec3 GeometryUtil::projectToPlane(glm::vec3 vector,glm::vec3 plane){
     float a = glm::dot(vector,plane)/glm::length(plane);
     return vector - plane * a;
}
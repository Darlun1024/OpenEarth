//
// Created by GXSN-Pro on 2017/12/13.
//

#include "transform.hpp"
#include "earth.hpp"
#include "geometry/ray.hpp"
#include "../logging.hpp"

using namespace OpenEarth::Geometry;

float distanceBetween(Ray* ray,glm::vec3 point);

OpenEarth::Transform::Transform(glm::mat4 modelMatrix,std::shared_ptr<OpenEarth::OpenGLProject> glProject){
    this->mModelMatrix         = modelMatrix;
    this->mInverseModelMatrix  = glm::inverse(modelMatrix);
    this->mProject = glProject;
}

OpenEarth::Transform::~Transform(){

}

void OpenEarth::Transform::setModelMatrix(glm::mat4 modelMatrix){
    this->mModelMatrix         = modelMatrix;
    this->mInverseModelMatrix  = glm::inverse(modelMatrix);
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
    Ray* ray = mProject->screen2Ray(point);
    glm::vec3 earthCenter = OpenEarth::Earth::getCenter();
    //通过模型矩阵转换为世界坐标
    glm::vec4 center = mModelMatrix * glm::vec4(earthCenter,1.0f);
    float distanceEarthCenterToRay = distanceBetween(ray,glm::vec3(center[0],center[1],center[2]));
    float R = OpenEarth::Earth::getRadius()*OpenEarth::Earth::getScale(); //经过模型矩阵转换后地球的半径

     if(R < distanceEarthCenterToRay){ //不相交
         return glm::vec2(-181,-181); //返回一个非法的坐标
     }else{
        //求解射线和地球的交点
         //已经球心到线的距离distanceEarthCenterToRay，已经球体半径 R
         //求交点与垂足的距离
         float dist = sqrt(R*R - distanceEarthCenterToRay*distanceEarthCenterToRay);
         //射线的长度
         float rayLength  = glm::length(ray->mVector); //求ray的长度
         //求圆心在向量上的投影
         glm::vec3 rayStartToCenter = glm::vec3(center[0]-ray->mPoint[0],center[1]-ray->mPoint[1],center[2]-ray->mPoint[2]);
         float a  = glm::dot(rayStartToCenter,ray->mVector)/rayLength; //求两个向量的点积
         glm::vec3 c = ray->mPoint +  a/rayLength * ray->mVector; //球心到射线的垂足

         glm::vec3 p1 = c + dist/rayLength * ray->mVector;
         glm::vec3 p0 = c - dist/rayLength * ray->mVector; //p0是较近的一点
         //反转模型矩阵，求出原始的球体坐标
         glm::vec4 p11 = mInverseModelMatrix * glm::vec4(p1,1.0f);
         glm::vec4 p00 = mInverseModelMatrix * glm::vec4(p0,1.0f);
         //转为经纬度
         float lat = asinf(p00[1]/R); // >0 north; <0 south
//         x1 = (float) (R * cos(latR1) * sin(lonR1));
//         z1 = (float) (R * cos(latR1) * cos(lonR1));
         float cosLat = R*cos(lat); //cos(lat)>=0;
         float lon = asin(p00[0]/cosLat); //大于0 (0-180)  小于0 (180-360)
         if(lon > 0){
             if(p00[2] < 0) lon= M_PI - lon;
         }

         if(lon < 0){
             if(p00[2] > 0)lon = -M_PI - lon;
         }
         float latD = 180*lat/M_PI;
         float lonD = 180*lon/M_PI;
         LOGE("transform","%f,%f,%f,%f,%f",p00[0],p00[1],p00[2],latD,lonD);
         return glm::vec2(latD,lonD);
     }

}

/**
 * 计算射线和某个点的距离
 * @param ray
 * @param point
 */
float distanceBetween(Ray* ray,glm::vec3 point){
    //用线的起点和终点分别于 第三个点构建向量
    glm::vec3 vect1 = ray->mPoint - point;
    glm::vec3 vect2 = ray->mPoint + ray->mVector - point;
    //求两个向量的叉积
    //两个向量的叉积，这个向量的长度恰好是前两个向量定义的三角形的面积的两倍
    glm::vec3 cross = glm::cross(vect1,vect2);
    float areaOf2Triangle =  glm::length(cross);
    //向量的长度
    float rayLength = ray->length();
    //求出向量与点的距离，三角形的高
    float distancePointToRay = areaOf2Triangle/rayLength;
    return  distancePointToRay;
}








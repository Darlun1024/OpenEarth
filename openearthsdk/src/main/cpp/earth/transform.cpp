//
// Created by GXSN-Pro on 2017/12/13.
//

#include "transform.hpp"
#include "earth.hpp"
#include "geometry/ray.hpp"
#include "../util/util.hpp"

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
    //转为弧度
    float lat = OpenEarth::Util::degree2Rad(latLng->lat);
    float lon = OpenEarth::Util::degree2Rad(latLng->lon);
    //先转为地球坐标
    float y = R * sin(lat);
    float x = R * cos(lat) * sin(lon);
    float z = R * cos(lat) * cos(lon);

    glm::vec4 origin = glm::vec4(x,y,z,1.0f);
    glm::vec4 world  = this->mModelMatrix * origin; //通过模型矩阵，转为世界坐标
    //TODO
    glm::vec2 screen = mProject->project(glm::vec3(world));
    return screen;
}

/**
 * 屏幕坐标转为经纬度，只返回离屏幕较近的一点的坐标
 * 纬度范围[-90 90] 经度 [0-360]
 * @param point
 * @return
 */
glm::vec2 OpenEarth::Transform::screenPointToLatlng(glm::vec2 point){
    Ray* ray = mProject->screen2Ray(point); //先构建一条射线
    glm::vec3 earthCenter = OpenEarth::Earth::getCenter(); //获取球心
    glm::vec4 center = mModelMatrix * glm::vec4(earthCenter,1.0f);//通过模型矩阵将球心转换为世界坐标 这样球心和射线就处于同一套坐标系中
    float distanceEarthCenterToRay = distanceBetween(ray,glm::vec3(center[0],center[1],center[2]));//球心到射线的距离
    float R = OpenEarth::Earth::getRadius()*OpenEarth::Earth::getScale(); //经过模型矩阵转换后地球的半径
    float r = OpenEarth::Earth::getRadius();
     if(R < distanceEarthCenterToRay){ //不相交
         return glm::vec2(MAXFLOAT,MAXFLOAT); //返回一个非法的坐标
     }else{
         float dist = sqrt(R*R - distanceEarthCenterToRay*distanceEarthCenterToRay);//球心与射线构成的三角形的底边的一半长度
         float rayLength  = glm::length(ray->mVector); //求射线的长度
         //求圆心在向量上的投影
         glm::vec3 rayStartToCenter = glm::vec3(center[0]-ray->mPoint[0],center[1]-ray->mPoint[1],center[2]-ray->mPoint[2]);
         float a     = glm::dot(rayStartToCenter,ray->mVector)/rayLength; //求圆心投影在射线上的长度
         glm::vec3 vectStartToFoootPoint = ray->mPoint +  a/rayLength * ray->mVector; //射线起点到垂足的向量

         glm::vec3 p1 = vectStartToFoootPoint + dist/rayLength * ray->mVector; //远交点
         glm::vec3 p0 = vectStartToFoootPoint - dist/rayLength * ray->mVector; //近交点
         //反转模型矩阵，求出原始的球体坐标0是较近的一点
         glm::vec4 pFar  = mInverseModelMatrix * glm::vec4(p1,1.0f);
         glm::vec4 pNear = mInverseModelMatrix * glm::vec4(p0,1.0f);
         //转为经纬度
         float lat    = asinf(pNear[1]/r); //lat [-pi/2, pi/2]
         float cosLat = r*cos(lat);     //cos(lat)>=0;
         float lon    = asin(pNear[0]/cosLat); //lon
         if(lon >= 0){
             if(pNear[2] < 0) lon = M_PI - lon;
         }else if(lon < 0){
             if(pNear[2] < 0) lon = M_PI - lon;
             if(pNear[2] > 0) lon = M_PI*2 + lon;
         }
         float latD = 180 * lat/M_PI;
         float lonD = 180 * lon/M_PI;
         return glm::vec2(latD,lonD);
     }

}

/**
 * 计算射线和某个点的距离
 * @param ray
 * @param point
 */
float OpenEarth::Transform::distanceBetween(Ray* ray,glm::vec3 point){
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


glm::vec3 OpenEarth::Transform::screenPointToWorld(glm::vec2 point){
    Ray* ray = mProject->screen2Ray(point); //先构建一条射线
    glm::vec3 earthCenter = OpenEarth::Earth::getCenter(); //获取球心
    glm::vec4 center = mModelMatrix * glm::vec4(earthCenter,1.0f);//通过模型矩阵将球心转换为世界坐标 这样球心和射线就处于同一套坐标系中
    float distanceEarthCenterToRay = distanceBetween(ray,glm::vec3(center[0],center[1],center[2]));//球心到射线的距离
    float R = OpenEarth::Earth::getRadius()*OpenEarth::Earth::getScale(); //经过模型矩阵转换后地球的半径
    if(R < distanceEarthCenterToRay){ //不相交
        return glm::vec3(MAXFLOAT,MAXFLOAT,MAXFLOAT); //返回一个非法的坐标
    }else{
        float dist = sqrt(R*R - distanceEarthCenterToRay*distanceEarthCenterToRay);//球心与射线构成的三角形的高，球心到射线的距离
        float rayLength  = glm::length(ray->mVector); //求射线的长度
        //求圆心在向量上的投影
        glm::vec3 rayStartToCenter = glm::vec3(center[0]-ray->mPoint[0],center[1]-ray->mPoint[1],center[2]-ray->mPoint[2]);
        float a     = glm::dot(rayStartToCenter,ray->mVector)/rayLength; //求圆心投影在射线上的长度
        glm::vec3 vectStartToFoootPoint = ray->mPoint +  a/rayLength * ray->mVector; //射线起点到垂足的向量

//        glm::vec3 p1 = vectStartToFoootPoint + dist/rayLength * ray->mVector; //远交点
        glm::vec3 p0 = vectStartToFoootPoint - dist/rayLength * ray->mVector; //近交点
        return  p0;
    }
}

glm::vec2 OpenEarth::Transform::worldToLatlng(glm::vec3 world){
    float R = OpenEarth::Earth::getRadius()*OpenEarth::Earth::getScale();
    glm::vec4 point  = mInverseModelMatrix * glm::vec4(world,1.0f);
    //转为经纬度
    float lat = asinf(point[1]/R); //lat [-pi/2, pi/2]
    float cosLat = R*cos(lat);     //cos(lat)>=0;
    float lon = asin(point[0]/cosLat); //lon
    if(lon >= 0){
        if(point[2] < 0) lon= M_PI - lon;
    }else if(lon < 0){
        if(point[2] < 0) lon= M_PI - lon;
        if(point[2] > 0) lon = M_PI*2 + lon;
    }
    float latD = 180 * lat/M_PI;
    float lonD = 180 * lon/M_PI;
    return glm::vec2(latD,lonD);
}

bool OpenEarth::Transform::isValidWorldCoordinate(glm::vec3 world){
    return world[0]<MAXFLOAT && world[1]<MAXFLOAT && world[2]<MAXFLOAT;
}

bool OpenEarth::Transform::isValidLatlng(glm::vec2 latlng){
    return (-90<=latlng[0])&&(90>=latlng[0])&&(0<=latlng[1])&&(360>=latlng[1]);
}







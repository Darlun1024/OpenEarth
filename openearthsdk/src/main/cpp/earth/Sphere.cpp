//
// Created by GXSN-Pro on 2017/11/15.
//

#include "sphere.hpp"
#include "../logging.hpp"
#include <math.h>
#include <android/log.h>


float  R; //球体半径
float*  vertexArray;

const float step = 1.0f;

OpenEarth::Sphere::Sphere(float radius){
    R = radius;
    generateVertexArray();
}

OpenEarth::Sphere::~Sphere(){
    if(vertexArray){
        delete  vertexArray;
    }
}

float dtor(float d){
    return M_PIl*d/180;
}

//生成顶点
void OpenEarth::Sphere::generateVertexArray(){
    int size = (int) ((180 / step + 1) * (360 / step + 1));
    vertexArray  = new GLfloat[size];
    int index = 0;
    for(float lat=90; lat >= -90; lat -= step){
        float latR = dtor(lat);
        for(float lon = 0; lon <= 360;lon += step){
            float lonR = dtor(lon);
            double y = R*sin(latR);
            double x = R*cos(latR)*sin(lonR);
            double z = R*cos(latR)*cos(lonR);
            LOGE("sphere","%f,%f,%f",x,y,z);
            vertexArray[index++] = x;
            vertexArray[index++] = y;
            vertexArray[index++] = z;
        }
    }
}


void draw(){

}



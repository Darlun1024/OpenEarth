//
// Created by GXSN-Pro on 2017/11/15.
//

#include "Sphere.hpp"
#include "../logging.hpp"
#include <math.h>
#include <android/log.h>


float  R; //球体半径
float*  vertexArray;
int vertextSize = 0;

const float step = 1.0f;  //必须可以被90°整除

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
    return M_PI*d/180;
}

//生成顶点
void OpenEarth::Sphere::generateVertexArray(){
    int size = (int) ((180 /step) * (360 / step));
    vertextSize = size*2*3;
    vertexArray  = new GLfloat[vertextSize];
    int index = 0;
    float x1,x2,y1,y2,z1,z2;
    for(float lat=90; lat > -90; lat -= step){
        float latR1 = dtor(lat);
        float latR2 = dtor(lat-step);
        y1 = (float) (R * sin(latR1));
        y2 = (float) (R * sin(latR2));
        for(float lon = 0; lon < 360;lon += step){
            float lonR1 = dtor(lon);
            float lonR2 = dtor(lon+step);
            x1 = (float) (R * cos(latR1) * sin(lonR1));
            z1 = (float) (R * cos(latR1) * cos(lonR1));
            x2 = (float) (R * cos(latR2) * sin(lonR2));
            z2 = (float) (R * cos(latR2) * cos(lonR2));
            LOGE("sphere","%f,%f,%f",x1,y1,z1);
            vertexArray[index++] = x1;
            vertexArray[index++] = y1;
            vertexArray[index++] = z1;
            vertexArray[index++] = x2;
            vertexArray[index++] = y2;
            vertexArray[index++] = z2;
        }
    }
    LOGE("===","%d",index);
}

GLfloat* OpenEarth::Sphere::getVertexArray(){
    return vertexArray;
}
int OpenEarth::Sphere::getVertextSize(){
    return vertextSize/3;
}
void draw(){

}



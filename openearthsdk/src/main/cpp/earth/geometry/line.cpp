//
// Created by GXSN-Pro on 2018/1/24.
//

#include "line.hpp"
#include<cstring>
#import <android/log.h>
#import "../earth.hpp"

using namespace OpenEarth::Geometry;
static const double earth_radius = 6378000; //地球半径，单位米

void Line::setCoordinates(const LatLng* points,long count) {
    this->lineString = new LatLng[count];
    memcpy(this->lineString,points,(size_t)count);
    this->pointCount = count;
    generateVertexAttribArray();
}

void  Line::generateVertexAttribArray(){
    float R = OpenEarth::Earth::getRadius();
    __android_log_print(ANDROID_LOG_INFO, "Line", "little R : %f/n", R);
    R = (float) ((1 + earth_radius) * R / earth_radius);
    vertextAttribArrary = new float[pointCount*3];
    for(int i=0; i < pointCount;i++){
        LatLng latLng = this->lineString[i];
        double latR = latLng.lat * M_PI / 180.0;
        double lonR = latLng.lon * M_PI / 180.0;
        float x = (float) (R * cos(latR) * sin(lonR));
        float y = (float) (R * sin(latR));
        float z = (float) (R * cos(latR) * cos(lonR));
        vertextAttribArrary[i*3] = x;
        vertextAttribArrary[i*3+1] = y;
        vertextAttribArrary[i*3+2] = z;
    }
}

float* Line::getVertexAttribArray(long *count) const{
    *count = this->pointCount;
    return this->vertextAttribArrary;
}

LatLng* Line::getCoordinates() const{
    return  this->lineString;
}

long Line::getPointCount() const{
    return this->pointCount;
}

Line::Line() {

}

Line::~Line(){
    if(this->pointCount>0){
        delete[](this->lineString);
    }
}

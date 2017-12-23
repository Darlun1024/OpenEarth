//
// Created by GXSN-Pro on 2017/12/13.
//

#include "latlng.hpp"

LatLng::LatLng(float lat, float lon) {
    this->lat = lat;
    this->lon = lon;
}

LatLng::~LatLng() {

}

void LatLng::normalize() {
    int n;
    if(this->lon > 180){
        n = (this->lon+180)/360;
        this->lon = this->lon - n*360;
    }

    if(this->lon < -180){
        n = (180-this->lon)/360;
        this->lon= this->lon + 360*n;
    }

    if(this->lat > 270){
        n = (this->lat+90)/360;
        this->lat = this->lat - n*360;
    }

    if(this->lat < -90){
        n = (270-this->lat)/360;
        this->lat = this->lat + n*360;
    }
    this->lat = 90-fabsf(this->lat-90);
}
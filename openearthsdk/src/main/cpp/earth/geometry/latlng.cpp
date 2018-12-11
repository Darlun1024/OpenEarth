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
        n = (int) ((this->lon + 180) / 360);
        this->lon = this->lon - n*360;
    }

    if(this->lon < -180){
        n = (int) ((180 - this->lon) / 360);
        this->lon= this->lon + 360*n;
    }

    if(this->lat > 270){
        n = (int) ((this->lat + 90) / 360);
        this->lat = this->lat - n*360;
    }

    if(this->lat < -90){
        n = (int) ((270 - this->lat) / 360);
        this->lat = this->lat + n*360;
    }
    this->lat = 90-fabsf(this->lat-90);
}
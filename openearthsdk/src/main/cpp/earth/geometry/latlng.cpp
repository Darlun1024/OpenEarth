//
// Created by GXSN-Pro on 2017/12/13.
//

#include "latlng.hpp"
LatLng::LatLng(){
    this->lat = 0;
    this->lon = 0;
}
LatLng::LatLng(double lat, double lon) {
    this->lat = lat;
    this->lon = lon;
}

LatLng::LatLng(const LatLng& latlng){
    this->lat = latlng.lat;
    this->lon = latlng.lon;
}

LatLng& LatLng::operator = (const LatLng &latlng){
        if(this == &latlng){
            return *this;
        }
        this->lon = latlng.lon;
        this->lat = latlng.lat;
    return *this;
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
    this->lat = 90-fabs(this->lat-90);
}
// 经纬度
// reated by GXSN-Pro on 2017/12/14.
//
#pragma once

#ifndef OPENEARTH_LATLNG_HPP
#define OPENEARTH_LATLNG_HPP


#include <cmath>


class LatLng {

public:
    float lat;
    float lon;

    LatLng(float lat, float lon);
    ~LatLng();
    /** 标注化坐标 */
    void normalize();


};


#endif //OPENEARTH_LATLNG_HPP

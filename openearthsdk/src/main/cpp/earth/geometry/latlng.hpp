// 经纬度
// reated by GXSN-Pro on 2017/12/14.
//
#pragma once

#ifndef OPENEARTH_LATLNG_HPP
#define OPENEARTH_LATLNG_HPP


#include <cmath>

        class LatLng {

        public:
            double lat;
            double lon;

            LatLng();
            LatLng(double lat, double lon);
            LatLng(const LatLng& latlng);
            LatLng &operator= (const LatLng &latlng);
            ~LatLng();

            /** 标注化坐标 */
            void normalize();
        };

#endif //OPENEARTH_LATLNG_HPP

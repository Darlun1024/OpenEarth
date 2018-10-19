//
// turf.js C++ 版本
// Created by GXSN-Pro on 2018/10/17.
//
//

#ifndef OPENEARTH_TURF_HPP
#define OPENEARTH_TURF_HPP

#include "../point.hpp"
#include "../latlng.hpp"

using namespace OpenEarth::Geometry;
namespace OpenEarth{
    namespace Geometry {
        class Turf {
        public:
            static  double distance(LatLng* from,LatLng* to);
            static  double degreesToRadians(double degree);
            static  double radiansToDegrees(double rad);
        };
    }
}


#endif //OPENEARTH_TURF_HPP

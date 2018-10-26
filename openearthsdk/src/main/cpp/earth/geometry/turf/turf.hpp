//
// turf.js C++ 版本
// Created by GXSN-Pro on 2018/10/17.
//
//

#ifndef OPENEARTH_TURF_HPP
#define OPENEARTH_TURF_HPP

#include "../point.hpp"
#include "../latlng.hpp"
#import <vector>

using namespace OpenEarth::Geometry;
namespace OpenEarth{
    namespace Geometry {
        class Turf {
        public:
            /**计算两点之间的距离*/
            static  double distance(Point* from,Point* to);
            /**计算给定点串构成的多边形的面积*/
            static  double area(std::vector<Point*>* points);
            /**角度转弧度*/
            static  double degreesToRadians(double degree);
            /**弧度转角度*/
            static  double radiansToDegrees(double rad);
            /**计算方位角*/
            static  double bearing(Point* start,Point* end);

            static double
        };
    }
}


#endif //OPENEARTH_TURF_HPP

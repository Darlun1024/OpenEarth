//
// turf.js C++ 版本
// Created by GXSN-Pro on 2018/10/17.
//
//

#ifndef OPENEARTH_TURF_HPP
#define OPENEARTH_TURF_HPP

#include "../latlng.hpp"
#include "envelope.hpp"
#import <vector>

using namespace OpenEarth::Geometry;
namespace OpenEarth{
    namespace Geometry {
        class SphereUtil {
        public:
            /**计算两点之间的距离*/
            static  double distance(const LatLng &from,const LatLng &to);
            /**计算给定点串构成的多边形的面积*/
            static  double area(std::vector<LatLng*>* points);
            /**角度转弧度*/
            static  double degreesToRadians(double degree);
            /**弧度转角度*/
            static  double radiansToDegrees(double rad);
            /**计算方位角*/
            static  double bearing(const LatLng &start,const  LatLng &end);
            /**最小包围矩形*/
            static Envelope envelope(std::vector<LatLng*>* points);

            /***
             * 在两点之间的球面上做插值，
             * @param from 起点坐标
             * @param to  终点坐标
             * @param fraction 系数
             * @return
             */
            static LatLng  interpolate(const LatLng &from, const LatLng &to, double fraction);

            /***
             * 计算两点之间的大圆夹角
             * @param from
             * @param to
             * @return
             */
            static double computeAngleBetween(const LatLng &from, const LatLng &to);

        private:
            static double distanceRadians(double lat1, double lng1, double lat2, double lng2);

            static double polarTriangleArea(double tan1, double lng1, double tan2, double lng2);
        };
    }
}


#endif //OPENEARTH_TURF_HPP

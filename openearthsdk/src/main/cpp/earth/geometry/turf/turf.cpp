//
// Created by GXSN-Pro on 2018/10/17.
//

#include "turf.hpp"
#include <cmath>
#include <boost/range/size_type.hpp>

#define earth_radius  6371008.8

#define MAX_COORDINATE_VALUE 1000000
#define MIN_COORDINATE_VALUE -1000000

using namespace OpenEarth::Geometry;

  double Turf::degreesToRadians(double degree){
    return degree * M_PI / 180.0;
}
  double Turf::radiansToDegrees(double rad){
    return rad * 180.0 / M_PI;
}
//function distance(from, to, options) {
//    // Optional parameters
//    options = options || {};
//    if (!isObject(options)) throw new Error('options is invalid');
//    var units = options.units;
//
//    var coordinates1 = getCoord(from);
//    var coordinates2 = getCoord(to);
//    var dLat = degreesToRadians((coordinates2[1] - coordinates1[1]));
//    var dLon = degreesToRadians((coordinates2[0] - coordinates1[0]));
//    var lat1 = degreesToRadians(coordinates1[1]);
//    var lat2 = degreesToRadians(coordinates2[1]);
//
//    var a = Math.pow(Math.sin(dLat / 2), 2) +
//            Math.pow(Math.sin(dLon / 2), 2) * Math.cos(lat1) * Math.cos(lat2);
//
//    return radiansToLength(2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a)), units);
//}
/**
 * 计算两点间的球面距离(单位 m)
 * @param from
 * @param to
 * */
double Turf::distance(Point* from,Point* to){
    double dLat = degreesToRadians(to->lat - from->lat);
    double dLon = degreesToRadians(to->lon - from->lon);
    double lat1 = degreesToRadians(from->lat);
    double lat2 = degreesToRadians(to->lat);
    double a = pow(sin(dLat/2),2) + pow(sin(dLon/2),2) * cos(lat1)*cos(lat2);
    double rad = 2*atan2(sqrt(a),sqrt(1-a));
    return rad * earth_radius;
}

/**
 * 算法来自OpenLayers
 * */
double Turf::area(std::vector<Point*>* points){
    if(points == nullptr)return 0.0;
    if(points->size() < 3) return  0.0;
    double area = 0.0;
    unsigned long length = points->size();
    for(unsigned long i = 0; i < length-1; i++){
        Point* point1 = points->at(i);
        Point* point2 = points->at(i+1);
        area += degreesToRadians(point2->lon - point1->lon) * (2 + sin(degreesToRadians(point1->lat))+sin(degreesToRadians(point2->lat)));
    }
    area = area * earth_radius * earth_radius / 2.0;
    return area;
}

double Turf::bearing(Point* start,Point* end){
    double lon1 = degreesToRadians(start->lon);
    double lon2 = degreesToRadians(end->lon);
    double lat1 = degreesToRadians(start->lat);
    double lat2 = degreesToRadians(end->lat);
    double a = sin(lon2 - lon1) * cos(lat2);
    double b = cos(lat1) * sin(lat2) -
            sin(lat1) * cos(lat2) * cos(lon2 - lon1);
    return radiansToDegrees(atan2(a, b));
}

Envelope* Turf::envelope(std::vector<Point*>* points){
    if(points == nullptr) {
        return new Envelope(0,0,0,0);
    }
    double minLon = MAX_COORDINATE_VALUE;
    double minLat = MAX_COORDINATE_VALUE;
    double maxLon = MIN_COORDINATE_VALUE;
    double maxLat = MIN_COORDINATE_VALUE;
    u_long length = points->size();
    for(u_long i = 0; i < length; i++){
        Point* point = points->at(i);
        minLon = fmin(minLon,point->lon);
        minLat = fmin(minLat,point->lat);
        maxLon = fmax(maxLon,point->lon);
        maxLat = fmax(maxLat,point->lat);
    }
    return new Envelope(minLon,minLat,maxLon,maxLat);
}

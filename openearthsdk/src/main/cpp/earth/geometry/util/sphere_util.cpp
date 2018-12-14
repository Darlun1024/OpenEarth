//
// Created by GXSN-Pro on 2018/10/17.
//

#include "sphere_util.hpp"
#include "math_util.hpp"
#include <cmath>
#include <boost/range/size_type.hpp>

#define earth_radius  6371008.8

#define MAX_COORDINATE_VALUE 1000000
#define MIN_COORDINATE_VALUE -1000000

using namespace OpenEarth::Geometry;

double SphereUtil::degreesToRadians(double degree) {
    return degree * M_PI / 180.0;
}

double SphereUtil::radiansToDegrees(double rad) {
    return rad * 180.0 / M_PI;
}

/**
 * 计算两点间的球面距离(单位 m)
 * @param from
 * @param to
 * */
double SphereUtil::distance(const LatLng &from,const LatLng &to) {
    //先计算两点之间的大圆夹角
    double rad = computeAngleBetween(from,to);
    return rad * earth_radius;
}

/**
 * 计算区域面积
 * google maps
 * */
double SphereUtil::area(std::vector<LatLng *> *points) {
    uint size = points->size();
    if (size < 3) { return 0; }
    double total = 0;
    LatLng *prePoint = points->at(size - 1);
    double prevTanLat = tan((M_PI / 2 - degreesToRadians(prePoint->lat)) / 2);
    double prevLng = degreesToRadians(prePoint->lon);
    for (uint i = 0; i < size; i++) {
        LatLng *point = points->at(i);
        double tanLat = tan((M_PI / 2 - degreesToRadians(point->lat)) / 2);
        double lng = degreesToRadians(point->lon);
        total += polarTriangleArea(tanLat, lng, prevTanLat, prevLng);
        prevTanLat = tanLat;
        prevLng = lng;
    }
    return fabs(total * (earth_radius * earth_radius));
}

double SphereUtil::polarTriangleArea(double tan1, double lng1, double tan2, double lng2) {
    double deltaLng = lng1 - lng2;
    double t = tan1 * tan2;
    return 2 * atan2(t * sin(deltaLng), 1 + t * cos(deltaLng));
}

double SphereUtil::bearing(const LatLng &start,const  LatLng &end) {
    double lon1 = degreesToRadians(start.lon);
    double lon2 = degreesToRadians(end.lon);
    double lat1 = degreesToRadians(start.lat);
    double lat2 = degreesToRadians(end.lat);
    double a = sin(lon2 - lon1) * cos(lat2);
    double b = cos(lat1) * sin(lat2) -
               sin(lat1) * cos(lat2) * cos(lon2 - lon1);
    return radiansToDegrees(atan2(a, b));
}

Envelope  SphereUtil::envelope(std::vector<LatLng *> *points) {
    if (points == nullptr) {
        return  Envelope(0, 0, 0, 0);
    }
    double minLon = MAX_COORDINATE_VALUE;
    double minLat = MAX_COORDINATE_VALUE;
    double maxLon = MIN_COORDINATE_VALUE;
    double maxLat = MIN_COORDINATE_VALUE;
    u_long length = points->size();
    for (u_long i = 0; i < length; i++) {
        LatLng *point = points->at(i);
        minLon = fmin(minLon, point->lon);
        minLat = fmin(minLat, point->lat);
        maxLon = fmax(maxLon, point->lon);
        maxLat = fmax(maxLat, point->lat);
    }
    return  Envelope(minLon, minLat, maxLon, maxLat);
}

LatLng SphereUtil::interpolate(const LatLng &from, const LatLng &to, double fraction) {
    double fromLat = degreesToRadians(from.lat);
    double fromLng = degreesToRadians(from.lon);
    double toLat = degreesToRadians(to.lat);
    double toLng = degreesToRadians(to.lon);
    double cosFromLat = cos(fromLat);
    double cosToLat = cos(toLat);

    // Computes Spherical interpolation coefficients.
    double angle = computeAngleBetween(from, to);
    double sinAngle = sin(angle);
    if (sinAngle < 1E-6) {
         LatLng latlng =  LatLng(
                from.lat + fraction * (to.lat - from.lat),
                from.lon + fraction * (to.lon - from.lon));;
        return latlng;
    }
    double a = sin((1 - fraction) * angle) / sinAngle;
    double b = sin(fraction * angle) / sinAngle;

    // Converts from polar to vector and interpolate.
    double x = a * cosFromLat * cos(fromLng) + b * cosToLat * cos(toLng);
    double y = a * cosFromLat * sin(fromLng) + b * cosToLat * sin(toLng);
    double z = a * sin(fromLat) + b * sin(toLat);

    // Converts interpolated vector back to polar.
    double lat = atan2(z, sqrt(x * x + y * y));
    double lng = atan2(y, x);
    return  LatLng(radiansToDegrees(lat), radiansToDegrees(lng));
}

double SphereUtil::computeAngleBetween(const LatLng &from, const LatLng &to) {
    return distanceRadians(degreesToRadians(from.lat), degreesToRadians(from.lon),
                           degreesToRadians(to.lat), degreesToRadians(to.lon));
}

double SphereUtil::distanceRadians(double lat1, double lng1, double lat2, double lng2) {
    return MathUtil::arcHav(MathUtil::havDistance(lat1, lat2, lng1 - lng2));
}
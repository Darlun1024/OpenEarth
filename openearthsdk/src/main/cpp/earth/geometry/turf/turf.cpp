//
// Created by GXSN-Pro on 2018/10/17.
//

#include "turf.hpp"
#include <cmath>
#define earth_radius  6371008.8
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
double Turf::distance(LatLng* from,LatLng* to){
    double dLat = degreesToRadians(to->lat - from->lat);
    double dLon = degreesToRadians(to->lon - from->lon);
    double lat1 = degreesToRadians(from->lat);
    double lat2 = degreesToRadians(to->lat);
    double a = pow(sin(dLat/2),2) + pow(sin(dLon/2),2) * cos(lat1)*cos(lat2);
    double rad = 2*atan2(sqrt(a),sqrt(1-a));
    return rad * earth_radius;
}

//var ring = this;  // so we can work with a clone if needed
//if(projection) {
//var gg = new OpenLayers.Projection("EPSG:4326");
//if(!gg.equals(projection)) {
//ring = this.clone().transform(projection, gg);
//}
//}
//var area = 0.0;
//var len = ring.components && ring.components.length;
//if(len > 2) {
//var p1, p2;
//for(var i=0; i<len-1; i++) {
//p1 = ring.components[i];
//p2 = ring.components[i+1];
//area += OpenLayers.Util.rad(p2.x - p1.x) *
//(2 + Math.sin(OpenLayers.Util.rad(p1.y)) +
//Math.sin(OpenLayers.Util.rad(p2.y)));
//}
//area = area * 6378137.0 * 6378137.0 / 2.0;
//}
//return area;
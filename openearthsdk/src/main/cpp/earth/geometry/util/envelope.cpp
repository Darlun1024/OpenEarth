//
// Created by GXSN-Pro on 2018/10/26.
//

#include "envelope.hpp"

using namespace OpenEarth::Geometry;
Envelope::Envelope(double minx,double miny,double maxx,double maxy){
    this->maxX = maxx;
    this->minX = minx;
    this->maxY = maxy;
    this->minY = miny;
}

Envelope::~Envelope() {

}
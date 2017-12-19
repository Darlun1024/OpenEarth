//
// Created by GXSN-Pro on 2017/11/14.
//

#include "util.hpp"
#include <sstream>
#include <math.h>

template <class T>
string OpenEarth::Util::tostr( const T& t) {
    std::ostringstream os;
    os<<t;
    return os.str();
}

float OpenEarth::Util::degree2Rad(float degree){
    return M_PI * degree / 180;
}
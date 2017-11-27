//
// Created by GXSN-Pro on 2017/11/14.
//

#include "util.hpp"
#include <sstream>
template <class T>
string OpenEarth::Util::tostr( const T& t) {
    std::ostringstream os;
    os<<t;
    return os.str();
}
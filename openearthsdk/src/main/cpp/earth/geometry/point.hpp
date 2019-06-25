//
// Created by GXSN-Pro on 2018/1/24.
//

#ifndef OPENEARTH_POINT_HPP
#define OPENEARTH_POINT_HPP

#include "geometry.hpp"

namespace OpenEarth {
    namespace Geometry {
        class Point: public Geometry {
        public:
            double lat;
            double lon;
        };
    }
}


#endif //OPENEARTH_POINT_HPP

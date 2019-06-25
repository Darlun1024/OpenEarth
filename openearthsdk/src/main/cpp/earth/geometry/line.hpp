//
// Created by GXSN-Pro on 2018/1/24.
//

#ifndef OPENEARTH_LINE_HPP
#define OPENEARTH_LINE_HPP

#include <vector>
#include "latlng.hpp"
#include "geometry.hpp"
#include <array>


namespace OpenEarth {
    namespace Geometry {
        class Line: public Geometry {
        private:
            LatLng *lineString;
            long pointCount;
            float * vertextAttribArrary;
            void  generateVertexAttribArray();
        public:
            Line();
            ~Line();
            void setCoordinates(const LatLng* points,long count);
            LatLng* getCoordinates() const ;
            float* getVertexAttribArray(long *count) const ;
            long getPointCount() const ;
//            double[]
        };
    }
}


#endif //OPENEARTH_LINE_HPP

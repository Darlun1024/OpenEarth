//
// Created by GXSN-Pro on 2017/12/29.
//

#ifndef OPENEARTH_BOUNDS_HPP
#define OPENEARTH_BOUNDS_HPP


namespace OpenEarth{
    namespace Geometry{
        class Bounds {
        public:
            double  left;
            double  right;
            double  bottom;
            double  top;
            Bounds(double l, double b, double r, double t){
                this->left   = l;
                this->bottom = b;
                this->right  = r;
                this->top    = t;
            }
            ~Bounds(){}
        };
    }
}



#endif //OPENEARTH_BOUNDS_HPP

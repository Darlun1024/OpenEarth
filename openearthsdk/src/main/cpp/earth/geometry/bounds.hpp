//
// Created by GXSN-Pro on 2017/12/29.
//

#ifndef OPENEARTH_BOUNDS_HPP
#define OPENEARTH_BOUNDS_HPP


namespace OpenEarth{
    namespace Geometry{
        class Bounds {
        public:
            float  left;
            float  right;
            float  bottom;
            float  top;
            Bounds(float l, float b, float r, float t){
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

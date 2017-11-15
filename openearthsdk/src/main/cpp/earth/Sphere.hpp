//
// Created by GXSN-Pro on 2017/11/15.
//

#ifndef OPENEARTH_SPHERE_HPP
#define OPENEARTH_SPHERE_HPP

#include <GLES2/gl2.h>
namespace OpenEarth {
    class Sphere {
    public:
         OpenEarth::Sphere::Sphere(float radius);
         OpenEarth::Sphere::~Sphere(float radius);

         void draw();

    };


}
#endif //OPENEARTH_SPHERE_HPP

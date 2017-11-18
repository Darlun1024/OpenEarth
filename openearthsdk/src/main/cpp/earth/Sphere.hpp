//
// Created by GXSN-Pro on 2017/11/15.
//

#ifndef OPENEARTH_SPHERE_HPP
#define OPENEARTH_SPHERE_HPP

#include <GLES2/gl2.h>
namespace OpenEarth {
    class Sphere {
    public:
         Sphere(float radius);
         ~Sphere();

         void draw();

    private:
        void generateVertexArray();
    };


}
#endif //OPENEARTH_SPHERE_HPP

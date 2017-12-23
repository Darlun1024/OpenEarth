#ifndef OPENEARTH_PROGRAM_HPP
#define OPENEARTH_PROGRAM_HPP

#include <GLES2/gl2.h>

namespace OpenEarth{
    namespace Programs{
        class Program{
        public:
            static GLuint createProgram(const char* vertexShaderSource,const char* fragmentShaderSource);
        };
    }
}


#endif
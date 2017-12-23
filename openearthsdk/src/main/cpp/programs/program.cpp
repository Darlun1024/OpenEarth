#include "program.hpp"

namespace OpenEarth{
    namespace Programs{
        GLuint Program::createProgram(const char* vertexShaderSource,const char* fragmentShaderSource){
            GLuint glProgram;
            GLuint vertexShader;
            GLuint fragmentShader;
            glProgram = glCreateProgram();

            if (glProgram == 0) {
                return 0;
            }

            glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
            //vertexShader
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

            //fragmentShader
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(vertexShader);
            glCompileShader(fragmentShader);

            glAttachShader(glProgram, vertexShader);
            glAttachShader(glProgram, fragmentShader);

            glLinkProgram(glProgram);
            int linkStatus;
            glGetProgramiv(glProgram, GL_LINK_STATUS, &linkStatus);
            if (linkStatus == 0) {
                glDeleteProgram(glProgram);
            }
            return glProgram;
        }
    }
}

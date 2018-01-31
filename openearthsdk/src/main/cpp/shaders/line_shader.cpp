//
// Created by GXSN-Pro on 2018/1/23.
//

#include "line_shader.hpp"

namespace OpenEarth {
    namespace Shaders {
        const char *LineShader::name = "line_shader";
        const char *LineShader::veterxShader = R"SHADER(
                 uniform mat4 u_MVPMatrix;
                 attribute vec4 POSITION;
                 uniform vec4 u_LineColor;
                 varying vec4 v_LineColor;
                 void main(){
                    gl_Position = u_MVPMatrix*POSITION;
                    v_LineColor = u_LineColor;
                 }
        )SHADER";

        const char *LineShader::fragmentShader = R"SHADER(
                 precision mediump float;
                varying vec4 v_LineColor;
                 void main(){
                    gl_FragColor = v_LineColor;
                 }
        )SHADER";
    }
}
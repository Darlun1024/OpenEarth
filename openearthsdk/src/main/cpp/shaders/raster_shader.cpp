//
// Created by GXSN-Pro on 2017/12/23.
//

#include "raster_shader.hpp"


namespace OpenEarth {
    namespace Shader {
        const char *RasterShader::name = "raster_shader";
        //这是C++11支持的新的字符串初始化方法 R"()"
        const char *RasterShader::veterxShader = R"SHADER(
                uniform mat4 u_MVPMatrix;
                 attribute vec4 POSITION;
                 attribute vec2 a_TextureCoordinates;
                 varying vec2 v_TextureCoordinates;
                 void main(){
                 gl_Position = u_MVPMatrix*POSITION;
                 v_TextureCoordinates = a_TextureCoordinates;
                 }
        )SHADER";

        const char *RasterShader::fragmentShader = R"SHADER(
                precision mediump float;
                 uniform sampler2D u_TextureUnit;
                 varying vec2 v_TextureCoordinates;
                 void main(){
                    gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);
                 }
        )SHADER";
    }
}
//
// Created by GXSN-Pro on 2018/3/21.
//

#include "light_raster_shader.hpp"

namespace OpenEarth {
    namespace Shaders {
        const char *LightRasterShader::name = "raster_shader";
        const char *LightRasterShader::veterxShader = R"SHADER(
                uniform mat4 u_MVPMatrix;
                uniform mat4 u_MVMatrix;
                uniform vec3 u_LightPos;

                attribute vec4 POSITION;
                attribute vec2 a_TextureCoordinates;
                attribute vec3 a_Normal;

                varying vec2 v_TextureCoordinates;
                varying float v_diffuse;
                 void main(){
                     a_Normal = vec3(0,0,1.0);
                     vec3 modelViewVertex = vec3(u_MVMatrix * a_Position);
                     vec3 modelViewNormal = vec3(u_MVMatrix * vec4(a_Normal, 0.0));
                     float distance = length(u_LightPos - modelViewVertex);
                     vec3 lightVector = normalize(u_LightPos - modelViewVertex);
                     v_diffuse = max(dot(modelViewNormal, lightVector), 0.1);
                     v_diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));
                     gl_Position = u_MVPMatrix * a_Position;
                 }
        )SHADER";

        const char *LightRasterShader::fragmentShader = R"SHADER(
                precision mediump float;
                 uniform sampler2D u_TextureUnit;
                 varying vec2 v_TextureCoordinates;
                 void main(){
                    gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);
                    gl_FragColor = gl_FragColor * v_diffuse;
                 }
        )SHADER";
    }
}
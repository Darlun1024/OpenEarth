//
// Created by GXSN-Pro on 2017/12/14.
//

#include <glm/vec3.hpp>
#include "earth.hpp"

namespace OpenEarth {
    static const float MIN_RADIUS  = 1.0f;
    static  int min_zoom  = 1;
    static  int max_zoom  = 16;
    static float earth_radius = DEFAULT_RADIUS;
    static float earth_scale  = DEFAULT_SCALE;
    static int earth_zoom   = 1;
    static glm::vec3 center = glm::vec3(0, 0, 0); //球心

    void OpenEarth::Earth::setRadius(float r) {
        earth_radius = r;
    }

    float OpenEarth::Earth::getRadius() {
        return earth_radius;
    }

    bool OpenEarth::Earth::setZoom(int zoom){
        assert(min_zoom <=zoom<=max_zoom);
        earth_scale = 1.0f;
        bool needUpdate = (earth_zoom!=zoom);
        earth_zoom = zoom;
        earth_radius =  OpenEarth::DEFAULT_RADIUS * pow(2, zoom - 1); //重新计算半径
        return needUpdate;
    }

    int OpenEarth::Earth::getZoom(){
        return earth_zoom;
    }

    bool OpenEarth::Earth::setScale(float scale) {
        assert(scale>0.0f);
        float newScale = earth_scale * scale;
        int newZoom  = earth_zoom;
        if(newScale >= 2.0f){   //[2.0 )
            int zoom = (int)log2f(newScale);
            newZoom += zoom;
            if(newZoom > max_zoom) {
                newZoom = max_zoom;
                newScale = 2.0f;
            }else{
                newScale = earth_radius * newScale / pow(2,newZoom-1); //放大后的球体半径/当前级别的球体半径
            }
        }else if(1.0 <= newScale){                   //[1.0  2.0)

        }else if(0.5 < newScale){                   //(0.5 1.0)
            if(earth_zoom == min_zoom) newScale = 1.0f;
        }else{                                      //(0 0.5)
            int zoom = (int)log2f(newScale);
            newZoom += zoom;
            if(newZoom < min_zoom) {
                newZoom = min_zoom;
                newScale = 1.0f;
            }else{
                newScale = earth_radius * newScale / pow(2,newZoom-1); //缩小后的球体半径/当前级别的球体半径
            }
        }

        earth_scale = newScale;
        if(newZoom!=earth_zoom){
            earth_zoom = newZoom;
            earth_radius =  OpenEarth::DEFAULT_RADIUS * pow(2, earth_zoom - 1); //重新计算半径
            return true;
        }
        return false;
    }

    float OpenEarth::Earth::getScale() {
        return earth_scale;
    }

    glm::vec3 OpenEarth::Earth::getCenter() {
        return center;
    }
}
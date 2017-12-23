//
// Created by GXSN-Pro on 2017/12/14.
//

#include <glm/vec3.hpp>
#include "earth.hpp"
#include "../util/util.hpp"
#include <glm/ext.hpp>

namespace OpenEarth {
    static float  const  DEFAULT_RADIUS = 1.0f;
    static float  const  DEFAULT_SCALE  = 1.0f;
    static  int   min_zoom  = 1;
    static  int   max_zoom  = 16;
    static  float earth_radius = DEFAULT_RADIUS;
    static  float earth_scale  = DEFAULT_SCALE;
    static  int   earth_zoom   = 1;
    static  glm::vec3 center = glm::vec3(0, 0, 0); //球心
    static  LatLng* center_latlng; //视图中心的经纬度(degree 表示) lon[-180 180] 纬度[-90 90]
    static  glm::mat4 model_matrix;
    /**初始化*/
     void OpenEarth::Earth::initialize(){
        earth_radius  = DEFAULT_RADIUS;
        earth_zoom    = 1.0f;
        center_latlng = new LatLng(0.0f,0.0f);

        model_matrix = glm::mat4(1.0);
        //这里减 1 是为了在调整camera瞄准点时，屏幕下方出现地图空缺的问题，这个数字和透视投影的角度，相机距球体的位置有关
        model_matrix = glm::translate(model_matrix, glm::vec3(0, 0, -earth_radius * earth_scale - 1));
        model_matrix = glm::scale(model_matrix,glm::vec3(earth_scale,earth_scale,earth_scale));
    }

    void OpenEarth::Earth::updateModelMatrix(){
        model_matrix = glm::mat4(1.0);
        //这里减 1 是为了在调整camera瞄准点时，屏幕下方出现地图空缺的问题，这个数字和透视投影的角度，相机距球体的位置有关
        model_matrix = glm::translate(model_matrix, glm::vec3(0, 0, -earth_radius * earth_scale - 1));
        model_matrix = glm::scale(model_matrix,glm::vec3(earth_scale,earth_scale,earth_scale));
        float earthRotateX =   center_latlng->lat * M_PI/180;  //anit-clockwise +
        float earthRotateY =   -(center_latlng->lon+180) * M_PI/180;
        model_matrix = glm::rotate(model_matrix,earthRotateX,glm::vec3(1,0,0));
        model_matrix = glm::rotate(model_matrix,earthRotateY,glm::vec3(0,1,0));
    }

    /** 获取当前的模型矩阵*/
     glm::mat4 OpenEarth::Earth::getModelMatrix(){
        return model_matrix;
    }

     void OpenEarth::Earth::rotate(float deltaLat, float deltaLon){
         center_latlng->lat  -= deltaLat;
         center_latlng->lon  -= deltaLon;
         updateModelMatrix();
     }

    void OpenEarth::Earth::setCenterLatLng(LatLng *latLng) {
        center_latlng->lat = latLng->lat;
        center_latlng->lon = latLng->lon;
        updateModelMatrix();
    }

    LatLng* OpenEarth::Earth::getCenterLatLng(){
        center_latlng->normalize();
        return center_latlng;
    }


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
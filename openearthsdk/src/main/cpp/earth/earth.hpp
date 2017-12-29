#pragma once
//
// Created by GXSN-Pro on 2017/11/29.
//

#ifndef OPENEARTH_EARTH_HPP
#define OPENEARTH_EARTH_HPP

#include <glm/matrix.hpp>
#include "geometry/latlng.hpp"
#include "geometry/bounds.hpp"

namespace OpenEarth{
    static int const  TILE_SIZE  = 256;
    class Earth{
    private:
    public:
        /**初始化*/
        static void initialize();

        static void updateModelMatrix();

        /** 获取当前的模型矩阵*/
        static glm::mat4 getModelMatrix();

        /*** 设置球体半径*/
        static void setRadius(float r);
        /*** 获取球体半径*/
        static float getRadius();

        /**
         * 设置球体缩放比例
         * @return 如果改变了级别，返回true,否则返回flase
         */
        static bool setScale(float scale);

        /*** 获取球体缩放比例*/
        static float getScale();

        static bool  setZoom(int zoom);

        static int getZoom();

        /**
         * 旋转球体
         * @param deltaLat  变化的纬度 (degree表示）
         * @param deltaLon  变化的经度（degree表示）
         */
        static void rotate(float deltaLat, float deltaLon);

        /*** 沿Z轴上移动球心*/
        static void setTranslateZ(float distance);

        /*** 获取球心的世界坐标*/
        static glm::vec3 getCenter();

        /***设置球心的世界坐标*/
        static void setCenter(glm::vec3 center);

        /*** 设置当前视图中心的经纬度*/
        static void   setCenterLatLng(LatLng* latLng);

        /*** 获取当前视图中心的经纬度*/
        static LatLng* getCenterLatLng();

         /**获取可见的地图范围*/
        static OpenEarth::Geometry::Bounds getViewMapBounds();

    };
}

#endif //OPENEARTH_EARTH_HPP

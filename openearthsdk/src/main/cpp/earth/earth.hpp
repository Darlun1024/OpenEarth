#pragma once
//
// Created by GXSN-Pro on 2017/11/29.
//

#ifndef OPENEARTH_EARTH_HPP
#define OPENEARTH_EARTH_HPP

#include <glm/matrix.hpp>
#include "geometry/latlng.hpp"

namespace OpenEarth{
    static float  const  DEFAULT_RADIUS = 1.0f;
    static float  const  DEFAULT_SCALE  = 1.0f;
    static int const  TILE_SIZE  = 256;
    class Earth{
    public:
        /**
         * 设置球体半径
         * @param r
         */
        static void setRadius(float r);
        /**
         * 获取球体半径
         * @return
         */
        static float getRadius();

        /**
         * 设置球体缩放比例
         * @param scale
         * @return 如果改变了级别，返回true,否则返回flase
         */
        static bool setScale(float scale);

        /**
         * 获取球体缩放比例
         * @return
         */
        static float getScale();


        /**
         * 绕X轴转
         * @param rad
         */
        static void rotateX(float rad);

        /**
         * 绕Y轴转
         * @param rad
         */
        static void rotateY(float rad);

        /**
         * 绕Z轴转
         * @param rad
         */
        static void rotateZ(float rad);

        /**
         * 沿Z轴上移动球心
         * @param distance
         */
        static void setTranslateZ(float distance);

        /**
         * 获取球心的世界坐标
         * @return
         */
        static glm::vec3 getCenter();

        /***设置球心的世界坐标*/
        static void setCenter(glm::vec3 center);

        /**
         * 设置当前视图中心的经纬度
         * @param latLng
         */
        static void   setCenterLatLng(LatLng latLng);
        /**
         * 获取当前视图中心的经纬度
         * @return
         */
        static LatLng getCenterLatLng();
    };
}

#endif //OPENEARTH_EARTH_HPP

#pragma once
//
// Created by GXSN-Pro on 2017/11/29.
//

#ifndef OPENEARTH_EARTH_HPP
#define OPENEARTH_EARTH_HPP

#include <glm/matrix.hpp>

namespace OpenEarth{
    static float  const  DEFAULT_RADIUS = 1.0f;
    static float  const  DEFAULT_SCALE  = 1.0f;
    static int const  TILE_SIZE  = 256;
    class Earth{
    public:
        static void setRadius(float r);
        static float getRadius();

        static void setScale(float scale);
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

        static glm::vec3 getCenter();
    };
}

#endif //OPENEARTH_EARTH_HPP

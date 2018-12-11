//
// Created by GXSN-Pro on 2018/12/11.
// from GoogleMaps
//

#ifndef OPENEARTH_MATH_UTIL_HPP
#define OPENEARTH_MATH_UTIL_HPP

namespace OpenEarth{
    namespace Geometry{
        class MathUtil {
        public:
            /**
             * 确保 x 保持在low 和 high 之 间
             * Restrict x to the range [low, high]
             * x ∈ [low,high]
             * */
            static double clamp(double x, double low, double high);
            /***
             * 周期性函数使用，确保n在[min,max]周期内
             * */
            static double wrap(double n, double min, double max);
            /***
             * 取余，结果为非负值
             * @param x
             * @param m
             * @return
             */
            static double mod(double x, double m);

            /***
             * 纬度转为墨卡托投影的y坐标
             * @param lat 纬度
             * @return
             */
            static double mercator(double lat) ;
            /***
             * 墨卡托y坐标转纬度
             * @param y
             * @return
             */
            static double inverseMercator(double y);
            /***
             * 半正矢，在球面三角形的相关计算中使用
             * @param x
             * @return
             */
            static double hav(double x);

            /***
             * 半正矢的逆函数
             * @param x
             * @return
             */
            static double arcHav(double x);

            /***
             * h==hav(x), returns sin(abs(x))
             * @param h
             * @return
             */
            static double sinFromHav(double h);

            /***
             * hav(asin(x))
             * @param x
             * @return
             */
            static double havFromSin(double x);
            /***
             * sin(arcHav(x) + arcHav(y))
             * @return
             */
            static double sinSumFromHav(double x, double y) ;

            /***
             * 计算两点之间的球面距离
             * @param lat1
             * @param lat2
             * @param dLng
             * @return
             */
            static double havDistance(double lat1, double lat2, double dLng);
        };
    }
}



#endif //OPENEARTH_MATH_UTIL_HPP

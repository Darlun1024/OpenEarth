//
// 不同栅格图层使用的Tile的级别，行列号规则都是一致的，
// 只是在绘制的时候使用不同的图片即可
// 不需要为每个图层都构建Tile对象
//
// Created by GXSN-Pro on 2017/12/26.
//

#ifndef OPENEARTH_TILEMANAGEMENT_HPP
#define OPENEARTH_TILEMANAGEMENT_HPP

#include <map>
#include <string>
#include "tile.hpp"

using namespace std;
namespace OpenEarth {
    namespace Earth {

        class TileManagement {
        private:

        public:
            TileManagement();
            ~TileManagement();
            Tile* getTile(int zoom,int x,int y);

        private:
            class Impl;
            Impl* impl;

        };

    }
}
#endif //OPENEARTH_TILEMANAGEMENT_HPP

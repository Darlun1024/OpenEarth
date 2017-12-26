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
            Tile* getTile(string type,int zoom,int x,int y);

        private:
            class Impl;
            Impl* impl;

        };

    }
}
#endif //OPENEARTH_TILEMANAGEMENT_HPP

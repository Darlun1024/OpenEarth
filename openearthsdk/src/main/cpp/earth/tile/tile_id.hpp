//
// Created by GXSN-Pro on 2018/1/3.
//

#ifndef OPENEARTH_TILE_ID_HPP
#define OPENEARTH_TILE_ID_HPP

#include <cstdint>

namespace OpenEarth{
    namespace Tile{
        class TileID {
        public:
            int32_t z;
            int32_t y;
            int8_t x;
            TileID(int32_t x,int32_t y,int8_t z){
                this->x = x;
                this->y = y;
                this->z = z;
            }

            ~TileID(){}
            bool operator==(const TileID&) const;
            bool operator!=(const TileID&) const;
            bool operator<(const TileID&) const;
            bool isChildOf(const TileID&) const;

        };
    }
}

template <>
struct std::hash<OpenEarth::Tile::TileID> {
    size_t operator()(const OpenEarth::Tile::TileID& id) const;
};

#endif //OPENEARTH_TILE_ID_HPP

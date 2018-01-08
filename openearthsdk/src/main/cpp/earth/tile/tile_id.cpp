//
// Created by GXSN-Pro on 2018/1/3.
//

#include <tuple>
#include "tile_id.hpp"
#include <boost/functional/hash.hpp>

namespace OpenEarth {
    namespace Tile {
        bool TileID::operator==(const TileID &rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        bool TileID::operator!=(const TileID &rhs) const {
            return x != rhs.x || y != rhs.y || z != rhs.z;
        }

        bool TileID::operator<(const TileID &rhs) const {
            return std::tie(z, x, y) < std::tie(rhs.z, rhs.x, rhs.y);
        }

        bool TileID::isChildOf(const TileID &rhs) const {

        }

    }
}

size_t std::hash<OpenEarth::Tile::TileID>::operator()(const OpenEarth::Tile::TileID &id)const {
    size_t seed;
    boost::hash_combine(seed,id.z);
    boost::hash_combine(seed,id.x);
    boost::hash_combine(seed,id.y);
    return seed;
}

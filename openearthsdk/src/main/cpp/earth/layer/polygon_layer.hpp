//
// Created by GXSN-Pro on 2018/2/9.
//

#ifndef OPENEARTH_POLYGON_LAYER_HPP
#define OPENEARTH_POLYGON_LAYER_HPP

#include "vector_layer.hpp"

namespace OpenEarth {
    namespace Layers {
        class PolygonLayer:VectorLayer {
        public:
            PolygonLayer(const std::string &layerId, const std::string &name,
                         const std::string &sourceId);

        };
    }
}

#endif //OPENEARTH_POLYGON_LAYER_HPP

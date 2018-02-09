//
// Created by GXSN-Pro on 2018/2/9.
//

#include "polygon_layer.hpp"
#include "vector_layer.hpp"

namespace OpenEarth {
    namespace Layers {
        OpenEarth::Layers::PolygonLayer::PolygonLayer(const std::string &layerId,
                                                      const std::string &name,
                                                      const std::string &sourceId) : VectorLayer(
                layerId,
                name,
                sourceId) {

        }
    }
}


//
// Created by GXSN-Pro on 2018/2/9.
//

#include "point_layer.hpp"

namespace OpenEarth{
    namespace  Layers{
        PointLayer::PointLayer(const std::string &layerId, const std::string &name,
                                                  const std::string &sourceId) : VectorLayer(layerId, name,
                                                                                             sourceId) {

        }

    }
}

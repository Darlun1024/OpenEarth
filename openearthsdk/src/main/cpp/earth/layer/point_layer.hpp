//
// Created by GXSN-Pro on 2018/2/9.
//

#ifndef OPENEARTH_POINT_LAYER_HPP
#define OPENEARTH_POINT_LAYER_HPP

#include "vector_layer.hpp"

namespace OpenEarth{
    namespace Layers{
        class PointLayer:VectorLayer{
        public:
            PointLayer(const std::string &layerId, const std::string &name,
                       const std::string &sourceId);

        };
    }
}



#endif //OPENEARTH_POINT_LAYER_HPP

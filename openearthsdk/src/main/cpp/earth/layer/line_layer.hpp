//
// Created by GXSN-Pro on 2018/2/9.
//

#ifndef OPENEARTH_LINE_LAYER_HPP
#define OPENEARTH_LINE_LAYER_HPP

#include "vector_layer.hpp"

namespace OpenEarth{
    namespace Layers{
        class LineLayer:VectorLayer {
        public:
            LineLayer(const std::string &layerId, const std::string &name,
                      const std::string &sourceId);

        };
    }
}



#endif //OPENEARTH_LINE_LAYER_HPP

//
// Created by GXSN-Pro on 2018/2/9.
//

#ifndef OPENEARTH_LINE_LAYER_HPP
#define OPENEARTH_LINE_LAYER_HPP

#include "vector_layer.hpp"
#include <list>

namespace OpenEarth{
    namespace Layers{
        class LineLayer:VectorLayer {
        private:
        public:
            LineLayer(const std::string &layerId, const std::string &name,
                      const std::string &sourceId);

            void draw(int aColorLocation,int aPositionLocation);
        };
    }
}


#endif //OPENEARTH_LINE_LAYER_HPP

//
// Created by 思杰 on 2018/1/24.
//

#ifndef OPENEARTH_VECTOR_LAYER_H
#define OPENEARTH_VECTOR_LAYER_H

#include "layer.hpp"
#include "../feature/feature.hpp"

namespace OpenEarth{
    namespace Layers{
        class VectorLayer:Layer {
        public:
            VectorLayer(const std::string &layerId,const std::string &name, const std::string &sourceId);
            void addFeature(const OpenEarth::Features::Feature *feature);
            void draw();
        };
    }
}



#endif //OPENEARTH_VECTOR_LAYER_H

//
// Created by 思杰 on 2018/1/24.
//

#include "vector_layer.hpp"

namespace OpenEarth {
    namespace Layers {
        VectorLayer::VectorLayer(const std::string &layerId, const std::string &name,
                                 const std::string &sourceId)
                : Layer(layerId, name, sourceId) {

        }

        void VectorLayer::draw() {

        }

        void VectorLayer::addFeature(const OpenEarth::Features::Feature *feature) {

        }
    }
}




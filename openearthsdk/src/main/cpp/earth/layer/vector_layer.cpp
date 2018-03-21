//
// Created by 思杰 on 2018/1/24.
//

#include "vector_layer.hpp"

namespace OpenEarth {
    namespace Layers {
        using namespace Features;
        VectorLayer::VectorLayer(const std::string &layerId, const std::string &name,
                                 const std::string &sourceId)
                : Layer(layerId, name, sourceId) {
            mFeatures = make_shared<list<const Feature*>>();
        }

        void VectorLayer::draw() {

        }

        void VectorLayer::addFeature(const Feature* feature) {
            mFeatures->push_back(feature);

        }

        void VectorLayer::removeFeature(const Feature* feature) {

        }
    }
}




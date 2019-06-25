//
// Created by 思杰 on 2018/1/24.
//

#ifndef OPENEARTH_VECTOR_LAYER_H
#define OPENEARTH_VECTOR_LAYER_H

#include "layer.hpp"
#include "../feature/feature.hpp"
#include <list>


namespace OpenEarth{
    namespace Layers{
        using namespace std;
        class VectorLayer:Layer {
        protected:
            shared_ptr<list<const  Features::Feature*>> mFeatures;
        public:
            VectorLayer(const std::string &layerId,const std::string &name, const std::string &sourceId);
            void addFeature(const OpenEarth::Features::Feature *feature);
            virtual void draw();
            void removeFeature(const OpenEarth::Features::Feature *feature);
        };
    }
}



#endif //OPENEARTH_VECTOR_LAYER_H

//
// Created by GXSN-Pro on 2017/12/27.
//

#include "layer.hpp"

namespace OpenEarth {
    namespace Layer {
        Layer::Layer(const std::string &layerId,const  std::string &name,const std::string &sourceId):
        id(layerId),name(name),sourceId(sourceId) {

        }
    }
}
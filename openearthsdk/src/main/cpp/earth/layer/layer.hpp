//
// Created by GXSN-Pro on 2017/12/27.
//

#ifndef OPENEARTH_LAYER_HPP
#define OPENEARTH_LAYER_HPP


#include <string>
namespace  OpenEarth{
    namespace  Layer{
        class Layer {
        public:
            Layer(std::string layerId,std::string sourceId);
            std::string name;
            std::string id;

        };
    }
}



#endif //OPENEARTH_LAYER_HPP

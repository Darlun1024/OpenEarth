//
// Created by GXSN-Pro on 2017/12/27.
//

#ifndef OPENEARTH_LAYER_HPP
#define OPENEARTH_LAYER_HPP


#include <string>
#include "../feature/feature.hpp"

namespace  OpenEarth{
    namespace Layer{
        class Layer {
        public:
            const  std::string name;
            std::string id;
            std::string sourceId;
            Layer(const std::string &layerId,const  std::string &name,const std::string &sourceId);

            void draw();
        };
    }
}



#endif //OPENEARTH_LAYER_HPP

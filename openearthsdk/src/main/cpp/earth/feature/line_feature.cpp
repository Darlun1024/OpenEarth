//
// Created by GXSN-Pro on 2018/1/24.
//

#include "line_feature.hpp"

namespace OpenEarth {
    namespace Features {
        LineFeature::LineFeature(){

        };

        LineFeature::~LineFeature(){
            if(this->line){
                delete  this->line;
            }
        }

        void LineFeature::setLine(Line* line){
            this->line = line;
        }

        Line* LineFeature::getLine() const{
            return  this->line;
        }
    }
}
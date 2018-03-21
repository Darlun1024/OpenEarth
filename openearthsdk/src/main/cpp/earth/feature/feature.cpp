//
// Created by GXSN-Pro on 2018/1/24.
//

#include "feature.hpp"

namespace  OpenEarth{
    namespace Features{
        Feature::Feature(){

        }

        Feature::~Feature(){

        }

        void Feature::setAttribute(string key, string value) {

        }

        void Feature::setGeometry(shared_ptr<Geometry::Geometry> geo) {
            mGeometry = std::move(geo);
        }
    }
}
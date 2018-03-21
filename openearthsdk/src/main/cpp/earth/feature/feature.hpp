//
// Created by GXSN-Pro on 2018/1/24.
//

#ifndef OPENEARTH_FEATURE_HPP
#define OPENEARTH_FEATURE_HPP

#include "../geometry/geometry.hpp"
#include <memory>
#include <jni.h>
#include <unordered_map>

//class VectorLayer;
namespace OpenEarth{
    namespace Features{

        using namespace std;

        class Feature{
        protected:
            shared_ptr<Geometry::Geometry> mGeometry;
            shared_ptr<unordered_map<string,string>> mAttributes;
//            weak_ptr<VectorLayer> mLayer;
        public:
            Feature();
            ~Feature();
            void setAttribute(string key,string value);

            void setGeometry(shared_ptr<Geometry::Geometry> geo);


        };
    }
}



#endif //OPENEARTH_FEATURE_HPP

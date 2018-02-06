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
            unique_ptr<Geometry::Geometry> mGeometry;
            unique_ptr<unordered_map<string,string>> mAttributes;
//            weak_ptr<VectorLayer> mLayer;
        public:
            Feature();
            ~Feature();
            void draw(JNIEnv* jniEnv);
            void setGeometry(unique_ptr<Geometry::Geometry> geo);
            void setAttribute(string key,string value);

        };
    }
}



#endif //OPENEARTH_FEATURE_HPP

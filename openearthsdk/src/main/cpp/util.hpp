//
// Created by GXSN-Pro on 2017/11/14.
//

#ifndef OPENEARTH_JNI_HPP
#define OPENEARTH_JNI_HPP

#include <string>

using namespace std;
namespace OpenEarth{
    class Util {

    public:
        template <class T>
        static  string tostr(const T& t);
    };
}


#endif //OPENEARTH_JNI_HPP

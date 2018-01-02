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
        static  float degree2Rad(float degree);
        static  string int2String(int i){
            char str[10];
            sprintf(str,"%d",i);
            return str;
        }

        static string   replace_all(string   str,const   string   old_value,const   string   new_value)
        {
            while(true)   {
                string::size_type   pos(0);
                if(   (pos=str.find(old_value))!=string::npos   )
                    str.replace(pos,old_value.length(),new_value);
                else   break;
            }
            return   str;
        }
    };
}


#endif //OPENEARTH_JNI_HPP

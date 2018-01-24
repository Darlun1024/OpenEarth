//
// Created by GXSN-Pro on 2018/1/20.
//

#ifndef OPENEARTH_RESPONE_HPP
#define OPENEARTH_RESPONE_HPP
#include <string>
namespace OpenEarth{
    namespace Storage{
        class Response{
        public:
            std::string url;
            char *byteArray;
            long length;
            Response(std::string urlString = "",char* bytes= nullptr,long len=0){
                url = urlString;
                byteArray = bytes;
                length = len;
            }

            ~Response(){

            }
        };
    }
}
#endif //OPENEARTH_RESPONE_HPP

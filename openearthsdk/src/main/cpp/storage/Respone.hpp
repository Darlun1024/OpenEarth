//
// Created by GXSN-Pro on 2018/1/20.
//

#ifndef OPENEARTH_RESPONE_HPP
#define OPENEARTH_RESPONE_HPP
#include <string>
namespace OpenEarth{
    namespace Storage{
        typedef struct {
            std::string url;
            char *byteArray;
            long length;
        }Response;
    }
}
#endif //OPENEARTH_RESPONE_HPP

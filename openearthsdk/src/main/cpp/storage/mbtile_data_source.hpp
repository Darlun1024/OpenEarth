//
// Created by GXSN-Pro on 2018/1/20.
//

#ifndef OPENEARTH_MBTILE_DATA_SOURCE_HPP
#define OPENEARTH_MBTILE_DATA_SOURCE_HPP

#include <string>
#include <memory>
#include "Respone.hpp"

namespace OpenEarth{
    namespace Storage{
        class MBTileDataSource {
        private:
            void initDataBase();
        public:
            MBTileDataSource();
            ~MBTileDataSource();
            static std::unique_ptr<MBTileDataSource> newInstance();
            Response* request(std::string url);
            bool isValidUrl(const std::string &url);

        };
    }
}



#endif //OPENEARTH_MBTILE_DATA_SOURCE_HPP

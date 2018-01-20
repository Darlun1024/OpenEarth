//
// Created by GXSN-Pro on 2018/1/20.
//

#ifndef OPENEARTH_MBTILE_DATA_SOURCE_HPP
#define OPENEARTH_MBTILE_DATA_SOURCE_HPP

#include <string>
#include <memory>


namespace OpenEarth{
    namespace Storage{
        class MBTileDataSource {
        private:
            void initDataBase();
        public:
            MBTileDataSource(std::string path);
            ~MBTileDataSource();
            std::unique_ptr<MBTileDataSource> newInstance(std::string path);

        };
    }
}



#endif //OPENEARTH_MBTILE_DATA_SOURCE_HPP

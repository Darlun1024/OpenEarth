//
// Created by GXSN-Pro on 2018/1/20.
//

#include <map>
#include "mbtile_data_source.hpp"
#include "../sqlite/sqlite3pp.h"
#include <boost/algorithm/string/split.hpp>

namespace OpenEarth{
    namespace Storage{
        using namespace std;
        using namespace sqlite3pp;


        const char *protocol = "mbtile://";
        const uint8_t protocolLength = 9;

        unique_ptr<database> db;
        unique_ptr<map<string,shared_ptr<database>>> databaseMap;

       // /storage/emulated/0/
        MBTileDataSource::MBTileDataSource(){
           databaseMap = make_unique<map<string,shared_ptr<database>>>();
        }

        MBTileDataSource::~MBTileDataSource(){
            databaseMap.reset();
        }

        std::unique_ptr<MBTileDataSource> MBTileDataSource::newInstance(){
            std::unique_ptr<MBTileDataSource> mbDataSource = std::make_unique<MBTileDataSource>();
            return mbDataSource;
        }

        Response request(std::string url){
            //TODO
            string dbPath = "";
//            shared_ptr<database> db = ;
        }

        bool MBTileDataSource::isValidUrl(const std::string &url) {
            return url.compare(0, protocolLength, protocol) == 0;
        }


    }
}

//
// Created by GXSN-Pro on 2018/1/20.
//

#include "mbtile_data_source.hpp"
#include "../sqlite/sqlite3pp.h"

namespace OpenEarth{
    namespace Storage{
        using namespace std;
        using namespace sqlite3pp;
        unique_ptr<database> db;
       // /storage/emulated/0/
        MBTileDataSource::MBTileDataSource(const std::string path){
            db = make_unique<database>(path.c_str());
           initDataBase();
        }

        MBTileDataSource::~MBTileDataSource(){

        }

        std::unique_ptr<MBTileDataSource> MBTileDataSource::newInstance(const  std::string path){
            std::unique_ptr<MBTileDataSource> mbDataSource = std::make_unique<MBTileDataSource>(path);
            return mbDataSource;
        }

        void MBTileDataSource::initDataBase(){
            const char* sql  = "create table tab_test(ID TEXT,Name TEXT)";
            db->execute(sql);
        }
    }
}

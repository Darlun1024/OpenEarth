//
// Created by GXSN-Pro on 2018/1/18.
//

#include "database_data_source.hpp"

namespace OpenEarth {
    namespace Storage {
        shared_ptr<DatabaseDataSource> singleDbPtr;

        DatabaseDataSource::DatabaseDataSource(string path):mDBPath(path) {

        }

        DatabaseDataSource::~DatabaseDataSource() {

        }


        shared_ptr<DatabaseDataSource> DatabaseDataSource::newInstance(string path){
            if(singleDbPtr== nullptr){
               singleDbPtr = make_shared<DatabaseDataSource>(path);
            }
            return singleDbPtr;
        }

        void DatabaseDataSource::initDatabase(){

        }
    }
}

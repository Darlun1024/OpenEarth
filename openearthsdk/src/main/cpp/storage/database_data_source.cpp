//
// Created by GXSN-Pro on 2018/1/18.
//

#include "database_data_source.hpp"

namespace OpenEarth {
    namespace Storage {

        DatabaseDataSource::DatabaseDataSource() {

        }

        DatabaseDataSource::~DatabaseDataSource() {

        }

        unique_ptr<DatabaseDataSource> DatabaseDataSource::newIntance(){
            unique_ptr<DatabaseDataSource> ptr = make_unique<DatabaseDataSource>();
            return ptr;
        }
    }
}

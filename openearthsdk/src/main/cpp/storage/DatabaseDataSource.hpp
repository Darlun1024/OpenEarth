//
// Created by GXSN-Pro on 2018/1/18.
//

#ifndef OPENEARTH_DATABASE_DATA_SOURCE_HPP
#define OPENEARTH_DATABASE_DATA_SOURCE_HPP

#include <sqlite3>
#include <memory>

namespace OpenEarth {
    namespace Storage {
        using namespace std;

        class DatabaseDataSource {
        private:
            static unique_ptr<DatabaseDataSource> singleDataBaseSource;

            DatabaseDataSource();

            ~DatabaseDataSource();

        public:
            static unique_ptr<DatabaseDataSource> singlePtr(){
                if(singleDataBaseSource == nullptr){
                    singleDataBaseSource = make_unique<DatabaseDataSource>();
                }
                return singleDataBaseSource;
            }
        };
    }
}


#endif //OPENEARTH_DATABASE_DATA_SOURCE_HPP

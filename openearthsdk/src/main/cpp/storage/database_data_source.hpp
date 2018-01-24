//
// Created by GXSN-Pro on 2018/1/18.
// 网络缓存的数据
//

#ifndef OPENEARTH_DATABASE_DATA_SOURCE_HPP
#define OPENEARTH_DATABASE_DATA_SOURCE_HPP

#include <memory>
#include "../earth/tile.hpp"
#include "../sqlite/sqlite3pp.h"

namespace OpenEarth {
    namespace Storage {
        using namespace std;

        class DatabaseDataSource {
        protected:
            string mDBPath;
            void initDatabase();
        public:
            DatabaseDataSource& operator= (const DatabaseDataSource&);
            DatabaseDataSource(string path);
            ~DatabaseDataSource();
            static std::shared_ptr<DatabaseDataSource> newInstance(string path);
            /**
             * 从缓存数据库加载数据
             * @param url
             * @return
             */
            char* find(string url);
        };
    }
}


#endif //OPENEARTH_DATABASE_DATA_SOURCE_HPP

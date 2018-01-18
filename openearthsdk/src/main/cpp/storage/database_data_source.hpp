//
// Created by GXSN-Pro on 2018/1/18.
//

#ifndef OPENEARTH_DATABASE_DATA_SOURCE_HPP
#define OPENEARTH_DATABASE_DATA_SOURCE_HPP

#include <memory>

namespace OpenEarth {
    namespace Storage {
        using namespace std;

        class DatabaseDataSource {
        private:

        public:
            DatabaseDataSource();
            ~DatabaseDataSource();

        public:
            static std::unique_ptr<DatabaseDataSource> newIntance();
        };
    }
}


#endif //OPENEARTH_DATABASE_DATA_SOURCE_HPP

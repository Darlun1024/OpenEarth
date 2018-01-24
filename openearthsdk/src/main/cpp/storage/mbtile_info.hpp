//
// Created by GXSN-Pro on 2017/11/16.
//

#ifndef ANDROID_MBTILE_INFO_HPP
#define ANDROID_MBTILE_INFO_HPP

#include <memory>
#include <string>
#include "../sqlite/sqlite3pp.h"

using namespace std;
namespace OpenEarth{
    namespace Storage{
        class LevelInfo{
        public:
            LevelInfo(int level,int minX,int maxX,int minY,int maxY){
                this->maxX = maxX;
                this->maxY = maxY;
                this->minX = minX;
                this->minY = minY;
                this->level = level;
            }

            bool  isExists(int x,int y){
                return x<=maxX&&x>=minX&&y<=maxY&&y>=minY;
            }


        protected:
            int minX,minY,maxX,maxY,level;
        };

        class MBTileInfo{

        public:
            MBTileInfo(shared_ptr<sqlite3pp::database> db);
            ~MBTileInfo();
            bool isExists(int level,int x,int y);
        protected:
            int minLevel,maxLevel;
            LevelInfo** levels;
        };
    }
}




#endif //ANDROID_MBTILE_INFO_HPP

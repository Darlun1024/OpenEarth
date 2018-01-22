//
// Created by GXSN-Pro on 2017/11/16.
//

#include "mbtile_info.hpp"

#include <map>

namespace  OpenEarth{
    namespace  Storage{
        MBTileInfo::~MBTileInfo(){
            delete levels;
        }

        MBTileInfo::MBTileInfo (shared_ptr<sqlite3pp::database> db)  {
            sqlite3pp::query qry(*db.get(), "SELECT minLevel, maxLevel FROM metadata");
            int colCount = qry.column_count();
            for (sqlite3pp::query::iterator it = qry.begin(); it != qry.end(); ++it) {
                    minLevel = (*it).get<int>(0);
                    minLevel = (*it).get<int>(1);
            }
//

            levels = new LevelInfo*[maxLevel+1];
            qry.reset();
            qry.prepare("select level,minX,minY,maxX,maxY from level_info");
            for (sqlite3pp::query::iterator it = qry.begin(); it != qry.end(); ++it) {
                int level = (*it).get<int>(0);
                int minx  = (*it).get<int>(1);
                int miny  = (*it).get<int>(2);
                int maxx  = (*it).get<int>(3);
                int maxy  = (*it).get<int>(4);
                LevelInfo* levelInfo = new LevelInfo(level,minx,maxx,miny,maxy);
                levels[level] = levelInfo;
            }

        }

        bool MBTileInfo::isExists(int level,int x,int y){
            if(level >= minLevel && level <= maxLevel){
                LevelInfo* info = levels[level];
                return  info->isExists(x,y);
            }
            return false;
        }
    }
}

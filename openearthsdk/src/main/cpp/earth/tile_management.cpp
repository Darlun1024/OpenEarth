//
// Created by GXSN-Pro on 2017/12/26.
//

#include "tile_management.hpp"

namespace OpenEarth{
    namespace  Earth{
        class TileManagement::Impl{
            struct HashValue{
                long accessTime;
                Tile* tile;
            };
        private:
            static const int max_size = 90;
            unique_ptr<map<string, OpenEarth::Tile*>> mTileMap;

            void checkCapacity(){
                if(mTileMap->size() <= max_size)return;
                map<string,Tile*>::iterator it;
                it = mTileMap->begin();
                //TODO 清理数据
            }

        public:
            Impl(){
                mTileMap = make_unique<std::map<string,Tile*>>();
            }
            ~Impl(){
                mTileMap.reset();
            }

            Tile* getTile(int zoom,int x,int y){
                string key  = OpenEarth::Tile::genUniqueCode(zoom,x,y);
                Tile* tile = mTileMap->at(key);
                if(tile== nullptr)
                    tile = new Tile(zoom,x,y);
                mTileMap->insert(std::pair<string,Tile*>(key,tile));
                return tile;
            }
        };


        TileManagement::TileManagement():impl(new Impl()){

        }

        TileManagement::~TileManagement(){

        }

        Tile* TileManagement::getTile(int zoom,int x,int y){
            return impl->getTile(zoom,x,y);
        }


    }
}

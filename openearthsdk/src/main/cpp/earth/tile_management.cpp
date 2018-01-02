//
// Created by GXSN-Pro on 2017/12/26.
//

#include <cmath>
#include "tile_management.hpp"
#include "../util/point.hpp"
#include "../texture/texture.hpp"
#include <vector>
#include <mutex>

namespace OpenEarth {
    static const char *const TAG = "TileManagement";

    class TileManagement::Impl {
        struct HashValue {
            long accessTime;
            Tile *tile;
        };
    private:
        static const int8_t max_size = 90;
        unique_ptr<map<string, shared_ptr<Tile>>> mTileMap;
        unique_ptr<vector<shared_ptr<Tile>>> mTileArray;
        unique_ptr<Texture> mTextureManager;
        std::mutex mMutex;
        void checkCapacity() {
            if (mTileMap->size() <= max_size)return;
            map<string, shared_ptr<Tile>>::iterator it;
            it = mTileMap->begin();
            //TODO 清理数据
        }


    public:
        Impl() {
            mTileMap = make_unique<std::map<string, shared_ptr<Tile>>>();
            mTileArray = make_unique<vector<shared_ptr<Tile>>>();
            mTextureManager = make_unique<Texture>();
        }

        ~Impl() {
            mTileMap.reset();
            mTileArray->clear();
            mTileArray.reset();
            mTextureManager.reset();
        }

        Tile *getTile(int8_t zoom, uint32_t x, uint32_t y) {
            string key = OpenEarth::Tile::genUniqueCode(zoom, x, y);
            shared_ptr<Tile> tile = mTileMap->find(key)->second;
            if (tile == nullptr)
                tile = std::make_shared<Tile>(zoom, x, y);
            mTileMap->insert(std::pair<string, shared_ptr<Tile>>(key, tile));
            return tile.get();
        }


        void update(int8_t zoom, OpenEarth::Geometry::Bounds bounds) {
            mMutex.lock();
            mTileArray->clear();
            PointI nw = latLng2TileXY(bounds.top, bounds.left, zoom);
            PointI se = latLng2TileXY(bounds.bottom, bounds.right, zoom);
            uint32_t minx = nw.x;
            uint32_t maxx = se.x;
            uint32_t miny = nw.y;
            uint32_t maxy = se.y;
            map<string, shared_ptr<Tile>>::iterator it;
            for (uint32_t x = minx; x <= maxx; x++) {
                for (int y = miny; y <= maxy; y++) {
                    shared_ptr<Tile> tile;
                    string key = OpenEarth::Tile::genUniqueCode(zoom, x, y);
                    it = mTileMap->find(key);
                    if(it == mTileMap->end()){
                        tile = std::make_shared<Tile>(x, y, zoom);
                        mTileMap->insert(std::pair<string, shared_ptr<Tile>>(key, tile));
                    }else{
                        tile = it->second;
                        if(tile == nullptr){
                            tile = std::make_shared<Tile>(x, y, zoom);
                            mTileMap->insert(std::pair<string, shared_ptr<Tile>>(key, tile));
                        }
                    }
                    tile = std::make_shared<Tile>(x, y, zoom);
                    mTileArray->push_back(tile);
                }
            }
            mMutex.unlock();
        }

        void draw(JNIEnv* env,GLuint  aPositionLocation, GLuint aTextureLocation,Source::Source* source){
            int length = mTileArray->size();
            vector<shared_ptr<Tile>>::iterator it = mTileArray->begin();
            mMutex.lock();
            while (it!=mTileArray->end()){
                if(*it == nullptr)continue;
                shared_ptr<Tile> tile = *it;
                GLuint textureId = mTextureManager->loadFromNet(env,source->getURLOfTile(tile.get()).c_str());
                tile->draw(aPositionLocation, aTextureLocation, textureId);
                it++;
            }
            mMutex.unlock();
        }

        PointI latLng2TileXY(float lat, float lon, int zoom) {
            int  max= std::pow(2,zoom);
            float tileSpan = 360.0f / std::pow(2, zoom);
            int x = (lon + 180) / tileSpan;
            int y = (90 - lat) / tileSpan;
            x = min(x,max-1);
            y = min(y,max/2-1);
            return PointI{
                    x,
                    y
            };
        }

    };


    TileManagement::TileManagement() : impl(new Impl()) {

    }

    TileManagement::~TileManagement() {

    }

    Tile *TileManagement::getTile(int8_t zoom, uint32_t x, uint32_t y) {
        return impl->getTile(zoom, x, y);
    }

    void TileManagement::update(int8_t zoom, OpenEarth::Geometry::Bounds bounds) {
        impl->update(zoom, bounds);
    }

    void TileManagement::draw(JNIEnv* env,GLuint  aPositionLocation, GLuint aTextureLocation,Source::Source* source){
        impl->draw(env,aPositionLocation,aTextureLocation,source);
    }


}

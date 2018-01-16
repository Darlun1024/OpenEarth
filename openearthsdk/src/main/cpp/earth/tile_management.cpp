//
// Created by GXSN-Pro on 2017/12/26.
//

#include <cmath>
#include "tile_management.hpp"
#include "../util/point.hpp"
#include "../texture/texture.hpp"
#include "../logging.hpp"
#include <vector>
#include <mutex>
#include <assert.h>

namespace OpenEarth {
    static const char *const TAG = "TileManagement";

    class TileManagement::Impl {
        struct TileCache {
            long accessTime;
            Tile *tile;
        };
    private:
        static const int8_t max_size = 90;
        unique_ptr<map<string, shared_ptr<Tile>>> mTileMap;
        unique_ptr<vector<shared_ptr<Tile>>> mTileArray;
        unique_ptr<Texture> mTextureManager;
        std::mutex mMutex;

        void checkCapacity(int8_t zoom) {
            if (mTileMap->size() <= max_size)return;
            map<string, shared_ptr<Tile>>::iterator it;
            it = mTileMap->begin();
            //TODO 清理数据
            while (it != mTileMap->end()) {
                if (it->second->z != zoom) {
                    LOGE(TAG, "erase tile %d_%d_%d", it->second->z, it->second->x, it->second->y);
                    mTileMap->erase(it);
                }
                it++;
            }
        }


    public:
        Impl() {
            mTileMap = make_unique<std::map<string, shared_ptr<Tile>>>();
            mTileArray = make_unique<vector<shared_ptr<Tile>>>();
            mTextureManager = make_unique<Texture>();
        }

        ~Impl() {
            mTileMap->clear();
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
            uint32_t MAX_X = std::pow(2,zoom)-1;
            uint32_t MAX_Y = std::pow(2,zoom-1)-1;

            uint32_t minx = nw.x > 0 ? nw.x:0;
            uint32_t maxx = se.x < MAX_X ? se.x:MAX_X;
            uint32_t miny = nw.y > 0 ? nw.y:0;
            uint32_t maxy = se.y < MAX_Y ? se.y:MAX_Y;
            map<string, shared_ptr<Tile>>::iterator it;
            for (uint32_t x = minx; x <= maxx; x++) {
                for (int y = miny; y <= maxy; y++) {
                    string key = OpenEarth::Tile::genUniqueCode(zoom, x, y);
                    if (mTileMap->find(key) == mTileMap->end()) {
                        mTileMap->insert(std::pair<string, shared_ptr<Tile>>(key, std::make_shared<Tile>(x, y, zoom)));
                    }
//                    shared_ptr<Tile> tile = std::make_shared<Tile>(x, y, zoom);
                    mTileArray->push_back(mTileMap->find(key)->second);
//                    LOGE(TAG, "tile count %d", tile.use_count());
                }
            }
            mMutex.unlock();
            checkCapacity(zoom);
        }

        void draw(JNIEnv *env, GLuint aPositionLocation, GLuint aTextureLocation,
                  Source::Source *source,AAssetManager* mgr) {
            mTextureManager->setJEnv(env);
            mMutex.lock();
            vector<shared_ptr<Tile>>::iterator it = mTileArray->begin();
            while (it != mTileArray->end()) {
                if (*it == nullptr)continue;
//                GLuint textureId = mTextureManager->loadFromAssets(mgr,"west.png");
                Tile* tile = it->get();
                if (tile) {
                    GLuint textureId = mTextureManager->loadFromNet(env, source->getURLOfTile(
                            tile).c_str());
                    if(textureId!=0)
                     tile->draw(aPositionLocation, aTextureLocation, textureId);
                }
                it++;
            }

            mMutex.unlock();
        }

        PointI latLng2TileXY(float lat, float lon, int zoom) {
            assert(-180<=lon && lon<=180);
            assert(-90 <=lat && lat<= 90 );
            assert(zoom >= 0 );
            int max = std::pow(2, zoom);
            float tileSpan = 360.0f / std::pow(2, zoom);
            int x = (lon + 180) / tileSpan;
            int y = (90 - lat) / tileSpan;
            x = min(x, max - 1);
            y = min(y, max / 2 - 1);
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

    void TileManagement::draw(JNIEnv *env, GLuint aPositionLocation, GLuint aTextureLocation,
                              Source::Source *source,AAssetManager* mgr) {
        impl->draw(env, aPositionLocation, aTextureLocation, source,mgr);
    }


}

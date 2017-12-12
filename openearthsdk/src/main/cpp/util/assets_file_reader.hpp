//
// Created by GXSN-Pro on 2017/11/28.
//

#ifndef OPENEARTH_ASSETS_FILE_READER_HPP
#define OPENEARTH_ASSETS_FILE_READER_HPP

#include <assert.h>
#include <android/asset_manager.h>

typedef struct {
    const long data_length;
    const void *data;
    const void *file_handle;
}FileData;

namespace OpenEarth {
    namespace util {
        class AssetsFileReader {
        public:
            static FileData get_file_data(const char *path);
            static void release_file_data(const FileData *file_data);
            static FileData get_asset_data(const char* relative_path, AAssetManager *assetManager);
            static  void release_asset_data(const FileData* file_data);
        };
    }
}


#endif //OPENEARTH_ASSETS_FILE_READER_HPP

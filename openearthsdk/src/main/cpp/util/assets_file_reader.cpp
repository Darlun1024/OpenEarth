//
// Created by GXSN-Pro on 2017/11/28.
//

#include <cstdio>
#include <cstdlib>
#include "assets_file_reader.hpp"

FileData OpenEarth::util::AssetsFileReader::get_asset_data(const char *path, AAssetManager* assetManager){
    assert(path != NULL);
    AAsset* asset =
            AAssetManager_open(assetManager, path, AASSET_MODE_STREAMING);
    assert(asset != NULL);

    return (FileData) { AAsset_getLength(asset), AAsset_getBuffer(asset), asset };
}

void OpenEarth::util::AssetsFileReader::release_asset_data(const FileData *file_data){
    assert(file_data != NULL);
    assert(file_data->file_handle != NULL);
    AAsset_close((AAsset*)file_data->file_handle);
}

FileData OpenEarth::util::AssetsFileReader::get_file_data(const char *path){
    assert(path != NULL);

    FILE* stream = fopen(path, "r");
    assert (stream != NULL);

    fseek(stream, 0, SEEK_END);
    long stream_size = ftell(stream);
    fseek(stream, 0, SEEK_SET);

    void* buffer = malloc(stream_size);
    fread(buffer, stream_size, 1, stream);

    assert(ferror(stream) == 0);
    fclose(stream);

    return (FileData) {stream_size, buffer, NULL};

}

void OpenEarth::util::AssetsFileReader::release_file_data(const FileData *file_data){
    assert(file_data != NULL);
    assert(file_data->data != NULL);

    free((void*)file_data->data);
}

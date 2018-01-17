#include <string>
#include <cstdlib>
#include <algorithm>    // std::find
#include "texture.hpp"
#include "../util/assets_file_reader.hpp"
#include "../storage/http_data_source.hpp"
#include "../logging.hpp"

extern "C" {
#include "../util/image.h"
#include "../util/jpeg_reader.h"
#include "../util/png_reader.h"
}

namespace  OpenEarth{

}
OpenEarth::Texture::Texture() {
    mHttpDataSource = new OpenEarth::Storage::HttpDataSource();
    mMap = std::make_unique<std::map<string, RawImageData>>();
    mRequestQueue = std::make_unique<vector<string>>();
    mWaitingRequestQueue=std::make_unique<vector<string>>();
}

OpenEarth::Texture::~Texture() {
    mMap.reset();
    mRequestQueue.reset();
    mWaitingRequestQueue.reset();
}

GLuint genTexture(RawImageData *data) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    assert(textureId != 0);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//超出图片范围，不重复
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
            GL_TEXTURE_2D, 0, data->gl_color_format, data->width, data->height, 0,
            data->gl_color_format, GL_UNSIGNED_BYTE, data->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

GLuint OpenEarth::Texture::loadFromAssets(AAssetManager *amgr, const char *path) {
    FileData fileData = OpenEarth::util::AssetsFileReader::get_asset_data(path, amgr);
    RawImageData *data;

    std::string spath = path;
    int extIndex = spath.find(".");
    std::string ext = spath.substr(extIndex + 1, spath.length() - extIndex - 1);
    if (ext.compare("png") == 0) {
        RawImageData dataPng = get_raw_image_data_from_png(fileData.data,
                                                           (int) fileData.data_length);
        data = &dataPng;
    } else if (ext.compare("jpg") == 0 || ext.compare("jpeg") == 0) {
        RawImageData dataJpg = get_raw_image_data_from_jpeg(fileData.data,
                                                            (int) fileData.data_length);
        data = &dataJpg;
    } else {
        return 0;
    }
    OpenEarth::util::AssetsFileReader::release_asset_data(&fileData);
    GLuint textureId = genTexture(data);
    release_raw_image_data(data);
    return textureId;
}

GLuint OpenEarth::Texture::loadFormFile(const char *filePath) {
    //RawImageData data = decompressJpegFromFile("/storage/emulated/0/west.jpeg");
    return 0;
}

GLuint OpenEarth::Texture::loadFromNet(JNIEnv *env, const char *url) {
    string surl = url;
    std::map<string, RawImageData>::iterator it;
    //首先从内存找，然后从本地文件系统缓存找，最后再从网上下载
    it = mMap->find(surl);
    if (it != mMap->end()) {
        RawImageData data = it->second;
        return genTexture(&data);
    } else {
        vector<string>::iterator it = std::find(mRequestQueue->begin(), mRequestQueue->end(), url);
        if (it == mRequestQueue->end()) {
            if(mRequestQueue->size()>MAX_HTTP_REQUEST_SIZE){
                //进入等待队列
                vector<string>::iterator wit = std::find(mRequestQueue->begin(), mRequestQueue->end(),
                                                        surl);
                if(wit == mWaitingRequestQueue->end()){
                     mWaitingRequestQueue->push_back(surl);
                    LOGE("waiting","%s",url);
                }
            }else{
                OpenEarth::Storage::HttpDataSource::request(env, url, this);
                mRequestQueue->push_back(surl);
            }
        }
        return 0;
    }
}

void OpenEarth::Texture::onResponse(HttpResponse response) {
    RawImageData dataPng = get_raw_image_data(response.byteArray, response.length);
    mMap->insert(pair<string, RawImageData>(response.url, dataPng));
    vector<string>::iterator it = std::find(mRequestQueue->begin(), mRequestQueue->end(),
                                            response.url);
    LOGE("texture respone","%s",response.url.c_str());
    if (it != mRequestQueue->end()) {
        mRequestQueue->erase(it);
    }
    next();
//     release_raw_image_data(&dataPng);
//     free(response.byteArray);
}

void OpenEarth::Texture::onFailure(int code, string url, string message) {
    vector<string>::iterator it = std::find(mRequestQueue->begin(), mRequestQueue->end(), url);
    if (it != mRequestQueue->end()) {
        mRequestQueue->erase(it);
    }
    next();
}

/**
 * 取消请求
 * @param url
 */
void  OpenEarth::Texture::cancel(const char* url){
    vector<string>::iterator it = std::find(mWaitingRequestQueue->begin(), mWaitingRequestQueue->end(), url);
    if (it != mWaitingRequestQueue->end()) {
        mWaitingRequestQueue->erase(it);
    }
}

void OpenEarth::Texture::next(){
    if(mWaitingRequestQueue->size()>0 && mRequestQueue->size()<=MAX_HTTP_REQUEST_SIZE){
        vector<string>::iterator it = mWaitingRequestQueue->begin();
        string url = *it;
        OpenEarth::Storage::HttpDataSource::request(mEnv,url, this);
        mRequestQueue->push_back(url);
        mWaitingRequestQueue->erase(it);
    }
}

void OpenEarth::Texture::setJEnv(JNIEnv* env){
        mEnv = env;
}



//
// Created by GXSN-Pro on 2018/1/20.
//

#include <map>
#include "mbtile_data_source.hpp"
#include "../sqlite/sqlite3pp.h"
#include "mbtile_info.hpp"
#include "../util/url.hpp"
#include "Respone.hpp"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

namespace OpenEarth{
    namespace Storage{
        using namespace std;
        using namespace sqlite3pp;

        const char *protocol = "mbtile://";
        const uint8_t protocolLength = 9;

        unique_ptr<database> db;
        unique_ptr<map<string,shared_ptr<database>>> databaseMap;
        unique_ptr<map<string,shared_ptr<MBTileInfo>>> tileInfoMap;

       // /storage/emulated/0/
        MBTileDataSource::MBTileDataSource(){
           databaseMap = make_unique<map<string,shared_ptr<database>>>();
           tileInfoMap = make_unique<map<string,shared_ptr<MBTileInfo>>>();
        }

        MBTileDataSource::~MBTileDataSource(){
            databaseMap.reset();
        }

        std::unique_ptr<MBTileDataSource> MBTileDataSource::newInstance(){
            std::unique_ptr<MBTileDataSource> mbDataSource = std::make_unique<MBTileDataSource>();
            return mbDataSource;
        }

        Response* MBTileDataSource::request(std::string url){
            //TODO
            string dbPath = "";
//            shared_ptr<database> db = ;
            std::string path = OpenEarth::Util::percentDecode(url.substr(protocolLength));
            //解析url
            vector<string> vStr;
            boost::split(vStr, path, boost::is_any_of("&"), boost::token_compress_on);
            int x,y,z;
            for (vector<string>::iterator it = vStr.begin(); it != vStr.end(); ++it) {
                string keyValue = *it;
                vector<string> kv;
                boost::split(kv, keyValue, boost::is_any_of("="), boost::token_compress_on);
                if(kv[0].compare("x")==0){
                    x = atoi(kv[1].c_str());
                }else if(kv[0].compare("y")==0){
                    y = atoi(kv[1].c_str());
                } else if(kv[0].compare("l")==0){
                    z = atoi(kv[1].c_str());
                }else if(kv[0].compare("path")==0){
                    dbPath = kv[1];
                }
            }

            shared_ptr<database> db;
            shared_ptr<MBTileInfo> tileInfo;

            map<string, shared_ptr<database>>::iterator it;
            map<string, shared_ptr<MBTileInfo>>::iterator itInfo;
            it = databaseMap->find(dbPath);
            itInfo = tileInfoMap->find(dbPath);
            if(it!=databaseMap->end()){
                db = it->second;
                tileInfo = itInfo->second;
            }else{
                db =  std::make_shared<database>(dbPath.c_str(),SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);
                databaseMap->insert(pair<string,shared_ptr<database>>(dbPath,db));
                tileInfo = std::make_shared<MBTileInfo>(db);
                tileInfoMap->insert(pair<string,shared_ptr<MBTileInfo>>(dbPath,tileInfo));
            }

            Response* response = new Response();
            response->url=url;
            if(!tileInfo->isExists(z,x,y)){
                response->length = 0;
            }else{
                sqlite3pp::query query(*db.get(),"SELECT tile_data FROM tiles WHERE zoom_level = ? AND tile_column = ? AND tile_row = ?");
                query.bind(1,z);
                query.bind(2,x);
                query.bind(3,y);
                sqlite3pp::query::iterator it = query.begin();
                if(it!=query.end()){
                    const char* bytes = (*it).get<const char *>(0);
                    int length = (*it).column_bytes(0);
                    char* newBytes = new char[length];
                    memcpy(newBytes, bytes, length);
                    response->length = length;
                    response->byteArray = newBytes;

                }
            }
            return  response;

        }

        bool MBTileDataSource::isValidUrl(const std::string &url) {
            return url.compare(0, protocolLength, protocol) == 0;
        }


    }
}

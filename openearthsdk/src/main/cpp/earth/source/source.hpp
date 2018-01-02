//
// Created by GXSN-Pro on 2018/1/2.
//

#ifndef OPENEARTH_SOURCE_HPP
#define OPENEARTH_SOURCE_HPP


#include <string>
#include "../tile.hpp"
#include "../../util/util.hpp"

namespace  OpenEarth{
    namespace Source{
        using namespace std;
        class Source {
        private:
            string mTemplateUrl;
        public:
            /**
             * http://xxxxx/{x}/{y}/{z}
             * @param templateUrl
             */
            Source(string templateUrl){
                mTemplateUrl = templateUrl;
            }
            ~Source(){

            }
            std::string getURLOfTile(Tile* tile){
                if(tile == nullptr)return "";
                if(mTemplateUrl.empty()) return "";
                string url = mTemplateUrl;
                url = Util::replace_all(url,"{x}",Util::int2String(tile->x));
                url = Util::replace_all(url,"{y}",Util::int2String(tile->y));
                url = Util::replace_all(url,"{z}",Util::int2String(tile->z));
                return url;
            }
        };
    }
}



#endif //OPENEARTH_SOURCE_HPP

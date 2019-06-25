//
// Created by GXSN-Pro on 2018/1/24.
//

#ifndef OPENEARTH_LINE_FEATURE_HPP
#define OPENEARTH_LINE_FEATURE_HPP


#include "feature.hpp"
#include "../geometry/line.hpp"

using namespace OpenEarth::Geometry;
namespace OpenEarth{
    namespace Features{
        class LineFeature: public Feature {
        private:
            Line *line;
        public:
            LineFeature();
            ~LineFeature();
            void setLine(Line* line);
            Line* getLine() const;
        };
    }
}


#endif //OPENEARTH_LINE_FEATURE_HPP

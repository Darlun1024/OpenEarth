//
// Created by GXSN-Pro on 2018/2/9.
//

#include <glm/vec4.hpp>
#include <GLES2/gl2.h>
#include <glm/ext.hpp>
#include "line_layer.hpp"
#include "../geometry/line.hpp"
#include "../feature/line_feature.hpp"
#import "../transform.hpp"

using namespace OpenEarth::Geometry;
using namespace OpenEarth::Features;
namespace OpenEarth {
    namespace Layers {
        LineLayer::LineLayer(const std::string &layerId, const std::string &name,
                             const std::string &sourceId) : VectorLayer(layerId, name,
                                                                        sourceId) {

        }

        void LineLayer::draw(int aColorLocation,int aPositionLocation){
            size_t size = mFeatures->size();
            if(size  < 1)
                return;
            const LineFeature* lineFeature = (LineFeature*)*mFeatures.get()->begin();
            Line* line = lineFeature->getLine();
            long count = 0;
            float* points = line->getVertexAttribArray(&count);

            glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT),
                                  points);
            glEnableVertexAttribArray(aPositionLocation);
            glDrawArrays(GL_LINE_STRIP,0,count);
        }
    }
}

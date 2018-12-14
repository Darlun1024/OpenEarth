//
// Created by GXSN-Pro on 2018/2/9.
//

#include <glm/vec4.hpp>
#include <GLES2/gl2.h>
#include <glm/ext.hpp>
#include "line_layer.hpp"

namespace OpenEarth {
    namespace Layers {
        LineLayer::LineLayer(const std::string &layerId, const std::string &name,
                             const std::string &sourceId) : VectorLayer(layerId, name,
                                                                        sourceId) {

        }

        void LineLayer::draw(int aColorLocation,int aPositionLocation){
            int size = mFeatures->size();
            glm::vec4 color = glm::vec4(0.0f,1.0f,1.0f,0.6f);
            glUniform4fv(aColorLocation,1,glm::value_ptr(color));
            float points[] = {0.0f,0.0f,-2.0f,1.0f,1.0f,-2.0f};
            glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT),
                                  points);
            glEnableVertexAttribArray(aPositionLocation);
            glDrawArrays(GL_LINE_STRIP,0,2);
        }
    }
}

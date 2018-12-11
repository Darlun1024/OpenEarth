//
// Created by GXSN-Pro on 2018/10/26.
//

#ifndef OPENEARTH_ENVELOPE_HPP
#define OPENEARTH_ENVELOPE_HPP

namespace OpenEarth {
    namespace Geometry {
        class Envelope {
        public:
            Envelope(double minx,double miny,double maxx,double maxy);
            ~Envelope();
            double minX;
            double minY;
            double maxX;
            double maxY;
        };
    }
}


#endif //OPENEARTH_ENVELOPE_HPP

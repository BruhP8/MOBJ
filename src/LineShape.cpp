#include "LineShape.h"

namespace Netlist
{
    using namespace std;

    Box LineShape::getBoundingBox() const {
        // TODO
    }

    void LineShape::toXml(ostream& stream){
        stream << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_   
               << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>\n";
    }
} // namespace Netlist

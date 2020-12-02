#include "LineShape.h"

namespace Netlist
{
    using namespace std;

    LineShape::LineShape(Symbol * s, int x1, int y1, int x2, int y2)
        :Shape(s), x1_(x1), y1_(y1), x2_(x2), y2_(y2)
    {
    }
    
    LineShape::~LineShape()
    {
    }


    Box LineShape::getBoundingBox() const {
        // TODO
        return Box(x1_, y1_, x2_, y2_);
    }

    void LineShape::toXml(ostream& stream){
        stream << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_   
               << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>\n";
    }

    LineShape* LineShape::fromXml( Symbol * symb, xmlTextReaderPtr reader)
    {
        LineShape* lineshape = NULL;

        return lineshape;
    }

} // namespace Netlist

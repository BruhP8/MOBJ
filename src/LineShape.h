#ifndef NETLIST_LINESHAPE_H
#define NETLIST_LINESHAPE_H

#include "Shape.h"
#include <iostream>

namespace Netlist
{   
    class Shape;

    class LineShape : public Shape
    {
        private:
            int x1_, y1_, x2_, y2_;
        public:
                                LineShape         (Symbol * s, int x1, int y1, int x2, int y2);
                                ~LineShape        ();
                    Box         getBoundingBox    () const;
            static  LineShape * fromXml           (Symbol *, xmlTextReaderPtr);
                    void        toXml             (std::ostream&);
    };
    
    LineShape::LineShape(Symbol * s, int x1, int y1, int x2, int y2)
              :Shape(s), x1_(x1), y1_(y1), x2_(x2), y2_(y2)
    {
    }
    
    LineShape::~LineShape()
    {
    }
    
} // namespace Netlist
#endif

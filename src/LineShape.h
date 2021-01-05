#ifndef NETLIST_LINESHAPE_H
#define NETLIST_LINESHAPE_H

#include "Shape.h"
#include "Symbol.h"
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
            inline  Point       startPoint        () const;
            inline  Point       endPoint          () const;
                    Box         getBoundingBox    () const;
            static  LineShape * fromXml           (Symbol *, xmlTextReaderPtr);
                    void        toXml             (std::ostream&);
    };

    inline Point LineShape::startPoint() const { return Point(x1_, y1_); }
    inline Point LineShape::endPoint  () const { return Point(x2_, y2_); }

} // namespace Netlist
#endif

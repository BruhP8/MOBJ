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
                    Box         getBoundingBox    () const;
            static  LineShape * fromXml           (Symbol *, xmlTextReaderPtr);
                    void        toXml             (std::ostream&);
    };

} // namespace Netlist
#endif

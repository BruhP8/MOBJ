#ifndef NETLIST_ARCSHAPE_H
#define NETLIST_ARCSHAPE_H

#include "Shape.h"
#include "Symbol.h"
#include <iostream>

namespace Netlist
{
  class Shape;

  class ArcShape : public Shape 
  {
    private :
      int start_, span_;
      Box box_;

    public :
                        ArcShape        ( Symbol* s, int x1, int y1, int x2, int y2, int start, int span );
                       ~ArcShape        ();
              Box       getBoundingBox  () const;
      inline  int       startAngle      () const;
      inline  int       spanAngle       () const;
      static  ArcShape* fromXml         ( Symbol*, xmlTextReaderPtr );
              void      toXml           ( std::ostream& );
  };

  inline  int  ArcShape::startAngle() const { return 16*start_; }
  inline  int  ArcShape::spanAngle () const { return 16*span_; }
} // namespace Netlist

#endif
#ifndef NETLIST_ELLIPSESHAPE_H
#define NETLIST_ELLIPSESHAPE_H

#include "Shape.h"
#include "Symbol.h"

namespace Netlist
{

  class Shape;

  class EllipseShape : public Shape
  {

    private :
      Box box_;

    public :
      EllipseShape(Symbol*, int x1, int y1, int x2, int y2);
      ~EllipseShape();

      Box getBoundingBox() const;

      void toXml ( std::ostream& );
      static EllipseShape* fromXml( Symbol*, xmlTextReaderPtr );

  };

} //namespace Netlist

#endif
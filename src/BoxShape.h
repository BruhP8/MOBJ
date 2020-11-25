#ifndef NETLIST_BOXSHAPE_H
#define NETLIST_BOXSHAPE_H

#include "Shape.h"


namespace Netlist {

  class Shape;

  class BoxShape:public Shape {

    private :
      Box   box_;

    public :

      BoxShape( Symbol* symbol, const Box& );
      BoxShape( Symbol* symbol, int x1, int y1, int x2, int y2 );
     ~BoxShape();
      Box getBoundingBox() const;
      static  BoxShape*  fromXml( Symbol*, xmlTextReaderPtr );
              void       toXml  ();

  };

}

#endif
#include "BoxShape.h"

namespace Netlist {

  using namespace std;

  BoxShape::BoxShape( Symbol* symbol, int x1, int y1, int x2, int y2 )
  : Shape(symbol), box_(x1, y1, x2, y2)
  {}

  BoxShape::BoxShape( Symbol* symbol, const Box& box )
  : Shape(symbol), box_(box)
  {}

  BoxShape::~BoxShape()
  {}

  Box BoxShape::getBoundingBox() const
  {
    return box_;
  }

  BoxShape* BoxShape::fromXml( Symbol* symbol, xmlTextReaderPtr reader )
  {
    BoxShape* boxshape = NULL;

    return boxshape;
  }

  void  BoxShape::toXml()
  {
    cout << "<Je suis dans BoxShape>" << endl;
  }

}
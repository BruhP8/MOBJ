#include "EllipseShape.h"
#include "XmlUtil.h"

namespace Netlist
{
  using namespace std;

  EllipseShape::EllipseShape( Symbol* s, int x1, int y1, int x2, int y2)
  :Shape(s), box_(x1, y1, x2, y2)
  {}

  EllipseShape::~EllipseShape()
  {}

  Box EllipseShape::getBoundingBox() const {
    return box_;
  }

  void EllipseShape::toXml( ostream& os ){
    os << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1()
       << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>";
  }

  EllipseShape* EllipseShape::fromXml( Symbol* symb, xmlTextReaderPtr reader )
  {
    EllipseShape* esh = NULL;
    
    const xmlChar* ellipseTag = xmlTextReaderConstString    ( reader, (const xmlChar*)"ellipse" );
    const xmlChar* nodeName   = xmlTextReaderConstLocalName ( reader );

    if (nodeName == ellipseTag){
      int x1 = 0;
      int y1 = 0;
      int x2 = 0;
      int y2 = 0;
      if(! xmlGetIntAttribute( reader, "x1", x1)){
        cerr << "[ERROR] EllipseShape::fromXml() : argument x1 not found" << endl;
        return esh;
      }
      if(! xmlGetIntAttribute( reader, "y1", y1)){
        cerr << "[ERROR] EllipseShape::fromXml() : argument y1 not found" << endl;
        return esh;
      }
      if(! xmlGetIntAttribute( reader, "x2", x2)){
        cerr << "[ERROR] EllipseShape::fromXml() : argument x2 not found" << endl;
        return esh;
      }
      if(! xmlGetIntAttribute( reader, "y2", y2)){
        cerr << "[ERROR] EllipseShape::fromXml() : argument y2 not found" << endl;
        return esh;
      }

      esh = new EllipseShape(symb, x1, y1, x2, y2);

    }

    return esh;

  }

}
#include "BoxShape.h"
#include "XmlUtil.h"

namespace Netlist {

  using namespace std;

  BoxShape::BoxShape( Symbol* symbol, int x1, int y1, int x2, int y2 )
  : Shape(symbol), box_(x1, y1, x2, y2)
  { }

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
    BoxShape* bsh = NULL;

    cout << "BoxShape::fromXml()" << endl;

    const xmlChar* boxTag   = xmlTextReaderConstString    ( reader, (const xmlChar*)"box" );
    const xmlChar* nodeName = xmlTextReaderConstLocalName ( reader );

    if ( nodeName == boxTag ){
      int x1 = 0;
      int y1 = 0;
      int x2 = 0;
      int y2 = 0;

      if (! xmlGetIntAttribute( reader, "x1", x1)){
        cerr << "[ERROR] BoxShape::fromXml() : x1 argument not found" << endl;
        return bsh;
      }
      if (! xmlGetIntAttribute( reader, "y1", y1)){
        cerr << "[ERROR] BoxShape::fromXml() : y1 argument not found" << endl;
        return bsh;
      }
      if (! xmlGetIntAttribute( reader, "x2", x2)){
        cerr << "[ERROR] BoxShape::fromXml() : x2 argument not found" << endl;
        return bsh;
      }
      if (! xmlGetIntAttribute( reader, "y2", y2)){
        cerr << "[ERROR] BoxShape::fromXml() : y2 argument not found" << endl;
        return bsh;
      }

      bsh = new BoxShape(symbol, x1, y1, x2, y2);
    }

    return bsh;
  }

  void  BoxShape::toXml( ostream& os )
  {
    //cout << "<Je suis dans BoxShape>" << endl;
    Indentation ind;
    //ind++;
    os << ind << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() 
    << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>" << endl;
  }

}
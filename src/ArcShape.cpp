#include "ArcShape.h"
#include "XmlUtil.h"

namespace Netlist 
{
  using namespace std;
  
  ArcShape::ArcShape( Symbol* s, int x1, int y1, int x2, int y2, int start, int span )
  :Shape(s), start_(start), span_(span), box_(x1, y1, x2, y2)
  {}

  ArcShape::~ArcShape()
  {}

  Box ArcShape::getBoundingBox() const
  {
    return box_;
  }

  void ArcShape::toXml( std::ostream& os )
  {
    os << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1()
       << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() 
       << "\" start=\"" << start_ << "\" span=\"" << span_ << "\"/>";
  }

  ArcShape* ArcShape::fromXml( Symbol* symb, xmlTextReaderPtr reader )
  {
    ArcShape* ash = NULL;

    const xmlChar* arcTag   = xmlTextReaderConstString    ( reader, (const xmlChar*)"arc" );
    const xmlChar* nodeName = xmlTextReaderConstLocalName ( reader );

    if ( nodeName == arcTag ){
      int x1    = 0;
      int y1    = 0;
      int x2    = 0;
      int y2    = 0;
      int start = 0;
      int span  = 0;
      
      if (! xmlGetIntAttribute( reader, "x1", x1)){
        cerr << "[ERROR] ArcShape::fromXml() : argument x1 not found" << endl;
        return ash;
      }
      if (! xmlGetIntAttribute( reader, "y1", y1)){
        cerr << "[ERROR] ArcShape::fromXml() : argument y1 not found" << endl;
        return ash;
      }
      if (! xmlGetIntAttribute( reader, "x2", x2)){
        cerr << "[ERROR] ArcShape::fromXml() : argument x2 not found" << endl;
        return ash;
      }
      if (! xmlGetIntAttribute( reader, "y2", y2)){
        cerr << "[ERROR] ArcShape::fromXml() : argument y2 not found" << endl;
        return ash;
      }
      if (! xmlGetIntAttribute( reader, "start", start)){
        cerr << "[ERROR] ArcShape::fromXml() : argument start not found" << endl;
        return ash;
      }
      if (! xmlGetIntAttribute( reader, "span", span)){
        cerr << "[ERROR] ArcShape::fromXml() : argument span not found" << endl;
        return ash;
      }

      ash = new ArcShape(symb, x1, y1, x2, y2, start, span);
    }

    return ash;

  }

}

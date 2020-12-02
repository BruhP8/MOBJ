#include "LineShape.h"
#include "XmlUtil.h"

namespace Netlist
{
  using namespace std;

  LineShape::LineShape(Symbol * s, int x1, int y1, int x2, int y2)
      :Shape(s), x1_(x1), y1_(y1), x2_(x2), y2_(y2)
  {
  }
    
  LineShape::~LineShape()
  {
  }


  Box LineShape::getBoundingBox() const {
      // TODO
      return Box(x1_, y1_, x2_, y2_);
  }

  void LineShape::toXml(ostream& stream){
    stream << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_   
           << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>\n";
  }

  LineShape* LineShape::fromXml( Symbol * symb, xmlTextReaderPtr reader)
  {
    LineShape* lsh = NULL;
  
    const xmlChar* lineTag  = xmlTextReaderConstString    ( reader, (const xmlChar*)"line" );
    const xmlChar* nodeName = xmlTextReaderConstLocalName ( reader );
  
    if (nodeName == lineTag ){
      int x1 = 0;
      int y1 = 0;
      int x2 = 0;
      int y2 = 0;
      if (! xmlGetIntAttribute( reader, "x1", x1)){
        cerr << "[ERROR] LineShape::fromXml() : argument not found" << endl;
        return lsh;
      }
      if (! xmlGetIntAttribute( reader, "y1", y1)){
        cerr << "[ERROR] LineShape::fromXml() : argument not found" << endl;
        return lsh;
      }
      if (! xmlGetIntAttribute( reader, "x2", x2)){
        cerr << "[ERROR] LineShape::fromXml() : argument not found" << endl;
        return lsh;
      }
      if (! xmlGetIntAttribute( reader, "y2", y2)){
        cerr << "[ERROR] LineShape::fromXml() : argument not found" << endl;
        return lsh;
      }

      lsh = new LineShape(symb, x1, y1, x2, y2);
    }
  
    return lsh;
  }

} // namespace Netlist

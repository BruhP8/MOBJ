#include "TermShape.h"
#include "XmlUtil.h"
//#include <libxml/reader.h>

namespace Netlist {

  using namespace std;

  TermShape::TermShape( Symbol* symbol, string name, int x, int y, NameAlign align )
  : Shape(symbol), x_(x), y_(y), align_(align)
  { 
    term_ = symbol->getCell()->getTerm(name);
  }

  TermShape::~TermShape()
  {
    term_ = NULL;
  }

  void TermShape::toXml ( ostream& os )
  {
    //Indentation ind;
    //cout << "TermShape::toXml()" << endl;
    string strAlign = "";
    switch( align_ ){
      case 1 :
        strAlign = "top_left";
        break;
      case 2 :
        strAlign = "top_right";
        break;
      case 3 :
        strAlign = "bottom_left";
        break;
      case 4 :
        strAlign = "bottom_right";
        break;
      default:
        break;
    }
    //ind++;
    //ind++;
    //ind++;
    os << indent << "<term name=\"" << term_->getName() << "\" x1=\"" << x_ 
       << "\" y1=\"" << y_ << "\" align=\"" << strAlign << "\" />" << endl;
  }

  TermShape* TermShape::fromXml ( Symbol* symb, xmlTextReaderPtr reader )
  {
    TermShape* tsh = NULL;

    cout << "TermShape::fromXml()" << endl;

    const xmlChar* termTag  = xmlTextReaderConstString    ( reader, (const xmlChar*)"term" );
    const xmlChar* nodeName = xmlTextReaderConstLocalName ( reader );

    if ( nodeName == termTag ) {

      string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ));
      if (termName.empty()){
        cerr << "[ERROR] TermShape::fromXml() : term name not found" << endl;
      }
      int x, y;
      string argx = "x1", argy = "y1";
      bool test = xmlGetIntAttribute ( reader, argx, x );
      if (! test){
        cerr << "[ERROR] TermShape::fromXml() : calling function xmlGetIntAttribute() for arg x" << endl;
        return tsh;
      }
      test = xmlGetIntAttribute( reader, argy, y );
      if (! test){
        cerr << "[ERROR] TermShape::fromXml() : calling function xmlGetIntAttribute() for arg y" << endl;
        return tsh;
      }
      //else {
      //  cout << "-- -- x1 : " << x << ", y1 : " << y << endl;
      //}
      string strAlign = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"align" ));
      if (strAlign.empty()){
        cerr << "[ERROR] TermShape::fromXml() : arg align not found" << endl;
      }
      NameAlign align;
      if (strAlign == "top_left"){
        align = TopLeft;
      } else if (strAlign == "top_right"){
        align = TopRight;
      } else if (strAlign == "bottom_left"){
        align = BottomLeft;
      } else if (strAlign == "bottom_right"){
        align = BottomRight;
      } else {
        cerr << "[ERROR] Term::fromXml() : invalid format for align attribute" << endl;
      }
      //cout << "-- -- align : " << strAlign << endl;

      tsh = new TermShape(symb, termName, x, y, align);

      cout << "-- new TermShape created !" << endl;
    }

    return tsh;

  }

}
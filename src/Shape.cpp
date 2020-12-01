// -*- explicit-buffer-name: "Shape.cpp<M1-MOBJ/7>" -*-

#include "Shape.h"
#include "BoxShape.h"
#include "TermShape.h"
#include "XmlUtil.h"
#include <libxml/xmlreader.h>

namespace Netlist {

  using namespace std;

  Shape::Shape( Symbol* symbol )
  : owner_( symbol )
  { }

  Shape::~Shape()
  {}

  Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
  {
    cout << "-- -- Shape::fromXml() : ";

    // Factory-like method.
    const xmlChar* boxTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
    const xmlChar* ellipseTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
    const xmlChar* arcTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
    const xmlChar* lineTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
    const xmlChar* termTag
      = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
    const xmlChar* nodeName
      = xmlTextReaderConstLocalName( reader );
    cout << nodeName << endl;
    Shape* shape = NULL;
    if (boxTag == nodeName)
      shape = BoxShape::fromXml( owner, reader );
    if (ellipseTag == nodeName)
      cout << "EllipseShape" << endl;
      //shape = EllipseShape::fromXml( owner, reader );
    if (arcTag == nodeName)
      cout << "ArcShape" << endl;
      //shape = ArcShape::fromXml( owner, reader );
    if (lineTag == nodeName)
      cout << "LineShape" << endl;
      //shape = LineShape::fromXml( owner, reader );
    if (termTag == nodeName)
      shape = TermShape::fromXml( owner, reader );

    if (shape == NULL)
      cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
           << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;

    return shape;
  }

  //void  Shape::toXml ( ostream& os ){
  //  os << "Je devrais pas être là" << endl;
  //}

}
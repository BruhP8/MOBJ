#include  "Symbol.h"
#include  "TermShape.h"
#include  "BoxShape.h"
#include  "LineShape.h"


namespace Netlist {

  using namespace std;

  Symbol::Symbol( Cell* cell )
  : owner_(cell), shapes_()
  {}

  Symbol::~Symbol()
  {
    for( vector<Shape*>::iterator ishape=shapes_.begin(); ishape != shapes_.end(); ++ishape ){
      shapes_.erase(ishape);
    }
  }

  Cell* Symbol::getCell() const
  {
    return owner_;
  }

  void  Symbol::add ( Shape* sh ){
    if ( sh ){
      shapes_.push_back(sh);
    }
  }

  void  Symbol::remove ( Shape* sh ){
    if ( sh ){
      for( vector<Shape*>::iterator ish = shapes_.begin();
           ish != shapes_.end(); ++ish ){
        if (*ish == sh){
          shapes_.erase(ish);
          return;
        }
      }
    }
  }

  void  Symbol::toXml ( ostream& os ) const
  {
    os << indent << "<symbol>" << endl;
    indent++;
    for (Shape* ishape : shapes_){
      ishape->toXml(os);
    }
    indent--;
    os << indent << "</symbol>" << endl;
  }

  Symbol* Symbol::fromXml( Cell* cell, xmlTextReaderPtr reader ){
    
    cout << "Beginning of Symbol::fromXml()" << endl;

    Symbol* symbol = NULL;

    switch( xmlTextReaderNodeType( reader ) ){
      case  XML_READER_TYPE_COMMENT:
      case  XML_READER_TYPE_WHITESPACE:
      case  XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
            break;
    }

    symbol = cell->getSymbol();

    while(xmlTextReaderNodeType( reader ) != XML_READER_TYPE_END_ELEMENT){

      int status = xmlTextReaderRead(reader);
      if (status != 1) {
        if (status != 0) {
          cerr << "[ERROR] Cell::fromXml(): Unexpected termination of the XML parser." << endl;
        }
        break;
      }

      switch( xmlTextReaderNodeType( reader ) ){
        case  XML_READER_TYPE_COMMENT:
        case  XML_READER_TYPE_WHITESPACE:
        case  XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
              continue;
      }

      const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

      cout << "-- Symbol::fromXml() : " << nodeName << endl;

      Shape* sh = Shape::fromXml(symbol, reader);

      if (sh == NULL){
        cerr << "[ERROR] Symbol::fromXml() calling Shape::fromXml()" << endl;
      } else {
        symbol -> add(sh);
        cout << "C'est OK" << endl;
      }

    }
    //cout << "rien n'a été trouvé" << endl;

    cout << "End of Symbol::fromXml()" << endl;
    return symbol;
  }

}
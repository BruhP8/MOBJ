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

  Box Symbol::getBoundingBox() const
  {
    if (shapes_.empty()){
      return Box(0, 0, 0, 0);
    }
    int x1 = shapes_[0]->getBoundingBox().getX1();
    int y1 = shapes_[0]->getBoundingBox().getY1();
    int x2 = shapes_[0]->getBoundingBox().getX2();
    int y2 = shapes_[0]->getBoundingBox().getY2();
    for(Shape* s : shapes_){
      Box b = s->getBoundingBox();
      if (x1 > b.getX1()){
        x1 = b.getX1();
      }
      if (y1 > b.getY1()){
        y1 = b.getY1();
      }
      if (x2 < b.getX2()){
        x2 = b.getX2();
      }
      if (y2 < b.getY2()){
        y2 = b.getY2();
      }
    }
    return Box(x1, y1, x2, y2);
  }

  Point Symbol::getTermPosition( Term* term ) const
  {
    //TermShape* tsh = NULL;
    for(Shape* s : shapes_){
      if( TermShape* tsh = dynamic_cast<TermShape*>(s) ){
        if ( term == tsh->getTerm() ){
          cout << "[DONE] Symbol::getTermPosition() : TermShape OK" << endl;
          return Point(tsh->getX(), tsh->getY());
        }
      }
    }
    cout << "[Warning] Symbol::getTermPosition() : nothing happened" << endl;
    return Point(-1, -1);
  }

  TermShape* Symbol::getTermShape ( Term* term ) const
  {
    for(Shape* s : shapes_){
      if( TermShape* tsh = dynamic_cast<TermShape*>(s) ){
        if ( tsh->getTerm() == term ){
          return tsh;
        }
      }
    }
    cout << "Symbol::getTermShape() : TermShape not found" << endl;
    return NULL;
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
    
    const xmlChar* symbTag  = xmlTextReaderConstString( reader, (const xmlChar*)"symbol" );

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

      std::cout << "--Symbol::fromXml() : While Loop (construction of Shapes)" << std::endl;

      int status = xmlTextReaderRead(reader);
      if (status != 1) {
        if (status != 0) {
          cerr << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser." << endl;
        }
        cerr << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser." << endl;
        break;
      }

      switch( xmlTextReaderNodeType( reader ) ){
        case  XML_READER_TYPE_COMMENT:
        case  XML_READER_TYPE_WHITESPACE:
        case  XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
              continue;
      }

      const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );
      if (nodeName == symbTag){
        cerr << "[END] Symbol::fromXml(): current nodeName is Symbol." << endl;
        break;
      }

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
#include "Symbol.h"

namespace Netlist {

  using namespace std;

  Symbol::Symbol( Cell* cell )
  : owner_(cell), shapes_()
  {}

  Symbol::~Symbol()
  {}

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

}
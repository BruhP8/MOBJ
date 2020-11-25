// -*- explicit-buffer-name: "Shape.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include <libxml/xmlreader.h>
#include "Box.h"
#include "Symbol.h"
//#include "BoxShape.h"

namespace Netlist {

  //class Symbol;
  class BoxShape;

  class Shape {

    private :
      
      Symbol* owner_;

    public  :
      
               Shape  ( Symbol* );
      virtual ~Shape  ();

      inline  Symbol* getSymbol       () const;
      static  Shape*  fromXml         ( Symbol* owner, xmlTextReaderPtr reader );
      virtual void    toXml           ( std::ostream& ); //pure virtual function. 
      virtual Box     getBoundingBox  () const = 0;

    
  };

  inline  Symbol* Shape::getSymbol()  const { return owner_; }

}

#endif
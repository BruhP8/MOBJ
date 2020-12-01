#ifndef NETLIST_TERMSHAPE_H
#define NETLIST_TERMSHAPE_H

#include "Shape.h"
#include "Symbol.h"
#include "Term.h"

namespace Netlist 
{

  class Shape;

  class TermShape : public Shape 
  {

    public :
      enum  NameAlign { TopLeft=1, TopRight=2, BottomLeft=3, BottomRight=4 };

    private :
      Term*        term_;
      int         x_, y_;
      NameAlign   align_;

    public :

                          TermShape ( Symbol*, std::string name, int x, int y, NameAlign align );
                         ~TermShape ();
              
              Box         getBoundingBox  () const;

      inline  Term*       getTerm         () const;
      inline  int         getX            () const;
      inline  int         getY            () const;
      
              void        toXml           ( std::ostream& );
      static  TermShape*  fromXml         ( Symbol*, xmlTextReaderPtr );

  };

  inline  Box   TermShape::getBoundingBox  ()  const { return Box(x_, y_, x_, y_); }
  inline  Term* TermShape::getTerm         ()  const { return term_; }
  inline  int   TermShape::getX            ()  const { return x_; }
  inline  int   TermShape::getY            ()  const { return y_; }

}

#endif
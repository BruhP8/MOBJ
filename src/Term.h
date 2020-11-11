#ifndef   NETLIST_TERM_H
#define   NETLIST_TERM_H

#include <iostream>
#include "Node.h"
//#include "Net.h"
#include "Cell.h"
#include "Point.h"
#include "Instance.h"

namespace Netlist {
  
  //class Node;  
  class Net;
  //class Instance;
  //class Cell;
  
  class Term {
    
    public :
      enum Type         { Internal=1, External=2 };
      enum Direction    { In=1, Out=2, Inout=3, Tristate=4, Transcv=5, Unknown=6 };
    
    private :
      void*                   owner_;
      std::string              name_;
      Direction           direction_; 
      Type                     type_;
      Net*                      net_; 
      Node                     node_;
      Term ( Term & );
      
    public  :
    
      Term ( Cell * , const std::string & name, Direction );
      Term ( Instance * , const Term * modelTerm );
     ~Term ();
      
      static  Term*                 fromXml      ( Cell* , xmlTextReaderPtr );

      /* Prédicats et Accesseurs : */
      
      inline  bool                  isInternal   ()  const  { return type_==Internal;  }
      inline  bool                  isExternal   ()  const  { return type_==External;  }
      
      inline  const std::string&    getName      ()  const  { return name_; }
      
      /*Renvoie un pointeur sur le Node contenu dans le Term*/
      inline  Node*                 getNode      ()         { return &node_;  }
      
      /*Accesseur sur le Net propriétaire du Node contenu dans le Term courant*/
      inline  Net*                  getNet       ()  const  { return net_;  }
      
      /*Accesseur sur la Cell propriétaire*/
      inline  Cell*                 getCell      ()  const  
      { return (type_ == External) ? static_cast<Cell *>(owner_) : NULL ;  }
      
              Cell*                 getOwnerCell ()  const;
      
      /*Accesseur sur l'Instance propriétaire*/
      inline  Instance*             getInstance  ()  const  
      { return (type_ == Internal) ? static_cast<Instance *>(owner_) : NULL;  }
      
      inline  Direction             getDirection ()  const  { return direction_; }
      inline  Point                 getPosition  ()  const  { return node_.getPosition(); }
      inline  Type                  getType      ()  const  { return type_; }
      
      /* Modificateurs : */
              void                  setNet       ( Net * );
              void                  setNet       ( const std::string& );
      inline  void                  setDirection ( Direction dir )   { direction_ = dir; }
      inline  void                  setPosition  ( const Point& p )  { node_.setPosition(p); }
      inline  void                  setPosition  ( int x, int y )    { node_.setPosition(x, y); }
      
      /* Methodes du parser */
              void                  toXml        ( std::ostream& os );

  };

}


#endif

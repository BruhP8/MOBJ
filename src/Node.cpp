// -*- explicit-buffer-name: "Node.cpp<M1-MOBJ/4-5>" -*-

#include  <limits>
#include  "Node.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include  "Cell.h"

namespace Netlist {

  using namespace std;


  const size_t  Node::noid = numeric_limits<size_t>::max();


  Node::Node ( Term* term, size_t id  )
    : id_      (id)
    , term_    (term)
    , position_()
  { }

  inline Net*    Node::getNet () const { return term_->getNet(); }

  Node::~Node ()
  {
    if (getNet()) getNet()->remove( this );
  }

  void Node::toXml ( ostream& os )
  {
    os << "<node term=\"" << term_->getName();
    if (term_->isInternal()){
      if(term_->getInstance()!=NULL){
        os << "\" instance=\"" << term_->getInstance()->getName();
      }
    }
    os << "\" id=\"" << id_ << "\" x=\"" << position_.getX() 
       << "\" y=\"" << position_.getY() << "\"/>"<< endl;
  }


}  // Netlist namespace.

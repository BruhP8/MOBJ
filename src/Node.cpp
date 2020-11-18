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

  bool Node::fromXml (Net * net, xmlTextReaderPtr reader){

    const xmlChar* nodeTag = xmlTextReaderConstString(reader, (const xmlChar*)"node");

    bool retval = false;

    int status = xmlTextReaderRead(reader);
    if (status != 1) {
        if (status != 0) {
            cerr << "[ERROR] Cell::fromXml(): Unexpected termination of the XML parser." << endl;
        }
        return retval;
    }
    
    const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

    if(nodeName == nodeTag){
        string term = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"term"));
        string instance = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"instance"));
        string id = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"id"));
        cerr << "found node connected to term " << term << " of instance " << instance << " and id " << id << endl;
        retval = true;
    }
    return retval; // TODO
  }

}  // Netlist namespace.

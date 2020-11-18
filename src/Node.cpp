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
    bool retval = true;
    Term * toFind;
    Node * newNode = NULL;

    switch ( xmlTextReaderNodeType(reader) ) {
        case XML_READER_TYPE_COMMENT:
        case XML_READER_TYPE_WHITESPACE:
        case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
            break;
    }
    
    string term = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"term"));
    string id = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"id"));
    string instance = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"instance"));

    if(not instance.empty()){
        // Instance provided, find the term using the instance.
        Instance * owner;
        owner = net->getCell()->getInstance(instance);
        toFind = owner->getTerm(term);
        if(toFind == NULL){
            retval = false;
            cerr << "[ERROR] Node::fromXml : Could not find term named " << term <<" in instance "<<instance << endl;
        }
    } else{
        // No instance provided, find the Term using the net's owner cell
        Cell * owner;
        owner = net->getCell();
        toFind = owner->getTerm(term);
        if(toFind == NULL){
            retval = false;
            cerr << "[ERROR] Node::fromXml : Could not find term named " << term <<" in Cell "<<owner->getName() << endl;
        } 
    }
    newNode = new Node(toFind, atoi(id.c_str()));
    net->add(newNode);
    return retval;
  }

}  // Netlist namespace.

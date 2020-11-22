// -*- explicit-buffer-name: "Node.cpp<M1-MOBJ/4-5>" -*-

#include  <limits>
#include  <libxml/xmlreader.h>
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
    os << "\" id=\"" << id_ << "\"/>" << endl;
    //"\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>"<< endl;
  }

  bool Node::fromXml (Net * net, xmlTextReaderPtr reader){
    bool retval = true;
    Term * toFind;
    const xmlChar* nodeTag = xmlTextReaderConstString( reader, (const xmlChar*)"node" );


    const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );
    if (nodeName != nodeTag){
      cerr << "We're not in a <node> balise but in <" << nodeName << "> !" << endl;
      return false;
    }

    switch ( xmlTextReaderNodeType(reader) ) {
        case XML_READER_TYPE_COMMENT:
        case XML_READER_TYPE_WHITESPACE:
        case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
            break;
    }
    
    string termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"term"));
    cout << "Node::fromXml() termName = " << termName << endl;
    
    string id = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"id"));
    cout << "Node::fromXml() nodeId = " << id << endl;
    
    string instance = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"instance"));

    if(not instance.empty()){
        // Instance provided, find the termName using the instance.
        Instance * owner;
        owner = net->getCell()->getInstance(instance);
        
        toFind = owner->getTerm(termName);
        
        if(toFind == NULL){
          retval = false;
          cerr << "[ERROR] Node::fromXml : Could not find term named " << termName <<" in instance "<< instance << endl;
          return false;
        } else {
          Term* term = new Term(owner, toFind);
          Node* node = term->getNode();
          node -> setId(atoi(id.c_str()));
          node -> setPosition(term->getPosition());
          net  -> add(node);
          return true;
        }

    } else{
        // No instance provided, find the Term using the net's owner cell
        Cell * owner;
        owner = net->getCell();
        toFind = owner->getTerm(termName);
        if(toFind == NULL){
            retval = false;
            cerr << "[ERROR] Node::fromXml : Could not find term named " << termName <<" in Cell "<<owner->getName() << endl;
            return false;
        } 
    }
    //newNode = new Node(toFind, atoi(id.c_str()));
    
    Node* node = toFind->getNode();
    node -> setId(atoi(id.c_str()));
    node -> setPosition(toFind->getPosition());
    net  -> add(node);
    
    return retval;
  }

}  // Netlist namespace.

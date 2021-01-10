/* Projet MOBJ                                  *
 * Auteurs : Clément Dupuy et Nicolas Legoueix  *
 * TME 4, 5 et 6                                */

#include "Net.h"
#include "Node.h"
#include "Instance.h"
#include "Line.h"
#include "XmlUtil.h"

namespace Netlist {
  
  using namespace std;
  
  /*------------------------------------------------------------------*
   * Constructeur de Net :                                            *
   * - Initialisation les attributs owner_, name_, id_, type_         *
   * - Ajout du net dans la liste de Net de sa cellule owner_         *
   * - Récupération des Node contenu dans les Term de la Cell owner_  * 
   *                                                                  *
   * Deux cas de construction d'un Net :                              *
   * 1 - On le crée en lui passant une cellule                        *
   * 2 - On le crée de manière indépendante (dans le main par ex)     *
   *     et là c'est la cellule qui doit gérer (connection avec       *
   *     un Term en l'occurence)                                      *   
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  Net::Net   ( Cell* cellule , const string& name , Term::Type type )
  : owner_(cellule)
  , name_(name)
  , id_(cellule->newNetId())
  , type_(type)
  , nodes_()
  , lines_()
  {
    cellule->add(this);
  }
  
  /*------------------------------------------------------------------*
   * à faire lors de la suppression :                                 *
   * - retirer le Net dans la Cell (appel à owner->remove(this))      *
   * - déconnecter les Node reliés par ce net                         *
   * STATUS   --   TODO                                               *
   *------------------------------------------------------------------*/
  Net::~Net  ()
  {
    owner_->remove(this);
    owner_ = NULL;
    for(vector<Node*>::iterator itnode = nodes_.begin(); 
        itnode != nodes_.end(); ++itnode){
          nodes_.erase(itnode);
    }
  }

  Node*   Net::getNode  ( size_t id ){
    //Node* nd = nodes_[id];
    Node *nd = NULL;

    for(size_t i = 0; i < nodes_.size(); i++){
      if (nodes_[i]->getId() == id){
        nd = nodes_[i];
      }
    }
    if (nd == NULL){
      cerr << "[ERROR] Net::getNode : Node not found" << endl;
    }


    return nd;
  }
  
  /*------------------------------------------------------------------*
   * Pour connaître le premier id de Node libre:                      *
   * - Parcours du vecteur de Node pour voir s'il y a une case libre  *
   * - si une case est libre, on retourne le numéro de la case        *
   * - sinon on renvoie la taille du vecteur                          *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  size_t  Net::getFreeNodeId  ()  const
  {
    if (nodes_.empty()){
      return 0;
    }
    for (size_t id = 0; id < nodes_.size(); id++) {
      if (nodes_[id] == NULL) {
        return id;
      }
    } return nodes_.size();
  }
  
  /*------------------------------------------------------------------*
   * Pour ajouter un Node dans la liste :                             *
   * - on récupère le premier id libre dans le tableau                *
   * - nd prend cet identifiant et est ajouté à la case correspondante*
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  void  Net::add  ( Node* nd )
  {
    //cout << "-- SIZE OF Net::nodes_ : " << nodes_.size() << endl;
    nodes_.push_back(nd);
    
  }

  void  Net::add  ( Line* line )
  {
    //cout << "-- INSERTION of Line" << endl;
    if ( line ) {
      
      lines_.push_back( line );
    }
  }
  
  /*------------------------------------------------------------------*
   * Pour retirer un Node dans le vecteur :                           *
   * On fait appel à la fonction erase de la class vector             *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  bool  Net::remove ( Node* nd )
  {
    for( vector<Node*>::iterator inode = nodes_.begin(); 
         inode != nodes_.end(); ++inode ){
      if (*inode == nd) {
        nodes_.erase( inode );
        return true;
      }
    } 
    return false;
  }

  bool  Net::remove ( Line* ln )
  {
    if (ln) {
      for ( vector<Line*>::iterator il = lines_.begin();
            il != lines_.end(); ++il ){
        if ( *il == ln ) {
          lines_.erase( il );
          return true;
        }
      }
    }
    return false;
  }

  /*------------------------------------------------------------------*
   * Converti la Net au format Xml :                                  *
   * On fait appel à la fonction toXml de Node pour afficher les      *
   * nodes contenu dans le vecteur                                    *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  void  Net::toXml ( ostream& os ){
    string type = "";
    if(type_ == Term::Internal){
      type = "Internal";
    } else {
      type = "External";
    }

    os << indent << "<net name=\"" << name_ << "\" type=\"" << type << "\"/>" << endl;

    indent++;    
    for ( Node* inode : nodes_ ){
      if (inode != NULL){
        //os << ind;
        inode->toXml(os);
      }
    }
    for ( Line* line : lines_ ){
      if (line != NULL){
        //os << ind;
        line->toXml(os);
      }
    }
    indent--;
    os << indent << "</net>" << endl;
  }


  Net* Net::fromXml ( Cell* cell, xmlTextReaderPtr reader )
  {
    //cout << "Beginning of Net::fromXml()" << endl;
    
    const xmlChar* netTag  = xmlTextReaderConstString( reader, (const xmlChar*)"net"  );
    const xmlChar* nodeTag = xmlTextReaderConstString( reader, (const xmlChar*)"node" );
    const xmlChar* lineTag = xmlTextReaderConstString( reader, (const xmlChar*)"line" ); 
    Net* net       = NULL;

    switch ( xmlTextReaderNodeType(reader) ) {
        case XML_READER_TYPE_COMMENT:
        case XML_READER_TYPE_WHITESPACE:
        case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
            break;
    }

    const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );
    
    if (nodeName != netTag){
      return net;
    }

    string netName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ));
    string typeStr = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ));
    if (netName.empty() or typeStr.empty()) return net;
    else
    {
      Term::Type type;
      if (typeStr == "Internal"){
        type = Term::Type::Internal;
      } else if (typeStr == "External"){
        type = Term::Type::External;
      } else {
        cerr << "[ERROR] Net::fromXml(): incorrect type" << endl;
        return NULL;
      }
      net = new Net(cell, netName, type);

      while( true ){
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
          case  XML_READER_TYPE_END_ELEMENT:
                return net;
        }

        const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);

        if ( nodeName == nodeTag ){
          bool test = Node::fromXml(net, reader);
          if (! test){
            cout << "[Error] Node::fromXml()" << endl;
            return NULL;
          } else {
            continue;
          }
        }

        if ( nodeName == lineTag ){
          bool test = Line::fromXml(net, reader);
          if (! test ){
            cout << "[Error] Line::fromXml()" << endl;
            return NULL;
          } else {
            continue;
          }
        }

        break;

      }
    }    
    return net;

  }


}
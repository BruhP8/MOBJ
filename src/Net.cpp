/* Projet MOBJ                                  *
 * Auteurs : Clément Dupuy et Nicolas Legoueix  *
 * TME 4, 5 et 6                                */

#include "Net.h"
#include "Node.h"
#include "Instance.h"

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
    //size_t nd_id = getFreeNodeId();
    size_t nd_id = nd->getId();
    if( nodes_.empty() ){
      nd -> setId(0);
      nodes_.push_back(nd);
    }
    /*if( nd_id < nodes_.size() ){
      nd -> setId ( nd_id ) ;
      nodes_[nd_id] = nd;
    }*/
    else {
      /*nd -> setId(nodes_.size());
      nodes_.push_back(nd);*/
      for( vector<Node*>::iterator inode = nodes_.begin();
           inode != nodes_.end(); ++inode ){
        size_t iid = (*inode)->getId();
        if (nd_id < iid){
          nodes_.insert(inode, nd);
        }
      } nodes_.push_back(nd);
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

  /*------------------------------------------------------------------*
   * Converti la Net au format Xml :                                  *
   * On fait appel à la fonction toXml de Node pour afficher les      *
   * nodes contenu dans le vecteur                                    *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  void  Net::toXml ( ostream& os ){
    Indentation ind;
    string type = "";
    if(type_ == Term::Internal){
      type = "Internal";
    } else {
      type = "External";
    }

    os << "<net name=\"" << name_ << "\" type=\"" << type << "\"/>" << endl;
    ++ind;
    ++ind;
    ++ind;
    for ( Node* inode : nodes_ ){
      if (inode != NULL){
        os << ind;
        inode->toXml(os);
      }
    }
    --ind;
    os << ind << "</net>" << endl;
  }


  Net* Net::fromXml ( Cell* cell, xmlTextReaderPtr reader )
  {
    enum NetState { Init      = 0
                  , BeginNet
                  , Loop
                  , EndNet
                  };

    const xmlChar* netTag = xmlTextReaderConstString( reader, (const xmlChar*)"net" );

    Net* net       = NULL;
    NetState state = Init;

    while(true){

      int status = xmlTextReaderRead(reader);
      if (status != 1) {
        if (status != 0) {
          cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser." << endl;
        }
        break;
      }

      switch ( xmlTextReaderNodeType(reader) ){
        case  XML_READER_TYPE_COMMENT:
        case  XML_READER_TYPE_WHITESPACE:
        case  XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
          continue;
      }

      const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

      /*A déboguer*/
      switch ( state ) {
        case Init:
          if ( (nodeName == netTag) ) {
            //cout << "Chouette !" << endl;
            string netName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ));
            if (netName.empty()){
              cerr << "[ERROR] Net::fromXml(): empty name" << endl;
              return net;
            }
            string typeStr = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ));
            if (typeStr.empty()){
              cerr << "[ERROR] Net::fromXml(): empty type attribute" << endl;
              return net;
            }
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
            state = Loop;
            //break;
          }
          break;
        case Loop :
          if ( (nodeName == netTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) ){
            //cout << "Chouette !" << endl;
            string netName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ));
            if (netName.empty()){
              cerr << "[ERROR] Net::fromXml(): empty name" << endl;
              return net;
            }
            string typeStr = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ));
            if (typeStr.empty()){
              cerr << "[ERROR] Net::fromXml(): empty type attribute" << endl;
              return net;
            }
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
            //state = EndNet;
            continue;
          } else {
            bool test = Node::fromXml(net, reader);
            if (test) continue;
          }
          //state = EndNet;
          break;
        case EndNet:
          if ( (nodeName == netTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) ){
            return net;
          } else {
            bool test = Node::fromXml(net, reader);
            if (test) continue;
            else break;
            
          }
          break;
      /*----------*/
        default: 
          break;
        
      }
      //cerr << "[ERROR] Net::fromXml(): Unknow or misplaced tag <" << nodeName
      //     << "> (line" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;

      

    }
    return net;

  }


}
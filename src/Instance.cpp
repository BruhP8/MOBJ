/* Projet MOBJ                                  *
 * Auteurs : Clément Dupuy et Nicolas Legoueix  *
 * TME 4, 5 et 6                                */

#include "Instance.h"
#include "Net.h"
#include "XmlUtil.h"

namespace Netlist {

  using namespace std;
  
  /*------------------------------------------------------------------*
   * Constructeur d'Instance :                                        *  
   * - Connecte l'Instance à son owner, et son model                  *
   * - Initialise le nom de l'instance                                *
   * - Effectue une copie des pointeurs de Term de la cellule modèle  *
   * STATUS  --   DONE                                                *
   *------------------------------------------------------------------*/
  Instance::Instance  ( Cell * owner, Cell * model, const string& name )
  :owner_(owner), masterCell_(model), name_(name), terms_()
  {
    if (model != NULL){
      for( Term* iterm : model -> getTerms() ){
        terms_.push_back(new Term(this, iterm));
      }
    }
    owner->add(this);
  }
  
  /*------------------------------------------------------------------*
   * Destructeur d'Instance :                                         *
   * - Déconnection de l'Instance de son owner_ et de sa masterCell   *
   * - Suppression des pointeurs de Term ainsi que du tableau         *
   * - Suppression du Point correspondant à la position de l'Instance *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  Instance::~Instance ()
  {
    owner_      -> remove(this);
    masterCell_ -> remove(this);
    owner_      =  NULL;
    masterCell_ =  NULL;
    for( size_t it = 0; it < terms_.size(); ++it ){
      delete terms_[it];
    }
  }
  
  /*------------------------------------------------------------------*
   * Getter sur un Term de l'Instance                                 *
   * - Parcours la liste des pointeurs de Term                        *
   * - Teste pour chaque Term si le nom correspond                    *
   * - Retourne le premier Term pour lequel le nom correspond         *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  Term* Instance::getTerm ( const string& name )  const
  {
    for (size_t it = 0; it < terms_.size(); ++it)
    {
      if (terms_[it]->getName() == name)
      {
        return terms_[it];
      }
    }
    return NULL;
  }
  
  /*------------------------------------------------------------------*
   * Connecteur de Net                                                *
   * - Chercher le Term ayant pour nom, name                          *
   * - Ajouter le node contenu dans Term à la liste des Node du Net   *
   * - Renvoie true si le term a été trouvé, false sinon              *
   * STATUS   --   TODO                                               *
   *------------------------------------------------------------------*/
  bool  Instance::connect ( const string& name, Net * net )
  {
    Term* term = getTerm( name );
    if (term == NULL) return false;

    term->setNet( net );
    return true;
  }
  
  /*------------------------------------------------------------------*
   * Ajout d'un Term                                                  *
   * - Ajoute le Term passé en argument                               *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  void  Instance::add ( Term* term )
  {
    if (term != NULL){
      terms_.push_back(term); //ça ne bouge plus non plus !
    }
  }
  
  /*------------------------------------------------------------------*
   * Suppression d'un Term                                            *
   * - Parcours la liste des Term                                     *
   * - Si le term courant est identique à celui passé en argument     *
   *   alors on l'efface de la liste                                  *
   * STATUS   --  DONE                                                *
   *------------------------------------------------------------------*/
  void  Instance::remove  ( Term* term )
  {
    for( vector<Term*>::iterator iterm = terms_.begin(); iterm != terms_.end(); ++iterm) {
      if (*iterm == term) terms_.erase( iterm );
    }
  }


  void Instance::setPosition ( const Point& p )
  {
    position_.setX(p.getX());
    position_.setY(p.getY());

    /* Pour chaque Term contenu dans le vecteur :
     * 1- on commence par récupérer la position du TermShape dans le symbol
     * 2- on translate cette position de la position de l'instance
     * 3- et on déplace le Term */
    for(Term* t : terms_){
      Point pos = owner_->getSymbol()->getTermPosition(t);
      pos.translate(position_);
      t->setPosition(pos);
    }
    
  }

  /* Idem avec x et y */
  void Instance::setPosition ( int x, int y )
  {
    position_.setX(x);
    position_.setY(y);
  
    for(Term* t : terms_){
      Point pos = owner_->getSymbol()->getTermPosition(t);
      pos.translate(x, y);
      t->setPosition(pos);
    }
  }

  /*------------------------------------------------------------------*
   * Affichage d'un Term                                              *
   * - Affiche le nom de l'instance à laquelle dont il dépend         *
   * - Affiche le nom de sa mastercell                                *
   * - Affiche ses coordonnées                                        *
   * STATUS   --  DONE                                                *
   *------------------------------------------------------------------*/
  void  Instance::toXml  ( ostream& os )
  {
    os << indent << "<instance name=\"" << name_ << "\" mastercell=\"" 
       << masterCell_->getName() << "\" x=\"" << position_.getX() 
       << "\" y=\"" << position_.getY() << "\"/>" << endl;
  }

  /*------------------------------------------------------------------*
   * Création d'une Instance à partir d'un fichier Xml                *
   * - Lit chaque attribut dans le fichier                            *
   * - Si les attributs ont été lus correctement                      *
   * - Alors on peut créer l'Instance                                 *
   * STATUS   --  DONE                                                *
   *------------------------------------------------------------------*/
  Instance* Instance::fromXml ( Cell* cell, xmlTextReaderPtr reader ){
    
    cout << "Beginning of Instance::fromXml()" << endl;

    Instance* instance = NULL;
    
    switch( xmlTextReaderNodeType(reader) ){
      case  XML_READER_TYPE_COMMENT :  
      case  XML_READER_TYPE_WHITESPACE :
      case  XML_READER_TYPE_SIGNIFICANT_WHITESPACE :
            break; 
    }

    string instName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
    cout << "Instance name : " << instName << endl;
    if( instName.empty() ){
      cerr << "[ERROR] Instance::fromXml(): name is empty" << endl;
      return instance;
    }

    string cellName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"mastercell" ) );
    cout << "MasterCell : " << cellName << endl;
    Cell* masterCell = Cell::find(cellName);
    if (masterCell == NULL){
      cerr << "[ERROR] Instance::fromXml(): no masterCell " << cellName << " found" << endl;
      cout << "[SOLUTION] Instance::fromXml(): try to load " << cellName << endl;
      masterCell = Cell::load( cellName );
      if (masterCell == NULL){
        cerr << "[ERROR] Instance::fromXml(): unfortunately, unable to load " << cellName << endl;
      }
    }


    string x_value = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );
    string y_value = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );
    int x = atoi(x_value.c_str());
    int y = atoi(y_value.c_str());

    instance = new Instance( cell, masterCell, instName );
    instance->setPosition(x, y);

    return instance;
  }

}

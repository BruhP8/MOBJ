#include "Instance.h"
#include "Net.h"

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
  Instance::~Instance  ()
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


  void  Instance::toXml  ( ostream& os )
  {
    os << "<instance name=\"" << name_ << "\" mastercell=\"" 
       << masterCell_->getName() << "\" x=\"" << position_.getX() 
       << "\" y=\"" << position_.getY() << "\" />" << endl;
  }


}
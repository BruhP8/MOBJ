#include "Term.h"
#include "Net.h"

namespace Netlist {
  
  using namespace std;
  
  /*------------------------------------------------------------------*
   * Constructeur de Term appartenant à une Cell:                     *
   * - Instancie : owner_ avec la cellule passée en argument          *
   *               name_ avec le nom passé en argument                *
   *               type_ est External car le Term appartient à Cell   *
   *               le node_ est créé à partir du Term en creation     *
   * - Le net_ est laissé vide, il sera connecté à postériori par la  *
   *   Cell owner_                                                    *
   * - Connecte le Term à la Cell owner_                              *
   * STATUS   --   TODO                                               *
   *------------------------------------------------------------------*/
  Term::Term ( Cell * cellule , const string & name, Direction dir )
  : owner_(cellule)
  , name_(name)
  , direction_(dir)
  , type_(External)
  , node_(this)
  {
    net_ = cellule->getNet(name);
    cellule->add(this);
  }
  
  /*------------------------------------------------------------------*
   * Constructeur de Term :                                           *
   * - Instancie : owner_ avec l'instance passée en argument          *
   *               name_ avec le nom du modelTerm                     *
   *               type_ est Internal car le Term appartient à Inst   *
   *               le node_ est créé à partir du Term en création     *
   * - Le net est laissé vide, il sera ajouté à postériori depuis     *
   *   l'extérieur (ici le main par exemple)                          *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  Term::Term ( Instance * inst , const Term * modelTerm )
  : owner_(inst)
  , name_(modelTerm->getName())
  , direction_(modelTerm->direction_)
  , type_(Internal)
  , node_(this)
  {
    inst->add(this); // ça ne bouge plus !!!! (j'ai eu une boucle infinie à ce niveau là lol)
  }
  
  /*------------------------------------------------------------------*
   * Destructeur de Term :                                            *
   * - Supprimer le Node de la liste et déconnecter le Term du Net    *
   * - Déconnecter le Term de la Cell/Instance owner_                 *
   * STATUS   --   TODO                                               *
   *------------------------------------------------------------------*/
  Term::~Term ()
  {
    if (type_ == External){
      getCell()->remove(this);  
    }
    else{
      getInstance()->remove(this);
    }
    owner_ = NULL; //Suppression du pointeur sur le propriétaire
    
    net_->remove(getNode());
    
    net_ = NULL;
    
  }

  /*------------------------------------------------------------------*
   * Getteur sur la Cell owner_ :                                     *
   * - si l'owner_ est une Instance, on renvoit sa Cell owner_        *
   * - si l'owner_ est une Cell alors c'est elle qu'on renvoie        *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  Cell* Term::getOwnerCell ()  const
  {
    if (type_ == Internal) {
      return getInstance() -> getCell();
    } else {
      return static_cast<Cell *>(owner_);
    }
  }
  
  /* Modificateurs : */

  /*------------------------------------------------------------------*
   * Setteur de Net :                                                 *
   * - ajoute le node_ dans le net passé en argument                  *
   * - affecte le champ net_ du Term                                  *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  void  Term::setNet  ( Net * net )
  {
    net->add( & node_ );
    net_ = net;
  }

  /*------------------------------------------------------------------*
   * Setter de Net :                                                  *
   * - ajoute le node_ dans le net contenu dans l'ownercell et dont   *
   *   le nom est passé en argument                                   *
   * STATUS   --   DONE                                               *
   *------------------------------------------------------------------*/
  void  Term::setNet  ( const string& name )
  {
    getOwnerCell()->getNet(name)->add( &node_ );
    net_ = getOwnerCell()->getNet(name);
  }

  /* Affiche le Term au format Xml */
  void  Term::toXml  ( ostream& os )
  {
    string dir="";
    switch(direction_){
      case 1 :
      {
        dir = "In";
        break;
      }
      case 2 :
      {
        dir = "Out";
        break;
      } 
      case 3:
      {
        dir = "Inout";
        break;
      }
      case 4:
      {
        dir = "Tristate";
        break;
      }  
      case 5:
      {
        dir = "Transcv";
        break;
      }
      default:
      {
        dir = "Unknown";
      }
    }
    os << "<term name=\"" << name_ << "\" direction=\"" << dir << "\"/>" << endl;
  }
  
  Term*  fromXml ( Cell* cell, xmlTextReaderPtr reader )
  {
    return new Term( cell, "pilou", Term::Tristate );
  }


}
// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/4-5>" -*-

#include <libxml/xmlreader.h>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
using namespace std;

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
using namespace Netlist;


int main ( int argc, char* argv[] )
{
  cout << "\nConstruction du modele <and2>." << endl;
  Cell* and2 = new Cell ( "and2" );
  new Term( and2, "i0", Term::In  );
  new Term( and2, "i1", Term::In  );
  new Term( and2,  "q", Term::Out );
  //and2->toXml( cout );

  cout << "\nConstruction du modele <or2>." << endl;
  Cell* or2 = new Cell ( "or2" );
  new Term( or2, "i0", Term::In  );
  new Term( or2, "i1", Term::In  );
  new Term( or2,  "q", Term::Out );
  //or2->toXml( cout );

  cout << "\nConstruction du modele <xor2>." << endl;
  Cell* xor2 = new Cell ( "xor2" );
  new Term( xor2, "i0", Term::In  );
  new Term( xor2, "i1", Term::In  );
  new Term( xor2,  "q", Term::Out );
  //xor2->toXml( cout );

  cout << "\nConstruction du modele <halfadder>." << endl;
  Cell* halfadder = new Cell ( "halfadder" );
  new Term( halfadder, "a"   , Term::In  );
  new Term( halfadder, "b"   , Term::In  );
  new Term( halfadder, "sout", Term::Out );
  
  new Term( halfadder, "cout", Term::Out );
  Net*      ha_a    = new Net      ( halfadder, "a"   , Term::External );
  
  Net*      ha_b    = new Net      ( halfadder, "b"   , Term::External );
  Net*      ha_sout = new Net      ( halfadder, "sout", Term::External );
  Net*      ha_cout = new Net      ( halfadder, "cout", Term::External );
  Instance* ha_xor2 = new Instance ( halfadder, Cell::find("xor2"), "xor2_1" );
  Instance* ha_and2 = new Instance ( halfadder, Cell::find("and2"), "and2_1" );
  
  halfadder->connect( "a"   , ha_a    );
  halfadder->connect( "b"   , ha_b    );
  halfadder->connect( "sout", ha_sout );
  halfadder->connect( "cout", ha_cout );
  ha_xor2->connect( "i0", ha_a    );
  ha_xor2->connect( "i1", ha_b    );
  ha_xor2->connect(  "q", ha_sout );
  ha_and2->connect( "i0", ha_a    );
  ha_and2->connect( "i1", ha_b    );
  ha_and2->connect(  "q", ha_cout );
  //halfadder->toXml( cout );

  cout << "\nConstruction du modele <fulladder>.\n" << endl;
  
  Cell * fulladder = new Cell ( "fulladder" );
  
  /*Création des terminaux*/
  new Term( fulladder, "fa_a"   , Term::In );  //Entrée a du FA
  new Term( fulladder, "fa_b"   , Term::In );  //Entrée b du FA
  new Term( fulladder, "fa_cin" , Term::In );  //Entrée cin du FA
  new Term( fulladder, "fa_sout", Term::Out);  //Sortie sout du FA
  new Term( fulladder, "fa_cout", Term::Out);  //Sortie cout du FA

  Net*      fa_a        = new Net      ( fulladder, "fa_a"    , Term::External );  //Fil qui part de a
  Net*      fa_b        = new Net      ( fulladder, "fa_b"    , Term::External );  //Fil qui part de b  
  Net*      fa_cin      = new Net      ( fulladder, "fa_cin"  , Term::External );  //Fil qui part de cin
  Net*      fa_sout     = new Net      ( fulladder, "fa_sout" , Term::External );  //Fil qui va vers sout
  Net*      fa_cout     = new Net      ( fulladder, "fa_cout" , Term::External );  //Fil qui va vers cout
  Net*      fa_sout_1   = new Net      ( fulladder, "fa_sout1", Term::Internal );
  Net*      fa_carry_1  = new Net      ( fulladder, "carry_1" , Term::Internal );
  Net*      fa_carry_2  = new Net      ( fulladder, "carry_2" , Term::Internal );
  
  /*On instancie les 2 halfadders, ainsi que la porte "or"*/
  Instance* fa_halfadd1 = new Instance ( fulladder, Cell::find("halfadder"), "halfadder_1" );
  Instance* fa_halfadd2 = new Instance ( fulladder, Cell::find("halfadder"), "halfadder_2" );
  Instance* fa_or2      = new Instance ( fulladder, Cell::find("or2"), "or2_1" );
  
  /*Connection du fil sur le terminal*/
  fulladder->connect( "fa_a"    , fa_a    );
  fulladder->connect( "fa_b"    , fa_a    );
  fulladder->connect( "fa_cin"  , fa_cin  );
  fulladder->connect( "fa_cout" , fa_cout );
  fulladder->connect( "fa_sout" , fa_sout );
  
  /*Connection des ports des instances les unes aux autres         *
   *Connection des entrées du modèle sur les entrées des instances *
   *Connection des sorties des instances sur les sorties du modèle */
  fa_halfadd1->connect( "a"   , fa_a      );  //connecte le point a du halfadder sur le port fa_a de mon fulladder
  fa_halfadd1->connect( "b"   , fa_b      );
  fa_halfadd1->connect( "sout", fa_sout_1 );
  fa_halfadd1->connect( "cout", fa_carry_1);

  fa_halfadd2->connect( "a"   , fa_cin    );
  fa_halfadd2->connect( "b"   , fa_sout_1 );
  fa_halfadd2->connect( "sout", fa_sout   );
  fa_halfadd2->connect( "cout", fa_carry_2);

  fa_or2->connect( "i0", fa_carry_2 );
  fa_or2->connect( "i1", fa_carry_1 );
  fa_or2->connect( "q" , fa_cout    );

  fulladder->toXml( cout );
  

  

  return 0;
}

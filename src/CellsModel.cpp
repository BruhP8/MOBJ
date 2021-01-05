#include <QAction>
#include <iostream>
#include "CellsModel.h"
#include "CellViewer.h"

namespace Netlist {

  /*------------------------------------------------------------------*
   * Constructeur de CellModel                                        *
   * - connect le signal cellLoaded émis par CellViewer               *
   *   au SLOT updateDatas()                                          *
   * STATUS   --  TODO                                                *
   *------------------------------------------------------------------*/
  CellsModel::CellsModel( QObject* parent )
  : QAbstractTableModel(parent)
  {
    QAction* action = new QAction( "&Refresh Model", this );
    connect( action, SIGNAL(CellViewer::cellLoaded()), this, SLOT(updateDatas()) );

  }

  CellsModel::~CellsModel()
  {}

  /*
   * Fait appelle au signal layoutChanged (existant dans QAbstractTableModel)
   * Permet de faire un update directement
   * */
  void CellsModel::updateDatas()
  {


    std::cout << "[SLOT] CellsModel::updateDatas() : signal received" << std::endl;
    
    
    layoutChanged();
  }


}
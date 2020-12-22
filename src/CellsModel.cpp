#include <QAction>
#include <iostream>
#include "CellsModel.h"

namespace Netlist {

  CellsModel::CellsModel( QObject* parent )
  : QAbstractTableModel(parent)
  {}

  CellsModel::~CellsModel()
  {}

  //A VERIFIER
  void CellsModel::updateDatas()
  {

    std::cout << "[SLOT] CellsModel::updateDatas() : signal received" << std::endl;
    QAction* action = new QAction( "&Update Data", this);
    connect( action, SIGNAL(layoutChanged()), this, NULL);
  }

}
#include <QAction>

#include "CellsModel.h"

namespace Netlist {

  CellsModel::CellsModel( QObject* parent )
  : QAbstractTableModel(parent)
  {}

  CellsModel::~CellsModel()
  {}

  //A VERIFIER
  void CellsModel::updateData()
  {
    QAction* action = new QAction( "&Update Data", this);
    connect( action, SIGNAL(layoutChanged()), this, NULL);
  }

}
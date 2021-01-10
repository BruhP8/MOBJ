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
    QModelIndex model = QModelIndex();
    beginInsertRows(model, 0, Cell::getAllCells().size());



    QAction* action = new QAction( "&Refresh Model", this );
    connect( action, SIGNAL(CellViewer::cellLoaded()), this, SLOT(updateDatas()) );

  }

  CellsModel::~CellsModel()
  {}


  QVariant CellsModel::data( const QModelIndex& index, int role ) const {
    if (not index.isValid()) {
      std::cerr << "[ERROR] CellsModel : index is not valid" << std::endl;
      return QVariant();
    } 
    if (role == Qt::DisplayRole) {
      if (index.column() == 0){
        int row = index.row();
        std::cout << "[CELLSMODEL] : " << Cell::getAllCells()[row]->getName() << std::endl;
        return Cell::getAllCells()[row]->getName().c_str();
      }
    }
    return QVariant();
  }

  QVariant CellsModel::headerData ( int section
                                  , Qt::Orientation orientation
                                  , int role ) const
  {
    if (orientation == Qt::Vertical) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    switch ( section ){
      case 0 : return "Cell";
    }
    return QVariant();
  }


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
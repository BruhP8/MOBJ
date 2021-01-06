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

  QVariant CellsModel::data( const QModelIndex& index, int role ) const {
    if (not index.isValid()) {
      std::cerr << "[ERROR] CellsModel : index is not valid" << std::endl;
      return QVariant();
    } 
    if (role == Qt::DisplayRole) {
      int row = index.row();
      //switch ( index.column() ) {
      //  case 0: return cell_->getInstances()[row]->getName().c_str();
      //  case 1: return cell_->getInstances()[row]
      //                      ->getMasterCell()->getName().c_str();
      //}
      return Cell::getAllCells()[row]->getName().c_str();
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
    
    if ( Cell::getAllCells().empty() ) {
      std::cout << "[CELLSMODEL] : No Cells" << std::endl;
    }
    //  //int row = rowCount(QModelIndex());
    //  indexRow(row);
    //  setData(createIndex(row, 0), QVariant::fromValue<std::string>("No data"), Qt::EditRole);
    //}

    //std::vector<Cell*> list = Cell::getAllCells();
    int row = 0;
    for( Cell* cell : Cell::getAllCells() ){
      insertRow(row);
      setData(createIndex(row, 0), data(QModelIndex(), row), Qt::EditRole);
      row++;
    }

    layoutChanged();
  }

}
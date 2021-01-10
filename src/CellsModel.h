#ifndef NETLIST_CELLSMODEL_H
#define NETLIST_CELLSMODEL_H

#include <QAbstractTableModel>
#include "Cell.h"

namespace Netlist {

  class CellsModel : public QAbstractTableModel {
    Q_OBJECT;

    public :
      CellsModel( QObject* parent=NULL );
     ~CellsModel();

      inline  int       columnCount ( const QModelIndex& parent = QModelIndex() ) const;
      inline  int       rowCount    ( const QModelIndex& parent = QModelIndex() ) const;
              QVariant  data        ( const QModelIndex& index, int role ) const;
              QVariant  headerData  ( int section, Qt::Orientation orientation,
                                    int role=Qt::DisplayRole ) const;


    public slots :
      void  updateDatas();

  };

  inline int CellsModel::rowCount( const QModelIndex& parent ) const {
    return Cell::getAllCells().size();
  }

  inline int CellsModel::columnCount( const QModelIndex& parent ) const {
    return 1;
  }


}

#endif
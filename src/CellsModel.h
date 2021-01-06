#ifndef NETLIST_CELLSMODEL_H
#define NETLIST_CELLSMODEL_H

#include <QAbstractTableModel>

namespace Netlist {

  class CellsModel : public QAbstractTableModel {
    Q_OBJECT;

    public :
      CellsModel( QObject* parent=NULL );
     ~CellsModel();

      QVariant data( const QModelIndex& index, int role ) const;

    public slots :
      void  updateDatas();

  };

}

#endif
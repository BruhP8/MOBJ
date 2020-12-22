#include <QHeaderView>
#include <iostream>
#include "InstancesWidget.h"
#include "InstancesModel.h"
#include "CellViewer.h"

namespace Netlist {

  InstancesWidget::InstancesWidget ( QWidget* parent )
  : QWidget     ( parent )
  , cellViewer_ ( NULL )
  , baseModel_  ( new InstancesModel(this) )
  , view_       ( new QTableView(this) )
  , load_       ( new QPushButton(this) )
  {
    setAttribute( Qt::WA_QuitOnClose,   false );
    setAttribute( Qt::WA_DeleteOnClose, false );
    setContextMenuPolicy( Qt::ActionsContextMenu );

    view_->setShowGrid              ( false );
    view_->setAlternatingRowColors  ( true );
    view_->setSelectionBehavior     ( QAbstractItemView::SelectRows );
    view_->setSelectionMode         ( QAbstractItemView::SingleSelection );
    view_->setSortingEnabled        ( true );
    view_->setModel                 ( baseModel_ );

    QHeaderView* horizontalHeader = view_->horizontalHeader();
    horizontalHeader->setDefaultAlignment   ( Qt::AlignHCenter );
    horizontalHeader->setMinimumSectionSize ( 300 );
    horizontalHeader->setStretchLastSection ( true );

    QHeaderView* verticalHeader = view_->verticalHeader();
    verticalHeader->setVisible( false );
    load_->setText( "Load" );
    connect( load_, SIGNAL(clicked()), this, SLOT(load()) );
  }

  /*A FAIRE*/
  void InstancesWidget::setCellViewer( CellViewer* cell )
  {
    //std::cout << "InstancesWidget::setCellViewer() : just before setEnabled" << std::endl;
    //setEnabled(false);
    //std::cout << "InstancesWidget::setCellViewer() : just after setEnabled" << std::endl;
    //if (cell == NULL){
    //  std::cerr << "[ERROR] InstancesWidget::setCellViewer: nullptr" << std::endl;
    //}
    //std::cout << "InstancesWidget::setCellViewer() : cellViewer_" << std::endl;
    //cellViewer_ = cell;
    //setEnabled(true);
    //update();
  }

  int InstancesWidget::getSelectedRow () const
  {
    QModelIndexList selecteds = view_->selectionModel()->selection().indexes();

    if( selecteds.empty() ) return -1;
    return selecteds.first().row();
  }

  void InstancesWidget::load ()
  {
    int selectedRow = getSelectedRow();
    if (selectedRow < 0) return;
    cellViewer_->setCell( baseModel_->getModel(selectedRow) );
  }

  void InstancesWidget::setCell( Cell* cell )
  { 
    baseModel_->setCell(cell); 
  }

}
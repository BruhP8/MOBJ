#include <iostream>
#include <QHeaderView>
#include "CellsLib.h"

namespace Netlist {

  CellsLib::CellsLib( QWidget* parent )
  : QWidget(parent)
  , cellViewer_(NULL)
  , baseModel_(NULL)
  {
  //  setAttribute( Qt::WA_QuitOnClose,   false );
  //  setAttribute( Qt::WA_DeleteOnClose, false );
  //  setContextMenuPolicy( Qt::ActionsContextMenu );
//
  //  view_->setShowGrid              ( false );
  //  view_->setAlternatingRowColors  ( true );
  //  view_->setSelectionBehavior     ( QAbstractItemView::SelectRows );
  //  view_->setSelectionMode         ( QAbstractItemView::SingleSelection );
  //  view_->setSortingEnabled        ( true );
  //  view_->setModel                 ( baseModel_ );
//
  //  QHeaderView* horizontalHeader = view_->horizontalHeader();
  //  horizontalHeader->setDefaultAlignment   ( Qt::AlignHCenter );
  //  horizontalHeader->setMinimumSectionSize ( 300 );
  //  horizontalHeader->setStretchLastSection ( true );
//
  //  QHeaderView* verticalHeader = view_->verticalHeader();
  //  verticalHeader->setVisible( false );
  //  load_->setText( "Load" );
  //  connect( load_, SIGNAL(clicked()), this, SLOT(load()) );

    std::cout << "[CELLS_LIB] : Constructor OK" << std::endl;

  }

  CellsLib::~CellsLib()
  {}

  /* Problème de pointeur : 
   * - */
  void CellsLib::setCellViewer( CellViewer* cv ){
    std::cout << "[CELL_LIB] : setCellViewer()" << std::endl;
    
    cellViewer_ = cv;

    std::cout << "[CELL_LIB] : End of setCellViewer()" << std::endl;
  }

  //TODO
  int CellsLib::getSelectedRow() const {
    return 0;
  }

  //TODO
  void CellsLib::load() {

  }

}
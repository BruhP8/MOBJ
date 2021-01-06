#include <iostream>
#include <QHeaderView>
#include "Cell.h"
#include "CellsModel.h"
#include "CellViewer.h"
#include "CellsLib.h"

namespace Netlist {

  CellsLib::CellsLib( QWidget* parent )
  : QWidget( parent )
  , cellViewer_( NULL )
  , baseModel_( NULL )
  , view_( new QTableView(this) )
  , load_( new QPushButton(this) )
  {

    if (view_ == NULL){
      std::cerr << "[CELLS_LIB] : error during creation of QTableView" << std::endl;
      exit(-1);
    }

    //setAttribute( Qt::WA_QuitOnClose,   false );
    //setAttribute( Qt::WA_DeleteOnClose, false );
    //setContextMenuPolicy( Qt::ActionsContextMenu );

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

    std::string wName = "CellsLib";
    setWindowTitle( QString(wName.c_str()) );

    setMinimumHeight(200);
    setMinimumWidth(300);

    QHeaderView* verticalHeader = view_->verticalHeader();
    verticalHeader->setVisible( false );
    load_->setText( "Load" );
    connect( load_, SIGNAL(clicked()), this, SLOT(load()) );
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
    QModelIndexList selecteds = view_->selectionModel()->selection().indexes();

    if ( selecteds.empty() ) return -1;
    return selecteds.first().row();
  }

  //TODO
  void CellsLib::load() {
    int selectedRow = getSelectedRow();
    if (selectedRow < 0) return;
    std::cout << "[CELLSLIB] : loading " << Cell::getAllCells()[selectedRow]->getName() << std::endl;
    cellViewer_->setCell( Cell::getAllCells()[selectedRow] );
  }

}
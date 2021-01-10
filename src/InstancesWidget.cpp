#include <QHeaderView>
#include <QHBoxLayout>
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

    view_->setShowGrid              ( false );   //Affichage de la grille
    view_->setAlternatingRowColors  ( true );   //Alternance de couleurs
    view_->setSelectionBehavior     ( QAbstractItemView::SelectRows );      //Toute la rangée d'un coup
    view_->setSelectionMode         ( QAbstractItemView::SingleSelection ); //Selection d'une seule ligne à la fois
    view_->setSortingEnabled        ( true );   //Tri automatique
    view_->setModel                 ( baseModel_ ); //Association entre le model et la view
    //view_->setColumnHidden          ( 0, false );
    //view_->setColumnHidden          ( 1, false );
    //view_->setColumnWidth           (0, 100);
    //view_->setColumnWidth           (1, 100);

    //Customisation du Header Horizontalement
    QHeaderView* horizontalHeader = view_->horizontalHeader();
    horizontalHeader->setDefaultAlignment   ( Qt::AlignHCenter );
    horizontalHeader->setMinimumSectionSize ( 300 );
    horizontalHeader->setStretchLastSection ( true );

    //Suppression du Header Vertical
    QHeaderView* verticalHeader = view_->verticalHeader();
    verticalHeader->setVisible( false );

    std::string wName = "InstancesWidget";
    setWindowTitle( QString(wName.c_str()) );
    //setMinimumWidth(600);
    //setMinimumHeight(200);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget( view_ );
    vLayout->addWidget( load_ );

    QFrame* separator = new QFrame();
    separator->setFrameShape ( QFrame::VLine );
    separator->setFrameShadow( QFrame::Sunken );

    vLayout->setSizeConstraint( QLayout::SetFixedSize );

    setLayout( vLayout );

    load_->setText( "Load" );
    connect( load_, SIGNAL(clicked()), this, SLOT(load()) );
    std::cout << "[INSTANCESWIDGET] Constructor : OK" << std::endl;

  }

  /*A FAIRE*/
  void InstancesWidget::setCellViewer( CellViewer* cell )
  {
    //std::cout << "InstancesWidget::setCellViewer() : just before setEnabled" << std::endl;
    //setEnabled(false);
    std::cout << "InstancesWidget::setCellViewer() : just after setEnabled" << std::endl;
    if (cell == NULL){
      std::cerr << "[ERROR] InstancesWidget::setCellViewer: nullptr" << std::endl;
    }
    std::cout << "InstancesWidget::setCellViewer() : cellViewer_" << std::endl;
    cellViewer_ = cell;
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

//  void InstancesWidget::show ()
//  {
//    std::cout << "InstanceWidget::show()" << std::endl;
//    QWidget::show();
//    view_->show();
//  }

  void InstancesWidget::setCell( Cell* cell )
  { 
    baseModel_->setCell(cell);
  }

}
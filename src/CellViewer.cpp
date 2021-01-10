#include <QAction>
#include <QMenuBar>

#include "CellViewer.h"
#include "CellsLib.h"
#include "InstancesWidget.h"
//#include "SaveCellDialog.h"
//#include "OpenCellDialog.h"

namespace Netlist
{
  
  /*------------------------------------------------------------------*
   * Constructeur de CellViewer                                       *
   * - crée une barre de menu                                         *
   * - ajoute des entrées du menu                                     *
   * - affecte des actions à ces entrées                              *
   * STATUS   --  DONE                                                *
   *------------------------------------------------------------------*/
  CellViewer::CellViewer( QWidget* parent )
  : QMainWindow(parent)
  , cellWidget_(NULL)
  , cellsLib_(NULL)
  , instancesWidget_(NULL)
  , saveCellDialog_(NULL)
  {
    std::cout << "[CELL_VIEWER] : Beginning of constructor" << std::endl;

    cellWidget_ = new CellWidget();
    saveCellDialog_ = new SaveCellDialog( this );
    cellsLib_ = new CellsLib(parent);
    cellsLib_->setCellViewer(this);
    instancesWidget_= new InstancesWidget(parent);
    instancesWidget_->setCellViewer(this);

    setCentralWidget( cellWidget_ );
    QMenu* fileMenu = menuBar()->addMenu( "&File" );

    QAction* action = new QAction( "&Open Cell", this );
    action->setStatusTip( "Open selected Cell ");
    action->setShortcut( QKeySequence("CTRL+O") );
    action->setVisible( true );
    fileMenu->addAction( action );
    connect( action, SIGNAL(triggered()), this, SLOT(openCell()) );

    action = new QAction( "&Save As", this );
    action->setStatusTip( "Save to disk (rename) the Cell" );
    action->setShortcut( QKeySequence("CTRL+S") );
    action->setVisible( true );
    fileMenu->addAction( action );
    connect( action, SIGNAL(triggered()), this, SLOT(saveCell()) );

    action = new QAction( "&Quit", this );
    action->setStatusTip( "Exit the Netlist Viewer" );
    action->setShortcut( QKeySequence("CTRL+Q") );
    action->setVisible(true);
    fileMenu->addAction( action );
    connect( action, SIGNAL(triggered()), this, SLOT(close()) );

    action = new QAction( "&Show CellsLib", this);
    action->setStatusTip( "Show the Cell library" );
    action->setShortcut( QKeySequence("CTRL+L") );
    action->setVisible(true);
    fileMenu->addAction( action );
    connect( action, SIGNAL(triggered()), this, SLOT(showCellsLib()) );

    action = new QAction( "&Show InstancesWidget", this);
    action->setStatusTip( "Show the Instance Widget" );
    action->setShortcut( QKeySequence("CTRL+I") );
    action->setVisible(true);
    fileMenu->addAction( action );
    connect( action, SIGNAL(triggered()), this, SLOT(showInstancesWidget()) );
    
    std::cout << "[CELL_VIEWER] : End of constructor" << std::endl;
  }


  /*
   * Destructeur de CellViewer
   */
  CellViewer::~CellViewer()
  {
    //A FAIRE
  }

  /*
   * Méthode de sauvegarde d'une Cell
   * */
  void CellViewer::saveCell()
  {
    Cell* cell = getCell();
    if ( cell == NULL ){
      return;
    }

    QString cellName = cell->getName().c_str();

    if(saveCellDialog_->run(cellName)){
      cell->setName( cellName.toStdString() );
      cell->save( cellName.toStdString() );
    }
  }

  /*------------------------------------------------------------------*
   * Getteur sur la Cell                                              *
   * - Appelle la méthode getCell de CellWidget                       *
   * STATUS   --  DONE                                                *
   *------------------------------------------------------------------*/
  Cell* CellViewer::getCell() const
  {
    return cellWidget_->getCell();
  }

  /*------------------------------------------------------------------*
   * Affectation d'une Cell                                           *
   * - Appelle la méthode setCell de CellWidget                       *
   * - Appelle la méthode setCell d'InstanceWidget                    *
   * STATUS   --  DONE                                                *
   *------------------------------------------------------------------*/
  void CellViewer::setCell ( Cell* cell )
  {
    cellWidget_->setCell(cell);
    instancesWidget_->setCell(cell);
  }

  /*------------------------------------------------------------------*
   * Ouverture d'une Cell                                             *
   * - Récupère le nom de la cell à charger dans le lineEdit          *
   * - Affiche le nom de sa mastercell                                *
   * STATUS   --  TODO                                                *
   *------------------------------------------------------------------*/
  void CellViewer::openCell ()
  {
    QString name;
    if ((name = OpenCellDialog::run()) != NULL){
      std::cout << "[DONE] CellViewer::openCell() : OpenCellDialog::run() name = " << name.toStdString() << std::endl;
    }
    Cell* cell = NULL;
    if ( not (cell = Cell::find(name.toStdString())) ){
      cell = Cell::load(name.toStdString());
      if (cell == NULL){
        std::cerr << "[ERROR] CellViewer::openCell() : cell not found" << std::endl;
        return;
      }
    }
    //A compléter
    cellWidget_->setCell(cell);
    std::cout << "[DONE] CellViewer::openCell() : End of Function" << std::endl;    
    emit cellLoaded();
  }

  void CellViewer::showCellsLib()
  {
    std::cout << "[BEFORE] CellViewer::showCellsLib: show()" << std::endl;
    cellsLib_->show();
    std::cout << "[AFTER]  CellViewer::showCellsLib: show()" << std::endl;
    update();
  }

  void CellViewer::showInstancesWidget()
  {
    std::cout << "[BEFORE] CellViewer::showInstancesWidget: show()" << std::endl;
    instancesWidget_->show();
    std::cout << "[AFTER]  CellViewer::showInstancesWidget: show()" << std::endl;
    update();
  }

} // namespace netlist

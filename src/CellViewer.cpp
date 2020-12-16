#include <QAction>
#include <QMenuBar>

#include "CellViewer.h"
//#include "SaveCellDialog.h"
//#include "OpenCellDialog.h"

namespace Netlist
{
  
  /*------------------------------------------------------------------*
   * Constructeur de CellViewer                                       *
   * - crée une barre de menu                                         *
   * - ajoute des entrées du menu                                     *
   * - affecte des actions à ces entrée                               *
   * STATUS   --  DONE                                                *
   *------------------------------------------------------------------*/
  CellViewer::CellViewer( QWidget* parent )
  : QMainWindow(parent)
  , cellWidget_(NULL)
  , saveCellDialog_(NULL)
  {
    cellWidget_ = new CellWidget();
    saveCellDialog_ = new SaveCellDialog( this );

    setCentralWidget( cellWidget_ );
    QMenu* fileMenu = menuBar()->addMenu( "&File" );

    QAction* action = new QAction( "&Open Cell", this );
    action->setStatusTip( "Open selected Cell ");
    action->setShortcut( QKeySequence("CTRL+C") );
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
   * STATUS   --  DONE                                                *
   *------------------------------------------------------------------*/
  void CellViewer::setCell ( Cell* cell )
  {
    cellWidget_->setCell(cell);
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
    if ( (cell = Cell::find(name.toStdString())) == NULL){
      cell = Cell::load(name.toStdString());
      if (cell == NULL){
        std::cerr << "[ERROR] CellViewer::openCell() : cell not found" << std::endl;
        return;
      }
    }
    //A compléter
    cellWidget_->setCell(cell);
    std::cout << "[DONE] CellViewer::openCell() : End of Function" << std::endl;

    QAction* action = new QAction( "&CellLoaded", this );
    connect( action, SIGNAL(cellLoaded()), this, SLOT(CellModels::updateDatas()) );

  }

} // namespace netlist

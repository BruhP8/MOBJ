#include <QAction>
#include <QMenuBar>

#include "CellViewer.h"
//#include "SaveCellDialog.h"

namespace Netlist
{
  
  /* Constructeur de CellViewer
   * 
   * */
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

  /*
   *
   * */
  Cell* CellViewer::getCell() const
  {
    return cellWidget_->getCell();
  }

  void CellViewer::setCell ( Cell* cell )
  {
    cellWidget_->setCell(cell);
  }

  //A FAIRE
  void CellViewer::openCell ()
  {
    QString name;
    if ((name = OpenCellDialog::run()) != NULL){
      std::cout << "[DONE] CellViewer::opendCell() : OpenCellDialog::run() name = " << name.toStdString() << std::endl;
    }

    
    //A compléter

    std::cerr << "[DONE] CellViewer::openCell() : End of Function" << std::endl;

  }

} // namespace netlist

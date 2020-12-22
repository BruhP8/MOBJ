//#include <libxml/xmlreader.h>
//#include <QWidget>
//#include <QWindow>
#include <QMainWindow>
#include "Cell.h"
#include "CellWidget.h"
#include "SaveCellDialog.h"
#include "OpenCellDialog.h"
//#include "CellsLib.h"
//#include "InstancesWidget.h"

namespace Netlist
{

  //class SaveCellDialog;
  //class OpenCellDialog;
  class CellsLib;
  class InstancesWidget;


  class CellViewer : public QMainWindow {
    
    Q_OBJECT;
  public:
                      CellViewer          ( QWidget* parent=NULL );
    virtual          ~CellViewer          ();
            Cell*     getCell             () const;
    inline  CellsLib* getCellsLib         (){ return cellsLib_; }  // TME9+.
  public slots:
            void      setCell             ( Cell* );
            void      saveCell            ();
            void      openCell            ();
            void      showCellsLib        ();  // TME9+.
            void      showInstancesWidget ();  // TME9+.
  
  private:
    CellWidget*      cellWidget_;
    CellsLib*        cellsLib_;         // TME9+.
    InstancesWidget* instancesWidget_;  // TME9+.
    SaveCellDialog*  saveCellDialog_;

  signals:
    void cellLoaded();

  };

//  inline CellsLib* CellViewer::getCellsLib () { return cellsLib; }

} // namespace netlist

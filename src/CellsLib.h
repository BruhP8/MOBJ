//#include <QWidget>
#include <QTableView>
#include <QPushButton>

//#include "CellViewer.h"
#include "CellsModel.h"

namespace Netlist {

  class CellViewer;

  class CellsLib : public QWidget {
    Q_OBJECT;

    public :
                          CellsLib        ( QWidget* parent=NULL );
                         ~CellsLib        ();
              void        setCellViewer   ( CellViewer* );
              int         getSelectedRow  () const;
      inline  CellsModel* getBaseModel    ();
    
    public slots :
      void                load            ();

    private :
      CellViewer*   cellViewer_;
      CellsModel*   baseModel_;
      QTableView*   view_;
      QPushButton*  load_;

  };

  inline CellsModel* CellsLib::getBaseModel() { return baseModel_; }

}
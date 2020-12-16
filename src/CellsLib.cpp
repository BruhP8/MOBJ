#include "CellsLib.h"

namespace Netlist {

  CellsLib::CellsLib( QWidget* parent )
  : QWidget(parent)
  , cellViewer_(NULL);
  , baseModel_(this)
  , 
  {}

  void CellsLib::setCellViewer( CellViewer* cv ){
    cellViewer_ = cv;
  }

  //TODO
  int getSelectedRow() const {
    return 0;
  }

  //TODO
  void load() {

  }

}
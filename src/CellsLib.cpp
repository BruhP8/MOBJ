#include <iostream>

#include "CellsLib.h"

namespace Netlist {

  CellsLib::CellsLib( QWidget* parent )
  : QWidget(parent)
  , cellViewer_(NULL)
  , baseModel_(NULL)
  {
    std::cout << "[CELLS_LIB] : Constructor OK" << std::endl;
  }

  CellsLib::~CellsLib()
  {}

  /* Problème de pointeur : 
   * - */
  void CellsLib::setCellViewer( CellViewer* cv ){
    std::cout << "[CELL_LIB] : setCellViewer()" << std::endl;
    
    //cellViewer_ = cv;

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
// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/7>" -*-

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
using namespace std;

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
using namespace Netlist;

#include <QApplication>
#include <QtGui>
#include "CellViewer.h"

int main ( int argc, char* argv[] )
{
  std::cout << "[MAIN] : Creation of QApplication" << std::endl;
  QApplication* qa = new QApplication(argc, argv);

  std::cout << "[MAIN] : Creation of CellViewer" << std::endl;
  CellViewer* viewer = new CellViewer();
  //viewer->setCell( halfadder );
  viewer->show();

  std::cout << "[MAIN] : Execution of QApplication" << std::endl;
  int rvalue = qa->exec();
  delete qa;
  return rvalue;
}

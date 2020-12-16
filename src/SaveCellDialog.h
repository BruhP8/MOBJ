#ifndef NETLIST_SAVECELLDIALOG_H
#define NETLIST_SAVECELLDIALOG_H

#include <QDialog>
#include <QLineEdit>
//#include "CellViewer.h"

namespace Netlist{
  class SaveCellDialog : public QDialog {
    
    class CellViewer;
    
    Q_OBJECT;
    public :
                    SaveCellDialog(QWidget* parent = NULL);
      bool          run(QString& name);
      const QString getCellName() const;
      void          setCellName(const QString& name);
    protected :
      QLineEdit*    lineEdit_;
    
    };
}

#endif  // NETLIST_SAVECELLDIALOG_H
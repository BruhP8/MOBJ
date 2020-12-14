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
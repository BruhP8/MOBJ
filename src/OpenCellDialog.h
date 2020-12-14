#include <QDialog>
#include <QLineEdit>

namespace Netlist {
  class OpenCellDialog : public QDialog {

    class CellViewer;

    Q_OBJECT;
    public :
                        OpenCellDialog(QWidget* parent = NULL);
      virtual          ~OpenCellDialog();
      static  QString   run           ();
      const   QString   getCellName   () const;
              void      setCellName   ( const QString& name );
    protected :
      QLineEdit*    lineEdit_;
  };
}
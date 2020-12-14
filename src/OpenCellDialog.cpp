#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>

#include "OpenCellDialog.h"

using namespace std;

namespace Netlist
{
  OpenCellDialog::OpenCellDialog(QWidget* parent)
  :QDialog( parent )
  ,lineEdit_( NULL )
  {
    setWindowTitle( tr("Open Cell") );
    QLabel *label = new QLabel();
    label->setText( tr("Enter Cell Name") );
    lineEdit_ = new QLineEdit();
    lineEdit_->setMinimumWidth(400);

    QPushButton* okButton = new QPushButton();
    okButton->setText   ( "ok" );
    okButton->setDefault( true );
  
    QPushButton* cancelButton = new QPushButton();
    cancelButton->setText( "cancel" );

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget( okButton );
    hLayout->addStretch();
    hLayout->addWidget( cancelButton );
    hLayout->addStretch();

    QFrame* separator = new QFrame();
    separator->setFrameShape ( QFrame::HLine );
    separator->setFrameShadow( QFrame::Sunken );

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setSizeConstraint( QLayout::SetFixedSize );
    vLayout->addWidget( label );
    vLayout->addWidget( lineEdit_ );
    vLayout->addLayout( hLayout );
    setLayout( vLayout );

    connect(     okButton, SIGNAL(clicked()), this, SLOT(accept()) );
    connect( cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
  }

  OpenCellDialog::~OpenCellDialog()
  {}

  

  const QString OpenCellDialog::getCellName() const
  {
    return lineEdit_->text();
  }

  void OpenCellDialog::setCellName( const QString& name )
  {
    lineEdit_->setText(name);
  }

  /* Alors, ici je pense qu'il faut retourner le nom récupéré dans le lineEdit
   * On renvoie une copie de la QString car lors de la suppression de l'objet 
   * OpenCellDialog, tout est supprimé (y compris la QString) */
  QString OpenCellDialog::run()
  {
    OpenCellDialog* dialog = new OpenCellDialog();  //Création "à la volée" de l'OpenCellDialog
    int dialogResult = dialog->exec();  //Affichage de la fenêtre de dialogue
    QString name = dialog->getCellName(); //On récupère juste le nom passé dans le lineEdit pour l'afficher ensuite
    std::cout << "[DONE] OpenCellDialog::run() : Name of Cell = " << name.toStdString() << std::endl;

    delete dialog;  //Suppression de l'objet

    return name;
  }

} // namespace Netlist

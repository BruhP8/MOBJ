// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
//#include  <QColor>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "ArcShape.h"
#include  "EllipseShape.h"
#include  "LineShape.h"
#include  "TermShape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"

namespace Netlist {

  using namespace std;


  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



  CellWidget::CellWidget ( QWidget* parent )
    : QWidget   (parent)
    , cell_     (NULL)
    , viewport_ (Box(0, 0, 500, 500))
  {
    setAttribute    ( Qt::WA_OpaquePaintEvent );
    setAttribute    ( Qt::WA_NoSystemBackground );
    setAttribute    ( Qt::WA_StaticContents );
    setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy  ( Qt::StrongFocus );
    setMouseTracking( true );
  }


  CellWidget::~CellWidget ()
  { }


  void  CellWidget::setCell ( Cell* cell )
  {
    cell_ = cell;
    repaint();
  }


  QSize  CellWidget::minimumSizeHint () const
  { return QSize(500,500); }


  void  CellWidget::resizeEvent ( QResizeEvent* event )
  { 
    const QSize& size = event->size();

    viewport_.setX2( viewport_.getX1() + size.width() );
    viewport_.setY1( viewport_.getY2() - size.height() );  
    
    cerr << "CellWidget::resizeEvent() viewport_:" << viewport_ << endl;
    //repaint(); 
  }


  void  CellWidget::paintEvent ( QPaintEvent* event )
  {

    QFont  bigFont = QFont( "URW Bookman L", 36 );

    QString cellName = "NULL";
    if (cell_) cellName = cell_->getName().c_str();

    QPainter painter(this);
    painter.setFont      ( bigFont );
    painter.setBackground( QBrush( Qt::black ) );
    painter.eraseRect    ( QRect( QPoint(0,0), size() ) );

    painter.setPen( QPen( Qt::darkGreen, 1) );
    QRect rect1 = boxToScreenRect(viewport_);
    painter.drawRect( rect1 );

    //painter.setPen( QPen( Qt::red, 0) );
    //painter.setBrush( QBrush( Qt::red ) );
    //QRect rect2 = boxToScreenRect(viewport_);
    //painter.drawRect( rect2 );

    /*Affichage du contenu de la Cell*/
    Symbol* symbol = cell_->getSymbol();
    if (symbol){
      painter.setPen( QPen( Qt::blue, 3 ) );
      QRect rect2 = boxToScreenRect( symbol->getBoundingBox() );
      painter.drawRect( rect2 );
      painter.setPen( QPen( Qt::darkCyan, 3 ) );
      for( Shape* sh : symbol->getShapes() ){
        QRect rect3 = boxToScreenRect( sh->getBoundingBox() );

        
        if (ArcShape* arcSh = dynamic_cast<ArcShape*>(sh)){
          painter.setPen( QPen( Qt::green, 2 ) );
          painter.drawArc( rect3, arcSh->startAngle(), arcSh->spanAngle() );
          std::cout << "[CELL_WIDGET] drawing ArcShape : start=" << arcSh->startAngle()
                    << " span=" << arcSh->spanAngle() << std::endl;
          //painter.setPen( QPen( Qt::red, 3) );
          //painter.drawRect( rect4 );
          //continue;
        }

        
        if (dynamic_cast<EllipseShape*>(sh)){
          painter.setPen( QPen( Qt::darkMagenta, 2 ) );
          painter.drawEllipse( rect3 );
          //painter.setPen( QPen( Qt::magenta, 3 ) );
          continue;
        }

        if (LineShape* lsh = dynamic_cast<LineShape*>(sh)){
          painter.setPen( QPen( Qt::cyan, 2 ) );
          painter.drawLine( pointToScreenPoint( lsh->startPoint() ),
                            pointToScreenPoint( lsh->endPoint() ) );
          continue;
        }

        if (TermShape* tsh = dynamic_cast<TermShape*>(sh)){
          painter.setPen( QPen(Qt::magenta, 5) );
          painter.drawPoint( QPoint(tsh->getX(), tsh->getY()) );
          continue;
        }
        
        else {
          if (sh){
            painter.setPen( QPen( Qt::gray, 3 ) );
            painter.drawRect(rect3);
          }
        }

      }
    }

    int frameWidth  = 460;
    int frameHeight = 100;
    QRect nameRect ( (size().width ()-frameWidth )/2
                   , (size().height()-frameHeight)/2
                   , frameWidth
                   , frameHeight
                   );

    painter.drawRect( nameRect );
    painter.drawText( nameRect, Qt::AlignCenter, cellName );
  }

  void CellWidget::keyPressedEvent( QKeyEvent* event )
  {
    event->ignore();
    if (event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier))
      return;

    switch( event->key() ){
      case Qt::Key_Up     : goUp();     break;
      case Qt::Key_Down   : goDown();   break;
      case Qt::Key_Left   : goLeft();   break;
      case Qt::Key_Right  : goRight();  break;
      default             : return;
    }
    event->accept();
  }

  void CellWidget::goRight() 
  {
    viewport_.translate( Point(20, 0) );
    repaint();
  }

  void CellWidget::goUp()
  {
    viewport_.translate( Point(0, 20) );
    repaint();
  }

  void CellWidget::goLeft()
  {
    viewport_.translate( Point(-20, 0) );
    repaint();
  }

  void CellWidget::goDown()
  {
    viewport_.translate( Point(0, -20) );
    repaint();
  }

  //void CellWidget::query( unsigned int flags, QPainter& painter )
  //{
  //  if ( (not cell_) or (not flags) ) return;
//
  //  const vector<Instance*>& instances = cell->getInstances();
  //  for( size_t i; i < instances.size(); ++i ){
  //    Point instPos = instances[i]->getPosition();
  //    const Symbol* symbol = instances[i]->getMasterCell()->getSymbol();
  //    if (not symbol) continue;
//
  //    if (flages & InstanceShapes){
  //      const vector<Shape*>& shapes = symbol->getShapes();
  //      for( size_t j = 0; j <= shapes.size(); ++j ){
  //        BoxShape* boxShape = dynamic_cast<BoxShape*>(shapes[j]);
  //        if (boxShape){
  //          Box box = boxShape->getBoundingBox();
  //          QRect rect = boxToScreenRect(box.translate(instPos));
  //          painter.drawRect(rect);
  //        }
  //      }
  //    }
  //  }
  //}


}  // Netlist namespace.

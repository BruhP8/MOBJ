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
//#include  "Symbol.h"
#include  "Shape.h"
#include  "ArcShape.h"
#include  "EllipseShape.h"
#include  "LineShape.h"
#include  "TermShape.h"
#include  "BoxShape.h"
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

    viewport_.setX2( viewport_.getX1() + size.width()  -1 );
    viewport_.setY1( viewport_.getY2() - size.height() +1 );  
    
    //cerr << "CellWidget::resizeEvent() viewport_:" << viewport_ << endl;
    repaint(); 
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

    painter.setPen( QPen( Qt::magenta, 2) );
    QRect rect1 = boxToScreenRect(viewport_);
    painter.drawRect( rect1 );

    painter.setPen( QPen( Qt::blue, 3 ) );

    if ( (cell_) and (cell_->getInstances().size() == 0) ){
      drawCellsSymbol( 1, painter );
    }

    query( 1, painter );
    drawNets(painter);

    painter.setFont      ( bigFont );
    painter.setPen       ( QPen( Qt::red, 2 ) );

    int frameWidth  = 460;
    int frameHeight = 100;
    QRect nameRect ( (size().width()-frameWidth )/2
                   , ((size().height()/2)-frameHeight)/2
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

  void CellWidget::query( unsigned int flags, QPainter& painter )
  {
    if ( (not cell_) or (not flags) ) return;

    QFont  littleFont = QFont( "Helvetica", 12 );
    painter.setFont(littleFont);

    const vector<Instance*>& instances = cell_->getInstances();
    for( size_t i; i < instances.size(); ++i ){
      Point instPos = instances[i]->getPosition();
      const Symbol* symbol = instances[i]->getMasterCell()->getSymbol();
      if (not symbol) continue;

      if ((flags) and (symbol)){
        const vector<Shape*>& shapes = symbol->getShapes();
        for( Shape* sh : shapes ){
          BoxShape* boxShape = dynamic_cast<BoxShape*>(sh);
          if (boxShape){
            Box box = boxShape->getBoundingBox();
            QRect rect = boxToScreenRect(box.translate(instPos));
            painter.drawRect(rect);
            continue;
          }

          ArcShape* arcSh = dynamic_cast<ArcShape*>(sh);
          if (arcSh){
            Box box = arcSh->getBoundingBox();
            QRect rect = boxToScreenRect(box.translate(instPos));
            painter.setPen( QPen( Qt::green, 2 ) );
            painter.drawArc( rect, arcSh->startAngle(), arcSh->spanAngle() );
            continue;
          }

          EllipseShape* elSh = dynamic_cast<EllipseShape*>(sh);
          if (elSh){
            Box box = elSh->getBoundingBox();
            QRect rect = boxToScreenRect(box.translate(instPos));
            painter.setPen( QPen( Qt::darkMagenta, 2 ) );
            painter.drawEllipse( rect );
            continue;
          }

          LineShape* lsh = dynamic_cast<LineShape*>(sh);
          if (lsh){
            Box box = lsh->getBoundingBox().translate(instPos);
            painter.setPen( QPen( Qt::cyan, 2 ) );
            Point start = Point( box.getX1(), box.getY1() );
            Point end   = Point( box.getX2(), box.getY2() );
            painter.drawLine( pointToScreenPoint( start ),
                              pointToScreenPoint( end ) );
            continue;
          }

          if (TermShape* tsh = dynamic_cast<TermShape*>(sh)){
            Point p = Point(tsh->getX(), tsh->getY());
            Term* term = tsh->getTerm();

            p.translate(instPos);
            painter.setPen( QPen(Qt::magenta, 5) );
            painter.drawPoint( pointToScreenPoint(p) );
            painter.drawText( QPoint( xToScreenX(p.getX()-5), yToScreenY(p.getY())+20)
                            , QString(term->getName().c_str()) );

            continue;
          }

        }
      }

    }
    const vector<Term*>& terms = cell_->getTerms();
    for( size_t i = 0; i < terms.size(); ++i ){
      Point termPos = terms[i]->getPosition();

      painter.setPen( QPen(Qt::yellow, 10) );
      painter.drawPoint( pointToScreenPoint(termPos) );
      QPoint textPos;
      if ( terms[i]->getDirection() == Term::Direction::In ){
        textPos = QPoint( xToScreenX(termPos.getX() - terms[i]->getName().size())
                        , yToScreenY(termPos.getY() - 15) );
      }
      else {
        textPos = QPoint( xToScreenX(termPos.getX() + terms[i]->getName().size())
                        , yToScreenY(termPos.getY() - 15) );
      }
      painter.drawText( textPos, QString( terms[i]->getName().c_str() ) );
    }

  }

  void CellWidget::drawNets( QPainter& painter ){
    if (not cell_) return;

    const vector<Net*>& nets = cell_->getNets();
    for(Net* net : nets){

      painter.setPen( QPen(Qt::gray, 1) );
      for(Line* line : net->getLines()){
        std::cout << "Line = " << line->getSource()->getId() << ": (" 
                  << line->getSourcePosition().getX() << ", "  << line->getSourcePosition().getY() << ") "
                  << line->getTarget()->getId() << ": (" 
                  << line->getTargetPosition().getX() << ", "  << line->getTargetPosition().getY() << ")" << std::endl;
        painter.drawLine( pointToScreenPoint(line->getSourcePosition())
                        , pointToScreenPoint(line->getTargetPosition()));
      }

    }
  }

  void CellWidget::drawCellsSymbol( unsigned int flags, QPainter &painter ){
    
    if ( (not cell_) or (not flags) ) return;

    /*Des Cells à partir de leur description en symboles*/
    Symbol* symbol = cell_->getSymbol();
    if (symbol){

      for( Shape* sh : symbol->getShapes() ){
        QRect rect = boxToScreenRect( sh->getBoundingBox() );

        
        if (ArcShape* arcSh = dynamic_cast<ArcShape*>(sh)){
          painter.setPen( QPen( Qt::green, 2 ) );
          painter.drawArc( rect, arcSh->startAngle(), arcSh->spanAngle() );
          std::cout << "[CELL_WIDGET] drawing ArcShape : start=" << arcSh->startAngle()
                    << " span=" << arcSh->spanAngle() << std::endl;
          continue;
        }

        
        if (dynamic_cast<EllipseShape*>(sh)){
          painter.setPen( QPen( Qt::darkMagenta, 2 ) );
          painter.drawEllipse( rect );
          painter.setPen( QPen( Qt::magenta, 3 ) );
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
          painter.drawPoint( QPoint(xToScreenX(tsh->getX()), yToScreenY(tsh->getY())) );
          continue;
        }
        
        else {
          if (sh){
            painter.setPen( QPen( Qt::gray, 3 ) );
            painter.drawRect(rect);
          }
        }

      }
    }

  }

}  // Netlist namespace.
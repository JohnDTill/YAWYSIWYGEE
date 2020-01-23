#include "edit.h"

#include "document.h"
#include "globals.h"
#include <QScrollBar>
#include <QWheelEvent>

namespace Typeset{

Edit::Edit(){
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    verticalScrollBar()->setCursor(Qt::CursorShape::ArrowCursor);
    horizontalScrollBar()->setCursor(Qt::CursorShape::ArrowCursor);

    setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void Edit::setScene(QGraphicsScene* scene){
    if(QGraphicsView::scene()){
        disconnect(QGraphicsView::scene(),SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),
                   this,SLOT(ensureFocusedItemVisible(QGraphicsItem*)));
    }

    QGraphicsView::setScene(scene);

    if(scene){
        connect(scene,SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),
                this,SLOT(ensureFocusedItemVisible(QGraphicsItem*)));
    }
}

void Edit::zoomIn(){
    qreal candidate = transform().m11() * scale_in_factor;

    if(candidate > max_scale){
        qreal scale_to_max = max_scale / transform().m11();
        scale(scale_to_max, scale_to_max);
    }else{
        scale(scale_in_factor, scale_in_factor);
    }
}

void Edit::zoomOut(){
    qreal candidate = transform().m11() * scale_out_factor;

    if(candidate < min_scale){
        qreal scale_to_min = min_scale / transform().m11();
        scale(scale_to_min, scale_to_min);
    }else{
        scale(scale_out_factor, scale_out_factor);
    }
}

void Edit::resetZoom(){
    resetTransform();
}

Document* Edit::doc(){
    return static_cast<Document*>(scene());
}

qreal Edit::heightInSceneCoordinates(){
    return mapToScene(0, height()).y() - mapToScene(0,0).y();
}

void Edit::mouseMoveEvent(QMouseEvent* e){
    qreal x = mapToScene(e->pos()).x();
    if(x > -linebox_offet) setCursor(Qt::IBeamCursor);
    else setCursor(Qt::ArrowCursor);

    dragging = true;
    QGraphicsView::mouseMoveEvent(e);
    dragging = false;
}

void Edit::wheelEvent(QWheelEvent* e){
    if( e->modifiers() == Qt::ControlModifier ){
        if(e->delta() > 0) zoomIn();
        else zoomOut();
    }else{
        verticalScrollBar()->event(e);
    }
}

void Edit::keyPressEvent(QKeyEvent* e){
    #define MATCH(arg) e->matches(QKeySequence::arg)

    if     (MATCH(ZoomIn))  zoomIn();
    else if(MATCH(ZoomOut)) zoomOut();
    else if(MATCH(MoveToNextPage))     doc()->cursor->moveToNextPage( heightInSceneCoordinates() );
    else if(MATCH(MoveToPreviousPage)) doc()->cursor->moveToPreviousPage( heightInSceneCoordinates() );
    else if(MATCH(SelectNextPage))     doc()->cursor->selectNextPage( heightInSceneCoordinates() );
    else if(MATCH(SelectPreviousPage)) doc()->cursor->selectPreviousPage( heightInSceneCoordinates() );
    else if(MATCH(New)) DO_THIS( "New" )
    else if(MATCH(Open)) DO_THIS( "Open" )
    else if(MATCH(Save)) DO_THIS( "Save" )
    else if(MATCH(SaveAs)) DO_THIS( "SaveAs" )
    else if(MATCH(Refresh)) DO_THIS( "Refresh" )
    else if(MATCH(Open)) DO_THIS( "Close" )
    else if(MATCH(Print)) DO_THIS( "Print" )
    else QGraphicsView::keyPressEvent(e);

    doc()->updateCursorView();

    #undef MATCH
}

void Edit::ensureFocusedItemVisible(QGraphicsItem* newFocusItem){
    if(newFocusItem && !dragging) ensureVisible(newFocusItem, 30, 15);
}

}

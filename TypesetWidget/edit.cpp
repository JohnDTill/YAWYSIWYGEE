#include "edit.h"

#include "document.h"
#include "globals.h"
#include "parser.h"
#include <QFile>
#include <QFileDialog>
#include <QScrollBar>
#include <QWheelEvent>

namespace Typeset{

Edit::Edit(bool allow_write, bool show_line_numbers){
    Typeset::Globals::initGlobals();

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    verticalScrollBar()->setCursor(Qt::CursorShape::ArrowCursor);
    horizontalScrollBar()->setCursor(Qt::CursorShape::ArrowCursor);

    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    doc = new Typeset::Document(allow_write, show_line_numbers);
    setDocument(doc);
}

void Edit::setDocument(Document* scene){
    if(QGraphicsView::scene()){
        disconnect(QGraphicsView::scene(),SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),
                   this,SLOT(ensureFocusedItemVisible(QGraphicsItem*)));
    }

    QGraphicsView::setScene(scene);

    if(scene){
        connect(scene,SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),
                this,SLOT(ensureFocusedItemVisible(QGraphicsItem*)));

        connect(doc->undo_stack, SIGNAL(canUndoChanged(bool)), this, SLOT(passUndo(bool)));
        connect(doc->undo_stack, SIGNAL(canRedoChanged(bool)), this, SLOT(passRedo(bool)));
    }
}

QString Edit::toCode() const{
    QString str;
    QTextStream out(&str);
    out.setCodec("UTF-8");

    doc->write(out);

    return str;
}

void Edit::setCode(QString& code){
    QTextStream in(&code);
    in.setCodec("UTF-8");

    if(!Parser::isValidCode(in)){
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "Invalid code.");
        messageBox.setFixedSize(500,200);
        return;
    }

    delete doc;
    doc = Parser::parseDocument(in, doc->allow_write, doc->show_line_nums);
    setDocument(doc);

    //Make sure the view gets the focus item signal
    QGraphicsItem* focus_item = doc->focusItem();
    doc->clearFocus();
    if(focus_item) focus_item->setFocus();
}

void Edit::copyAsPng(){
    doc->copyAsPng();
}

void Edit::updateTheme(){
    doc->updateTheme();
}

void Edit::newDocument(bool allow_write, bool show_line_numbers){
    delete doc;
    doc = new Typeset::Document(allow_write, show_line_numbers);
    setDocument(doc);
}

void Edit::load(QString filename, bool allow_write, bool show_line_numbers){
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "Could not open \"" + filename + "\" to read.");
        messageBox.setFixedSize(500,200);
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    if(!Parser::isValidCode(in)){
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "Invalid code in \"" + filename + "\".");
        messageBox.setFixedSize(500,200);
        return;
    }

    delete doc;
    doc = Parser::parseDocument(in, allow_write, show_line_numbers);
    doc->save_path = filename;
    setDocument(doc);

    //Make sure the view gets the focus item signal
    QGraphicsItem* focus_item = doc->focusItem();
    doc->clearFocus();
    if(focus_item) focus_item->setFocus();
}

void Edit::loadPrompt(bool allow_write, bool show_line_numbers){
    QString file_name = QFileDialog::getOpenFileName(nullptr, tr("Load File"),
                                "./",
                                tr("Text (*.txt)"));

    if(file_name.isEmpty()) return;
    else load(file_name, allow_write, show_line_numbers);
}

void Edit::save(){
    doc->save();
}

void Edit::saveAs(QString save_path){
    doc->saveAs(save_path);
}

void Edit::savePrompt(){
    doc->savePrompt();
}

void Edit::printSvgPrompt(){
    doc->printSvgPrompt();
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

void Edit::setLineNumbersVisible(bool show){
    doc->setLineNumbersVisible(show);
}

void Edit::undo(){
    doc->undo_stack->undo();
}

void Edit::redo(){
    doc->undo_stack->redo();
}

qreal Edit::heightInSceneCoordinates() const {
    return mapToScene(0, height()).y() - mapToScene(0,0).y();
}

void Edit::mouseMoveEvent(QMouseEvent* e){
    qreal x = mapToScene(e->pos()).x();
    if(x > -linebox_offet) setCursor(Qt::IBeamCursor);
    else setCursor(Qt::ArrowCursor);

    QGraphicsView::mouseMoveEvent(e);
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
    else if(MATCH(MoveToNextPage))     doc->cursor->moveToNextPage( heightInSceneCoordinates() );
    else if(MATCH(MoveToPreviousPage)) doc->cursor->moveToPreviousPage( heightInSceneCoordinates() );
    else if(MATCH(SelectNextPage))     doc->cursor->selectNextPage( heightInSceneCoordinates() );
    else if(MATCH(SelectPreviousPage)) doc->cursor->selectPreviousPage( heightInSceneCoordinates() );
    else if(MATCH(New)) DO_THIS( "New" )
    else if(MATCH(Open)) DO_THIS( "Open" )
    else if(MATCH(Save)) DO_THIS( "Save" )
    else if(MATCH(SaveAs)) DO_THIS( "SaveAs" )
    else if(MATCH(Refresh)) DO_THIS( "Refresh" )
    else if(MATCH(Open)) DO_THIS( "Close" )
    else if(MATCH(Print)) DO_THIS( "Print" )
    else QGraphicsView::keyPressEvent(e);

    doc->updateCursorView();

    #undef MATCH
}

void Edit::ensureFocusedItemVisible(QGraphicsItem* newFocusItem){
    if(newFocusItem) ensureVisible(newFocusItem, 30, 15);
}

void Edit::passUndo(bool available){
    emit undoAvailable(available);
}

void Edit::passRedo(bool available){
    emit redoAvailable(available);
}

}

#include "document.h"

#include "algorithm.h"
#include "construct.h"
#include "cursor.h"
#include "globals.h"
#include "line.h"
#include "parser.h"
#include "text.h"
#include <QCursor>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QtMath>
#include <QtSvg/QSvgGenerator>

namespace Typeset{

Document::Document(bool allow_write, bool show_line_numbers, Line* f, Line* b, QString save_path)
    : save_path(save_path),
      front(f),
      back(b),
      allow_write(allow_write) {
    double_click_time = QTime::currentTime();

    if(front==nullptr){
        front = new Line();
        front->next = nullptr;
        front->prev = nullptr;
    }
    if(back==nullptr) back = front;

    addItem(front);
    front->setPos(0, first_line_vspace);

    updateSize();

    if(!show_line_numbers) setLineNumbersVisible(false);

    cursor = new Cursor(*this);
    cv = new CursorView(*this);
    cv->update(*cursor);

    undo_stack = new QUndoStack();
}

Document::~Document(){
    while(undo_stack->canRedo()) //QUndoStack deletion is FIFO, which causes crashes undo() is called
        undo_stack->redo();      //so that a latter redo() depends on data from an earlier one.
    delete undo_stack;
    delete cursor;
    delete cv;

    Line* curr = back;
    while(curr){
        Line* prev = curr;
        curr = curr->prev;
        prev->deletePostorder();
    }
}

void Document::save(){
    if(save_path.isEmpty()) savePrompt();
    else saveAs(save_path);
}

void Document::saveAs(QString save_path){
    this->save_path = save_path;
    QFile file(save_path);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "Could not open \"" + save_path + "\" to write.");
        messageBox.setFixedSize(500,200);
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    write(out);
}

void Document::savePrompt(){
    QString file_name = QFileDialog::getSaveFileName(nullptr, tr("Save File"),
                                save_path.isEmpty() ? "./untitled" : save_path,
                                tr("Text (*.txt)"));

    if(!file_name.isEmpty()) saveAs(file_name);
}

void Document::printSvgPrompt(){
    QString prompt_name = "./" + save_path + ".svg";
    QString file_name = QFileDialog::getSaveFileName(nullptr, tr("Export PDF"),
                                prompt_name,
                                tr("SVG (*.svg)"));

    if(file_name.isEmpty()) return;

    QRectF bounds = sceneRect();
    int x = qCeil(bounds.x());
    int y = qCeil(bounds.y());
    int w = qCeil(bounds.width());
    int h = qCeil(bounds.height());

    QSvgGenerator svgGen;
    svgGen.setFileName(file_name);
    svgGen.setSize(QSize(w, h));
    svgGen.setViewBox(QRect(x, y, w, h));

    QPainter painter(&svgGen);
    render(&painter, bounds);
}

void Document::setLineNumbersVisible(bool show){
    if(show_line_nums == show) return;

    show_line_nums = show;

    if(show){
        update();
        w = w + linebox_width + linebox_offet - horizontal_scroll_padding;
        setSceneRect(QRectF(-linebox_width-linebox_offet, -margin_top, w, h));
    }else{
        w = w - linebox_width - linebox_offet + horizontal_scroll_padding;
        setSceneRect(QRectF(-horizontal_scroll_padding, -margin_top, w, h));
    }
}

void Document::write(QTextStream& out) const{
    for(Line* l = front; l; l = l->next) l->write(out);
}

void Document::updateCursorView(){
    cv->update(*cursor);
}

void Document::updateSize(){
    h = margin_top + margin_bot + front->u + front->d;
    w = front->w;
    for(Line* l = front->next; l; l = l->next){
        h += line_vspace + l->u + l->d;
        w = qMax(w, l->w);
    }

    qreal left_width = show_line_nums ? linebox_width+linebox_offet : horizontal_scroll_padding;
    w += horizontal_scroll_padding + left_width;
    setSceneRect(QRectF(-left_width, -margin_top, w, h));
}

void Document::copyAsPng(qreal upscale){
    QImage image(upscale*sceneRect().size().toSize(), QImage::Format_RGB16);
    image.fill(palette().base().color());

    QPainter painter(&image);
    render(&painter);
    QApplication::clipboard()->setImage(image, QClipboard::Clipboard);
}

void Document::copySelectionAsPng(qreal upscale){
    if(!cursor->hasSelection()) return;

    cursor->copy();

    Document doc(true, false);
    doc.paste();
    doc.copyAsPng(upscale);
}

void Document::drawBackground(QPainter* painter, const QRectF& rect){
    QGraphicsScene::drawBackground(painter, rect);
    if(show_line_nums){
        painter->setBrush(palette().window());
        painter->setPen(palette().midlight().color());
        painter->drawRect(QRectF(-1-linebox_width-linebox_offet, -margin_top-1, linebox_width+1, 1e8));
    }
}

void Document::keyPressEvent(QKeyEvent* e){
    #define MATCH(arg) e->matches(QKeySequence::arg)

    if(allow_write && MATCH(Undo))      undo_stack->undo();
    else if(allow_write && MATCH(Redo)) undo_stack->redo();
    else if(MATCH(MoveToNextChar))          cursor->moveToNextChar();
    else if(MATCH(MoveToPreviousChar))      cursor->moveToPreviousChar();
    else if(MATCH(MoveToNextWord))          cursor->moveToNextWord();
    else if(MATCH(MoveToPreviousWord))      cursor->moveToPreviousWord();
    else if(MATCH(MoveToNextLine))          cursor->moveToNextLine();
    else if(MATCH(MoveToPreviousLine))      cursor->moveToPreviousLine();
    else if(MATCH(MoveToStartOfLine))       cursor->moveToStartOfLine();
    else if(MATCH(MoveToEndOfLine))         cursor->moveToEndOfLine();
    else if(MATCH(MoveToStartOfBlock))      cursor->moveToStartOfBlock();
    else if(MATCH(MoveToEndOfBlock))        cursor->moveToEndOfBlock();
    else if(MATCH(MoveToStartOfDocument))   cursor->moveToStartOfDocument();
    else if(MATCH(MoveToEndOfDocument))     cursor->moveToEndOfDocument();
    else if(MATCH(SelectNextChar))          cursor->selectNextChar();
    else if(MATCH(SelectPreviousChar))      cursor->selectPreviousChar();
    else if(MATCH(SelectNextWord))          cursor->selectNextWord();
    else if(MATCH(SelectPreviousWord))      cursor->selectPreviousWord();
    else if(MATCH(SelectNextLine))          cursor->selectNextLine();
    else if(MATCH(SelectPreviousLine))      cursor->selectPreviousLine();
    else if(MATCH(SelectStartOfLine))       cursor->selectStartOfLine();
    else if(MATCH(SelectEndOfLine))         cursor->selectEndOfLine();
    else if(MATCH(SelectStartOfBlock))      cursor->selectStartOfBlock();
    else if(MATCH(SelectEndOfBlock))        cursor->selectEndOfBlock();
    else if(MATCH(SelectStartOfDocument))   cursor->selectStartOfDocument();
    else if(MATCH(SelectEndOfDocument))     cursor->selectEndOfDocument();
    else if(MATCH(SelectAll))               cursor->selectAll();
    else if(MATCH(Deselect))                cursor->deselect();
    else if(allow_write && MATCH(Delete))                   cursor->del();
    else if(allow_write && MATCH(DeleteEndOfWord))          cursor->deleteEndOfWord();
    else if(allow_write && MATCH(DeleteStartOfWord))        cursor->deleteStartOfWord();
    else if(allow_write && MATCH(DeleteEndOfLine))          cursor->deleteEndOfLine();
    else if(allow_write && MATCH(DeleteCompleteLine))       cursor->deleteCompleteLine();
    else if(allow_write && (MATCH(Backspace) || e->key() == 16777219))
        cursor->backspace(); //Backspace detection is bugged
    else if(allow_write && MATCH(InsertLineSeparator))      cursor->insertLineSeparator();
    else if(allow_write && MATCH(InsertParagraphSeparator)) cursor->insertParagraphSeparator();
    else if(MATCH(Copy))                    cursor->copy();
    else if(allow_write && MATCH(Cut))      cursor->cut();
    else if(allow_write && MATCH(Paste))    cursor->paste();
    else if(MATCH(Find)) DO_THIS( "Find" )
    else if(MATCH(FindNext)) DO_THIS( "FindNext" )
    else if(MATCH(FindPrevious)) DO_THIS( "FindPrevious" )
    else if(allow_write && MATCH(Replace)) DO_THIS( "Replace" )
    else if(allow_write) processTextInput(e->text());

    #undef MATCH

    cv->update(*cursor);

    #ifdef YAWYSIWYGEE_TEST
    Text::verify();
    Construct::verify();
    Phrase::verify();
    qDebug() << "Verified";
    #endif
}

void Document::mousePressEvent(QGraphicsSceneMouseEvent* e){
    double_clicked = false;
    triple_clicked = false;

    if(e->buttons() == Qt::LeftButton){
        triple_clicked = testTripleClick(e->screenPos());
        click_location = e->screenPos();

        if(e->scenePos().x() < -linebox_offet) processLineSelection(e->scenePos().y());
        else if(triple_clicked) cursor->tripleClick();
        else if(e->modifiers() == Qt::ShiftModifier) processLeftShiftClick(e);
        else processLeftClick(e);
    }else if(e->buttons() == Qt::RightButton) contextClick(e);

    cv->update(*cursor);
}

void Document::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e){
    double_clicked = true;
    triple_clicked = false;

    if(e->buttons() != Qt::LeftButton) return;

    double_click_time = QTime::currentTime();
    processLeftClick(e);
    cursor->doubleClick();

    cv->update(*cursor);
}

void Document::mouseMoveEvent(QGraphicsSceneMouseEvent* e){
    if(e->buttons() != Qt::LeftButton) return;
    if( (click_location - e->screenPos()).manhattanLength() < 2 ) return;

    bool drag_on_selection = false;
    //this would require complicated logic to wait for release or drag
    //when a selection is clicked

    if(triple_clicked) cursor->lineSelectPoint(e->scenePos());
    else if(double_clicked) cursor->wordSelectPoint(e->scenePos());
    else if(drag_on_selection) DO_THIS( "selection drag: drag-and-drops a selection" )
    else processDrag(e);

    cv->update(*cursor);
}

bool Document::testTripleClick(QPointF click_location) const{
    //Qt does not support triple-click detection out of the box,
    //so this defines a workaround based on timing and travel

    QPointF deviation = click_location - click_location;
    qreal L1_norm = deviation.x() + deviation.y();

    return L1_norm < allowed_triple_click_travel &&
            double_click_time.msecsTo(QTime::currentTime()) < triple_click_period;
}

void Document::processTextInput(const QString& text){
    if(text.isEmpty()) return;

    const QChar& c = text.front();
    if(c.isPrint()) cursor->keystroke(c);
}

void Document::processLeftClick(QGraphicsSceneMouseEvent* e){
    QPointF p = e->scenePos();
    QGraphicsItem* item = itemAt(p, QTransform());
    if(item==nullptr) processClickMiss(p);
    else if(Text* t = dynamic_cast<Text*>(item)) cursor->clickText(*t, p.x());
    else if(Construct* c = dynamic_cast<Construct*>(item)) cursor->clickConstruct(*c);
    else if(SubPhrase* sp = dynamic_cast<SubPhrase*>(item)){
        if(sp->isEmpty()) cursor->clickText(*sp->front, p.x());
        else processClickMiss(p);
    }else{
        DO_THIS("Invalid items can be clicked")
        processClickMiss(p);
    }
}

void Document::processLeftShiftClick(QGraphicsSceneMouseEvent* e){
    cursor->selectClickPoint(e->scenePos());
}

void Document::contextClick(QGraphicsSceneMouseEvent* e){
    QMenu menu;

    bool clicked_on_selection = cursor->contains(e->scenePos());
    if(!clicked_on_selection){
        processRightClick(e, menu);
        cv->update(*cursor);
    }

    menu.addSeparator();
    QAction* undoAction = menu.addAction("Undo");
    QAction* redoAction = menu.addAction("Redo");
    connect(undoAction,SIGNAL(triggered()),undo_stack,SLOT(undo()));
    connect(redoAction,SIGNAL(triggered()),undo_stack,SLOT(redo()));
    undoAction->setEnabled(undo_stack->canUndo());
    redoAction->setEnabled(undo_stack->canRedo());

    menu.addSeparator();
    QAction* cutAction = menu.addAction("Cut");
    QAction* copyAction = menu.addAction("Copy");
    #ifndef __EMSCRIPTEN__
    QAction* copyImageAction = menu.addAction("Copy as PNG");
    #endif
    QAction* pasteAction = menu.addAction("Paste");
    connect(cutAction,SIGNAL(triggered()),this,SLOT(cutSelection()));
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copySelection()));
    #ifndef __EMSCRIPTEN__
    connect(copyImageAction,SIGNAL(triggered()),this,SLOT(copySelectionAsPng()));
    #endif
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));
    cutAction->setEnabled(clicked_on_selection);
    copyAction->setEnabled(clicked_on_selection);
    #ifndef __EMSCRIPTEN__
    copyImageAction->setEnabled(clicked_on_selection);
    #endif

    menu.addSeparator();
    QAction* selectAllAction = menu.addAction("Select All");
    connect(selectAllAction,SIGNAL(triggered()),this,SLOT(selectAll()));

    menu.exec(e->screenPos());
}

void Document::processRightClick(QGraphicsSceneMouseEvent* e, QMenu& menu){
    QPointF p = e->scenePos();
    QGraphicsItem* item = itemAt(p, QTransform());
    if(item==nullptr) processClickMiss(p);
    else if(Text* t = dynamic_cast<Text*>(item)){
        cursor->clickText(*t, p.x());
        t->populateMenu(menu);
    }else if(Construct* c = dynamic_cast<Construct*>(item)){
        cursor->clickConstruct(*c);
        c->populateMenu(menu);
    }else if(SubPhrase* sp = dynamic_cast<SubPhrase*>(item)){
        if(sp->isEmpty()){
            cursor->clickText(*sp->front, p.x());
            sp->front->populateMenu(menu);
        }
        else processClickMiss(p);
    }else{
        processClickMiss(p);
        DO_THIS("Invalid items can be clicked")
    }
}

void Document::processDrag(QGraphicsSceneMouseEvent* e){
    click_location = e->screenPos();
    cursor->selectClickPoint(e->scenePos());
}

void Document::processWordDrag(QGraphicsSceneMouseEvent* e){
    click_location = e->screenPos();
}

void Document::processLineSelection(qreal y){
    double_clicked = false;
    triple_clicked = true;

    cursor->tripleClick(Algorithm::lineAtY(*front, y));
}

void Document::processClickMiss(QPointF scene_pos){
    cursor->clickPoint(scene_pos);
}

void Document::cutSelection(){
    cursor->cut();
}

void Document::copySelection(){
    cursor->copy();
}

void Document::paste(){
    cursor->paste();
}

void Document::selectAll(){
    cursor->selectAll();
}

}

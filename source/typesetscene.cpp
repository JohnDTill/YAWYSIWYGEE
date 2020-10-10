#include "typesetscene.h"

#include "algorithm.h"
#include "construct.h"
#include "cursor.h"
#include "cursorview.h"
#include "globals.h"
#include "line.h"
#include "parser.h"
#include "subphrase.h"
#include "text.h"
#include <QClipboard>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QTextStream>
#include <QTime>
#include <QUndoStack>

static QTime double_click_time;

TypesetScene::TypesetScene(bool allow_write, bool show_line_numbers, Line* f, Line* b)
    : front(f),
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

TypesetScene::~TypesetScene(){
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

    #ifdef YAWYSIWYGEE_TEST
    Q_ASSERT(Text::allFreed());
    Q_ASSERT(Construct::allFreed());
    Q_ASSERT(Phrase::allFreed());
    #endif
}

void TypesetScene::setLineNumbersVisible(bool show){
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

void TypesetScene::write(QTextStream& out) const{
    for(Line* l = front; l; l = l->next) l->write(out);
}

void TypesetScene::updateCursorView(){
    cv->update(*cursor);
}

void TypesetScene::updateSize(){
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

void TypesetScene::copyAsPng(qreal upscale){
    QImage image(upscale*sceneRect().size().toSize(), QImage::Format_RGB16);
    image.fill(palette().base().color());

    QPainter painter(&image);
    render(&painter);
    QGuiApplication::clipboard()->setImage(image, QClipboard::Clipboard);
}

void TypesetScene::copySelectionAsPng(qreal upscale){
    if(!cursor->hasSelection()) return;

    cursor->copy();

    TypesetScene doc(true, false);
    doc.paste();
    doc.copyAsPng(upscale);
}

void TypesetScene::drawBackground(QPainter* painter, const QRectF& rect){
    QGraphicsScene::drawBackground(painter, rect);
    if(show_line_nums){
        painter->setBrush(palette().window());
        painter->setPen(palette().midlight().color());
        painter->drawRect(QRectF(-1-linebox_width-linebox_offet, -margin_top-1, linebox_width+1, 1e8));
    }
    painter->setPen(palette().text().color());
}

#include <QDebug>
void TypesetScene::keyPressEvent(QKeyEvent* e){
    constexpr int Ctrl = Qt::ControlModifier;
    constexpr int Shift = Qt::ShiftModifier;
    constexpr int CtrlShift = Qt::ControlModifier + Qt::ShiftModifier;

    switch (e->key() | e->modifiers()) {
        case Qt::Key_Z|Ctrl: if(allow_write) undo_stack->undo(); break;
        case Qt::Key_Y|Ctrl: if(allow_write) undo_stack->redo(); break;
        case Qt::Key_Right: cursor->moveToNextChar(); break;
        case Qt::Key_Left: cursor->moveToPreviousChar(); break;
        case Qt::Key_Right|Ctrl: cursor->moveToNextWord(); break;
        case Qt::Key_Left|Ctrl: cursor->moveToPreviousWord(); break;
        case Qt::Key_Down: cursor->moveToNextLine(); break;
        case Qt::Key_Up: cursor->moveToPreviousLine(); break;
        case Qt::Key_Home: cursor->moveToStartOfLine(); break;
        case Qt::Key_End: cursor->moveToEndOfLine(); break;
        case Qt::Key_Home|Ctrl: cursor->moveToStartOfDocument(); break;
        case Qt::Key_End|Ctrl: cursor->moveToEndOfDocument(); break;
        case Qt::Key_Right|Shift: cursor->selectNextChar(); break;
        case Qt::Key_Left|Shift: cursor->selectPreviousChar(); break;
        case Qt::Key_Right|CtrlShift: cursor->selectNextWord(); break;
        case Qt::Key_Left|CtrlShift: cursor->selectPreviousWord(); break;
        case Qt::Key_Down|Shift: cursor->selectNextLine(); break;
        case Qt::Key_Up|Shift: cursor->selectPreviousLine(); break;
        case Qt::Key_Home|Shift: cursor->selectStartOfLine(); break;
        case Qt::Key_End|Shift: cursor->selectEndOfLine(); break;
        case Qt::Key_Home|CtrlShift: cursor->selectStartOfDocument(); break;
        case Qt::Key_End|CtrlShift: cursor->selectEndOfDocument(); break;
        case Qt::Key_A|Ctrl: selectAll(); break;
        case Qt::Key_Delete: if(allow_write) cursor->del(); break;
        case Qt::Key_Delete|Ctrl: if(allow_write) cursor->deleteEndOfWord(); break;
        case Qt::Key_Backspace|Ctrl: if(allow_write) cursor->deleteStartOfWord(); break;
        case Qt::Key_Backspace: if(allow_write) cursor->backspace(); break;
        case Qt::Key_Return: if(allow_write) cursor->insertParagraphSeparator(); break;
        case Qt::Key_Return|Shift: if(allow_write) cursor->insertLineSeparator(); break;
        case Qt::Key_Tab: if(allow_write) cursor->tab(); break;
        case (Qt::Key_Tab|Shift)+1: if(allow_write) cursor->shiftTab(); break;
        case Qt::Key_Equal|Ctrl: if(allow_write) cursor->alignAtEquals(); break;
        case Qt::Key_C|Ctrl: cursor->copy(); break;
            #ifndef __EMSCRIPTEN__
        case Qt::Key_X|Ctrl: if(allow_write) cutSelection(); break;
            #endif
        case Qt::Key_V|Ctrl: if(allow_write) cursor->paste(); break;
        default:
            if(e->modifiers().testFlag(Qt::ControlModifier)) return;
            QString text = e->text();
            if(allow_write && !text.isEmpty() && text.front().isPrint()) cursor->keystroke(text.front());
    }

    cv->update(*cursor);

    #ifdef YAWYSIWYGEE_TEST
    Q_ASSERT(Text::verify());
    Q_ASSERT(Construct::verify());
    Q_ASSERT(Phrase::verify());
    #endif
}

void TypesetScene::mousePressEvent(QGraphicsSceneMouseEvent* e){
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

void TypesetScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e){
    double_clicked = true;
    triple_clicked = false;

    if(e->buttons() != Qt::LeftButton) return;

    double_click_time = QTime::currentTime();
    processLeftClick(e);
    cursor->doubleClick();

    cv->update(*cursor);
}

void TypesetScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e){
    if(e->buttons() != Qt::LeftButton) return;
    if( (click_location - e->screenPos()).manhattanLength() < 2 ) return;

    //No selection dragging - I don't know if users appreciate that feature for text editing

    if(triple_clicked) cursor->lineSelectPoint(e->scenePos());
    else if(double_clicked) cursor->wordSelectPoint(e->scenePos());
    else processDrag(e);

    cv->update(*cursor);
}

bool TypesetScene::testTripleClick(QPointF click_location) const{
    //Qt does not support triple-click detection out of the box,
    //so this defines a workaround based on timing and travel

    QPointF deviation = click_location - click_location;
    qreal L1_norm = deviation.x() + deviation.y();

    return L1_norm < allowed_triple_click_travel &&
            double_click_time.msecsTo(QTime::currentTime()) < triple_click_period;
}

void TypesetScene::processLeftClick(QGraphicsSceneMouseEvent* e){
    QPointF p = e->scenePos();
    QGraphicsItem* item = itemAt(p, QTransform());
    if(item==nullptr) processClickMiss(p);
    else if(Text* t = dynamic_cast<Text*>(item)) cursor->clickText(*t, p.x());
    else if(Construct* c = dynamic_cast<Construct*>(item)) cursor->clickConstruct(*c);
    else if(SubPhrase* sp = dynamic_cast<SubPhrase*>(item)){
        if(sp->isEmpty()) cursor->clickText(*sp->front, p.x());
        else processClickMiss(p);
    }else{
        qDebug() << "Invalid item left clicked";
        processClickMiss(p);
    }
}

void TypesetScene::processLeftShiftClick(QGraphicsSceneMouseEvent* e){
    cursor->selectClickPoint(e->scenePos());
}

void TypesetScene::contextClick(QGraphicsSceneMouseEvent* e){
    QMenu menu;

    bool clicked_on_selection = cursor->contains(e->scenePos());
    if(clicked_on_selection){
        if(allow_write) cursor->populateContextMenu(menu);
    }else{
        processRightClick(e, menu);
        cv->update(*cursor);
    }

    menu.addSeparator();

    if(allow_write){
        QAction* undoAction = menu.addAction("Undo");
        QAction* redoAction = menu.addAction("Redo");
        connect(undoAction,SIGNAL(triggered()),undo_stack,SLOT(undo()));
        connect(redoAction,SIGNAL(triggered()),undo_stack,SLOT(redo()));
        undoAction->setEnabled(undo_stack->canUndo());
        redoAction->setEnabled(undo_stack->canRedo());

        #ifndef __EMSCRIPTEN__
        menu.addSeparator();
        QAction* cutAction = menu.addAction("Cut");
        connect(cutAction,SIGNAL(triggered()),this,SLOT(cutSelection()));
        cutAction->setEnabled(clicked_on_selection);
        #endif
    }
    #ifndef __EMSCRIPTEN__
    QAction* copyAction = menu.addAction("Copy");
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copySelection()));
    copyAction->setEnabled(clicked_on_selection);
    QAction* copyImageAction = menu.addAction("Copy as PNG");
    connect(copyImageAction,SIGNAL(triggered()),this,SLOT(copySelectionAsPng()));
    copyImageAction->setEnabled(clicked_on_selection);
    if(allow_write){
        QAction* pasteAction = menu.addAction("Paste");
        connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));
    }
    #endif

    menu.addSeparator();
    QAction* selectAllAction = menu.addAction("Select All");
    connect(selectAllAction,SIGNAL(triggered()),this,SLOT(selectAll()));

    menu.exec(e->screenPos());
}

void TypesetScene::processRightClick(QGraphicsSceneMouseEvent* e, QMenu& menu){
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
        qDebug() << "Invalid item right clicked";
        processClickMiss(p);
    }
}

void TypesetScene::processDrag(QGraphicsSceneMouseEvent* e){
    click_location = e->screenPos();
    cursor->selectClickPoint(e->scenePos());
}

void TypesetScene::processWordDrag(QGraphicsSceneMouseEvent* e){
    click_location = e->screenPos();
}

void TypesetScene::processLineSelection(qreal y){
    double_clicked = false;
    triple_clicked = true;

    cursor->tripleClick(Algorithm::lineAtY(*front, y));
}

void TypesetScene::processClickMiss(QPointF scene_pos){
    cursor->clickPoint(scene_pos);
}

void TypesetScene::cutSelection(){
    cursor->cut();
    updateCursorView();
}

void TypesetScene::copySelection(){
    cursor->copy();
}

void TypesetScene::paste(){
    cursor->paste();
    updateCursorView();
}

void TypesetScene::selectAll(){
    cursor->selectAll();
    updateCursorView();
}

void TypesetScene::alignAtEquals(){
    cursor->alignAtEquals();
    updateCursorView();
}

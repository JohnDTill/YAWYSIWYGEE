#include "typesetedit.h"

#include "construct.h"
#include "cursor.h"
#include "cursorview.h"
#include "errorview.h"
#include "globals.h"
#include "line.h"
#include "parser.h"
#include "subphrase.h"
#include "text.h"
#include "typesetscene.h"
#include "MathBran/include/QMathBran.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMessageBox>
#include <QScrollBar>
#include <QSvgGenerator>
#include <QTextStream>
#include <QUndoStack>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QtMath>

class TypesetEdit::TypesetView : public QGraphicsView{
private:
    TypesetEdit* edit;

public:
    TypesetView(TypesetEdit* edit);
    void keyPressEventQGraphicsView(QKeyEvent* e);

protected:
    virtual void keyPressEvent(QKeyEvent* e) override final;
    virtual void mousePressEvent(QMouseEvent* e) override final;
    virtual void mouseMoveEvent(QMouseEvent* e) override final;
    virtual void wheelEvent(QWheelEvent* event) override final;
};

TypesetEdit::TypesetEdit(QWidget* parent)
    : QWidget(parent) {
    Globals::initGlobals();

    view = new TypesetView(this);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    view->verticalScrollBar()->setCursor(Qt::CursorShape::ArrowCursor);
    view->horizontalScrollBar()->setCursor(Qt::CursorShape::ArrowCursor);

    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    zoomReset();

    scene = new TypesetScene(!read_only, show_line_numbers);
    setScene(scene);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(view);
    view->setScene(scene);
}

TypesetEdit::~TypesetEdit(){
    delete scene;
}

QString TypesetEdit::documentTitle() const{
    return doc_title;
}

QFont TypesetEdit::font() const{
    return Globals::fonts[0];
}

bool TypesetEdit::isReadOnly() const{
    return read_only;
}

void TypesetEdit::printSvg(QSvgGenerator* svg_generator, bool draw_background) const{
    if(!svg_generator) return;

    QRectF bounds = scene->sceneRect();
    int x = qCeil(bounds.x());
    int y = qCeil(bounds.y());
    int w = qCeil( bounds.width() + (draw_background ? 10 : 0) );
    int h = qCeil(bounds.height());
    QRect box(x, y, w, h);

    svg_generator->setSize(QSize(w, h));
    svg_generator->setViewBox(box);
    svg_generator->setDescription( toMathBran().replace('<', "⁜≮").replace('>', "⁜≯") );

    QPainter painter(svg_generator);
    if(draw_background){
        painter.setBrush(palette().base());
        painter.setPen(QPen(palette().base().color()));
        painter.drawRect(box);
    }
    scene->render(&painter, bounds);
}

QString TypesetEdit::toMathBran() const{
    QString str;
    QTextStream out(&str);
    out.setCodec("UTF-8");

    scene->write(out);

    return str;
}

QString TypesetEdit::selectedMathBran() const{
    return scene->cursor->selectedCode();
}

void TypesetEdit::setDocumentTitle(const QString& title){
    doc_title = title;
}

void TypesetEdit::setPalette(const QPalette& palette){
    QWidget::setPalette(palette);
    view->setPalette(palette);
    scene->setPalette(palette);
}

void TypesetEdit::setReadOnly(bool ro){
    read_only = ro;
    scene->allow_write = !ro;
}

void TypesetEdit::showLineNumbers(bool show){
    show_line_numbers = show;
    scene->setLineNumbersVisible(show);
}

std::vector<Text*> TypesetEdit::getTextPointers() const{
    return scene->getTextPointers();
}

void TypesetEdit::reportError(Text* tL, int pL, Text* tR, int pR, const QString& msg){
    scene->ev->reportError(tL, pL, tR, pR, msg);
}

void TypesetEdit::reportError(const QString& mb, int start, int length, const QString& msg){
    Q_ASSERT(mb == toMathBran());
    Q_ASSERT(start >= 0);
    Q_ASSERT(length >= 0);
    Q_ASSERT(start+length <= mb.length());

    int lL_num = 0;
    int lL_start = 0;
    for(int i = 0; i < start; i++){
        if(mb[i] == '\n'){
            lL_num++;
            lL_start = i+1;
        }
    }

    int l_span = 0;
    int lR_start = lL_start;
    for(int i = start; i < start+length; i++){
        if(mb[i] == '\n'){
            l_span++;
            lR_start = i+1;
        }
    }
    if(start == mb.size()-1) l_span--;

    if(l_span) reportErrorMultiline(mb, lL_num, l_span, lL_start, lR_start, start, length, msg);
    else reportErrorSameLine(mb, lL_num, lL_start, start, length, msg);
}

void TypesetEdit::clearErrors(){
    scene->ev->clear();
}

//SLOTS
void TypesetEdit::clear(){
    delete scene;
    scene = new TypesetScene(!read_only, show_line_numbers);
    scene->setPalette(palette());
    setScene(scene);
    emit contentsChanged();
}

void TypesetEdit::copy(){
    scene->copySelection();
}

void TypesetEdit::copyPng(qreal upscale){
    scene->copySelectionAsPng(upscale);
}

void TypesetEdit::cut(){
    scene->cutSelection();
}

void TypesetEdit::insertMathBran(const QString& text){
    scene->cursor->insert(text);
    scene->cv->update(*scene->cursor);
}

void TypesetEdit::insertPlainText(const QString& text){
    scene->cursor->insert(text);
    scene->cv->update(*scene->cursor);
}

void TypesetEdit::paste(){
    scene->paste();
}

void TypesetEdit::redo(){
    if(!scene->undo_stack->canRedo()) return;
    scene->undo_stack->redo();
    emit contentsChanged();
}

void TypesetEdit::setMathBran(const QString& text){
    if(!MathBran::isWellFormed(text)){
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "TypesetEdit::setMathBran - Invalid MathBran source.");
        messageBox.setFixedSize(500, 200);
        return;
    }

    delete scene;
    scene = Parser::parseDocument(text, !read_only, show_line_numbers);
    setScene(scene);

    //Make sure the view gets the focus item signal
    QGraphicsItem* focus_item = scene->focusItem();
    scene->clearFocus();
    if(focus_item) focus_item->setFocus();
}

void TypesetEdit::undo(){
    if(!scene->undo_stack->canUndo()) return;
    scene->undo_stack->undo();
    emit contentsChanged();
}

static constexpr qreal default_scale = 2;

void TypesetEdit::zoomIn(qreal scale_factor){
    constexpr qreal max_scale = 15;
    qreal candidate = view->transform().m11() * scale_factor;
    qreal proximity = candidate - default_scale;

    if(proximity < 1e-9 && proximity > -1e-9){
        zoomReset(); //Correct drift
    }else if(candidate > max_scale){
        qreal scale_to_max = max_scale / view->transform().m11();
        view->scale(scale_to_max, scale_to_max);
    }else{
        view->scale(scale_factor, scale_factor);
    }
}

void TypesetEdit::zoomOut(qreal scale_factor){
    constexpr qreal min_scale = 0.5;
    qreal candidate = view->transform().m11() * scale_factor;
    qreal proximity = candidate - default_scale;

    if(proximity < 1e-9 && proximity > -1e-9){
        zoomReset(); //Correct drift
    }else if(candidate < min_scale){
        qreal scale_to_min = min_scale / view->transform().m11();
        view->scale(scale_to_min, scale_to_min);
    }else{
        view->scale(scale_factor, scale_factor);
    }
}

void TypesetEdit::zoomReset(){
    view->resetTransform();
    view->scale(default_scale, default_scale);
}

//Protected functions
void TypesetEdit::keyPressEvent(QKeyEvent* e){
    switch (e->key() + e->modifiers()) {
        case Qt::Key_Plus+Qt::ControlModifier: zoomIn(); break;
        case Qt::Key_Minus+Qt::ControlModifier: zoomOut(); break;
        case Qt::Key_PageDown: scene->cursor->moveToNextPage(heightInScene()); break;
        case Qt::Key_PageUp: scene->cursor->moveToPreviousPage(heightInScene()); break;
        case Qt::Key_PageDown+Qt::ShiftModifier: scene->cursor->selectNextPage(heightInScene()); break;
        case Qt::Key_PageUp+Qt::ShiftModifier: scene->cursor->selectPreviousPage(heightInScene()); break;
    }

    view->keyPressEventQGraphicsView(e);
}

void TypesetEdit::mouseMoveEvent(QMouseEvent* e){
    qreal x = view->mapToScene(e->pos()).x();
    if(x > -linebox_offet) setCursor(Qt::IBeamCursor);
    else setCursor(Qt::ArrowCursor);
}

void TypesetEdit::wheelEvent(QWheelEvent* e){
    if( e->modifiers() == Qt::ControlModifier ){
        if(e->angleDelta().y() > 0) zoomIn();
        else zoomOut();
    }else{
        view->verticalScrollBar()->event(e);
    }
}

//Private functions
void TypesetEdit::setScene(TypesetScene* scene){
    scene->setPalette(palette());
    view->setScene(scene);

    if(scene){
        connect(scene->undo_stack, SIGNAL(canUndoChanged(bool)), this, SLOT(passUndo(bool)));
        connect(scene->undo_stack, SIGNAL(canRedoChanged(bool)), this, SLOT(passRedo(bool)));
        connect(scene->undo_stack, SIGNAL(indexChanged(int)), this, SLOT(passContentsChanged()));
    }
}

qreal TypesetEdit::heightInScene() const {
    return view->mapToScene(0, view->height()).y() - view->mapToScene(0,0).y();
}

void TypesetEdit::reportErrorSameLine(const QString& mb,
                                      int l_num,
                                      int l_start,
                                      int start,
                                      int length,
                                      const QString& msg){
    Line* l = scene->front;
    for(int i = 0; i < l_num; i++) l = l->next;

    Phrase* p = l;
    Text* tL = l->front;
    int tL_start = l_start;
    for(int i = l_start; i < start; i++){
        if(mb[i] == MB_CONSTRUCT_SYMBOL){
            i++;
            if(mb[i] == MB_CONSTRUCT_SYMBOL || mb[i] == MB_OPEN || mb[i] == MB_CLOSE) continue;
            else if(mb[i] == MB_MATRIX){
                while(mb[i++] != MB_CLOSE);
                while(mb[i++] != MB_CLOSE);
                i--;
            }

            if(tL->next->front()){
                //Move down
                p = tL->next->front();
                tL = p->front;
                tL_start = ++i;
            }else{
                //Move across phrase
                tL = tL->next->next;
                tL_start = i;
            }
        }else if(mb[i] == MB_CLOSE){
            //Move across construct or up
            if(i+1 < mb.size() && mb[i+1] == MB_OPEN) i++;
            Q_ASSERT(p->type() == SUBPHRASE);
            SubPhrase* sp = static_cast<SubPhrase*>(p);
            tL = sp->parent->textRight(sp);
            tL_start = i+1;
            p = tL->parent;
        }
    }


    Text* tR = tL;
    int tR_start = tL_start;
    for(int i = tL_start; i < start+length; i++){
        if(mb[i] == MB_CONSTRUCT_SYMBOL){
            i++;
            if(mb[i] == MB_CONSTRUCT_SYMBOL || mb[i] == MB_OPEN || mb[i] == MB_CLOSE) continue;
            i += 2;
            int level = 1;
            while(level || mb[i+1] == MB_OPEN){
                if(mb[i] == MB_CLOSE) level--;
                else if(mb[i] == MB_OPEN) level++;

                i++;
            }

            tR = tR->next->next;
            tR_start = i+1;
        }
    }

    reportError(tL, start-tL_start, tR, start+length-tR_start, msg);
}

void TypesetEdit::reportErrorMultiline(const QString& mb,
                                       int lL_num,
                                       int l_span,
                                       int lL_start,
                                       int lR_start,
                                       int start,
                                       int length,
                                       const QString& msg){
    Line* lL = scene->front;
    for(int i = 0; i < lL_num; i++) lL = lL->next;
    Line* lR = lL;
    for(int i = 0; i < l_span; i++) lR = lR->next;

    //Using the given information, find the appropriate text in the tree
    Text* tL = lL->front;
    int tL_start = lL_start;
    for(int i = lL_start; i < start; i++){
        if(mb[i] == MB_CONSTRUCT_SYMBOL){
            i++;
            if(mb[i] == MB_CONSTRUCT_SYMBOL || mb[i] == MB_OPEN || mb[i] == MB_CLOSE) continue;
            i += 2;
            int level = 1;
            while(level || mb[i+1] == MB_OPEN){
                if(mb[i] == MB_CLOSE) level--;
                else if(mb[i] == MB_OPEN) level++;

                i++;
            }

            tL = tL->next->next;
            tL_start = i+1;
        }
    }

    Text* tR = lR->front;
    int tR_start = lR_start;
    for(int i = lR_start; i < start; i++){
        if(mb[i] == MB_CONSTRUCT_SYMBOL){
            i++;
            if(mb[i] == MB_CONSTRUCT_SYMBOL || mb[i] == MB_OPEN || mb[i] == MB_CLOSE) continue;
            i += 2;
            int level = 1;
            while(level || mb[i+1] == MB_OPEN){
                if(mb[i] == MB_CLOSE) level--;
                else if(mb[i] == MB_OPEN) level++;

                i++;
            }

            tR = tR->next->next;
            tR_start = i+1;
        }
    }

    reportError(tL, start-tL_start, tR, start+length-tR_start, msg);
}

//Private slots
void TypesetEdit::passUndo(bool available){
    emit undoAvailable(available);
}

void TypesetEdit::passRedo(bool available){
    emit redoAvailable(available);
}

void TypesetEdit::passContentsChanged(){
    emit contentsChanged();
}

TypesetEdit::TypesetView::TypesetView(TypesetEdit* edit)
    : QGraphicsView(edit), edit(edit) {}

void TypesetEdit::TypesetView::keyPressEventQGraphicsView(QKeyEvent* e){
    QGraphicsView::keyPressEvent(e);
}

void TypesetEdit::TypesetView::keyPressEvent(QKeyEvent* e){
    edit->keyPressEvent(e);

    #ifdef YAWYSIWYGEE_LOGGING
    qDebug() << "keyPressEvent(new QKeyEvent(QEvent::KeyPress,"
             << e->key() << " ," << e->modifiers() << "));";
    #endif

    QLineF line = edit->scene->getCursorLine();
    ensureVisible(line.x1(), line.y1(), 1, line.dy(), 30, 15);
}

void TypesetEdit::TypesetView::mousePressEvent(QMouseEvent* e){
    int cached_scrollbar_x = horizontalScrollBar()->value();
    int cached_scrollbar_y = verticalScrollBar()->value();
    QGraphicsView::mousePressEvent(e);
    horizontalScrollBar()->setValue(cached_scrollbar_x);
    horizontalScrollBar()->update();
    verticalScrollBar()->setValue(cached_scrollbar_y);
    verticalScrollBar()->update();
    QLineF line = edit->scene->getCursorLine();
    ensureVisible(line.x1(), line.y1(), 1, line.dy(), 30, 15);
}

void TypesetEdit::TypesetView::mouseMoveEvent(QMouseEvent* e){
    edit->mouseMoveEvent(e);
    QGraphicsView::mouseMoveEvent(e);

    //#ifdef YAWYSIWYGEE_LOGGING
    //qDebug() << "mouseMoveEvent(new QMouseEvent(QEvent::MouseMove,"
    //         << e->pos() << " ,"
    //         << e->button() << " ,"
    //         << e->buttons() << " ,"
    //         << e->modifiers() << "));";
    //#endif
}

void TypesetEdit::TypesetView::wheelEvent(QWheelEvent* event){
    edit->wheelEvent(event);
}

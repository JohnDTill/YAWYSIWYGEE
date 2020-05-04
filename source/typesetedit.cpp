#include "typesetedit.h"

#include "typesetscene.h"
#include "parser.h"
#include "MathBran/include/QMathBran.h"
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QtMath>

TypesetEdit::TypesetEdit(QWidget* parent)
    : QWidget(parent) {
    Globals::initGlobals();

    setMouseTracking(true);
    view = new QGraphicsView(this);

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
}

void TypesetEdit::showLineNumbers(bool show){
    show_line_numbers = show;
    scene->setLineNumbersVisible(show);
}

//SLOTS
void TypesetEdit::clear(){
    delete scene;
    scene = new TypesetScene(!read_only, show_line_numbers);
    scene->setPalette(palette());
    setScene(scene);
}

void TypesetEdit::copy(){
    scene->cursor->copy();
}

void TypesetEdit::copyPng(qreal upscale){
    scene->copySelectionAsPng(upscale);
}

void TypesetEdit::cut(){
    scene->cursor->cut();
    scene->updateCursorView();
}

void TypesetEdit::insertMathBran(const QString& text){
    scene->cursor->paste(text);
    scene->cv->update(*scene->cursor);
}

void TypesetEdit::insertPlainText(const QString& text){
    scene->cursor->paste(text);
    scene->cv->update(*scene->cursor);
}

void TypesetEdit::paste(){
    scene->cursor->paste();
    scene->updateCursorView();
}

void TypesetEdit::redo(){
    scene->undo_stack->redo();
}

void TypesetEdit::setMathBran(const QString& text){
    if(!MathBran::isWellFormed(text)){
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "TypesetEdit::setMathBran - Invalid MathBran source.");
        messageBox.setFixedSize(500, 200);
        return;
    }

    delete scene;
    scene = Parser::parseDocument(text, scene->allow_write, scene->show_line_nums);
    setScene(scene);

    //Make sure the view gets the focus item signal
    QGraphicsItem* focus_item = scene->focusItem();
    scene->clearFocus();
    if(focus_item) focus_item->setFocus();
}

void TypesetEdit::undo(){
    scene->undo_stack->undo();
}

void TypesetEdit::zoomIn(qreal scale_factor){
    constexpr qreal max_scale = 15;
    qreal candidate = view->transform().m11() * scale_factor;

    if(candidate > max_scale){
        qreal scale_to_max = max_scale / view->transform().m11();
        view->scale(scale_to_max, scale_to_max);
    }else{
        view->scale(scale_factor, scale_factor);
    }
}

void TypesetEdit::zoomOut(qreal scale_factor){
    constexpr qreal min_scale = 0.5;
    qreal candidate = view->transform().m11() * scale_factor;

    if(candidate < min_scale){
        qreal scale_to_min = min_scale / view->transform().m11();
        view->scale(scale_to_min, scale_to_min);
    }else{
        view->scale(scale_factor, scale_factor);
    }
}

void TypesetEdit::zoomReset(){
    view->resetTransform();
    view->scale(2, 2);
}

//Protected functions
void TypesetEdit::keyPressEvent(QKeyEvent* e){
    #define MATCH(arg) e->matches(QKeySequence::arg)

    if     (MATCH(ZoomIn))  zoomIn();
    else if(MATCH(ZoomOut)) zoomOut();
    else if(MATCH(MoveToNextPage))     scene->cursor->moveToNextPage( heightInSceneCoordinates() );
    else if(MATCH(MoveToPreviousPage)) scene->cursor->moveToPreviousPage( heightInSceneCoordinates() );
    else if(MATCH(SelectNextPage))     scene->cursor->selectNextPage( heightInSceneCoordinates() );
    else if(MATCH(SelectPreviousPage)) scene->cursor->selectPreviousPage( heightInSceneCoordinates() );
    else if(MATCH(Refresh)) DO_THIS( "Refresh" )
    else if(MATCH(Close)) DO_THIS( "Close" )
    else QWidget::keyPressEvent(e);

    scene->updateCursorView();

    #undef MATCH
}

void TypesetEdit::wheelEvent(QWheelEvent* e){
    if( e->modifiers() == Qt::ControlModifier ){
        if(e->delta() > 0) zoomIn();
        else zoomOut();
    }else{
        view->verticalScrollBar()->event(e);
    }
}

//Private functions
void TypesetEdit::setScene(TypesetScene* scene){
    if(view->scene()){
        disconnect(view->scene(), SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),
                   this, SLOT(ensureFocusedItemVisible(QGraphicsItem*)));
    }

    scene->setPalette(palette());
    view->setScene(scene);

    if(scene){
        connect(scene, SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),
                this, SLOT(ensureFocusedItemVisible(QGraphicsItem*)));

        connect(scene->undo_stack, SIGNAL(canUndoChanged(bool)), this, SLOT(passUndo(bool)));
        connect(scene->undo_stack, SIGNAL(canRedoChanged(bool)), this, SLOT(passRedo(bool)));
    }
}

qreal TypesetEdit::heightInSceneCoordinates() const {
    return view->mapToScene(0, height()).y() - view->mapToScene(0,0).y();
}

//Private slots
void TypesetEdit::ensureFocusedItemVisible(QGraphicsItem* newFocusItem){
    if(newFocusItem) view->ensureVisible(newFocusItem, 30, 15);
}

void TypesetEdit::passUndo(bool available){
    emit undoAvailable(available);
}

void TypesetEdit::passRedo(bool available){
    emit redoAvailable(available);
}

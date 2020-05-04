#include "bigqchar.h"

#include "../algorithm.h"
#include "../cursor.h"
#include "../globals.h"
#include "../subphrase.h"
#include "../typesetscene.h"
#include <QMenu>
#include <QPainter>

BigQChar::BigQChar(QChar qchar, bool allow_overscript)
    : ch(qchar),
      allow_overscript(allow_overscript) {
    updateLayout();
}

void BigQChar::updateLayout(){
    QRectF child_bounds = Globals::bigqchar_font_metrics.boundingRect(ch);
    w = child_bounds.width();
    u = d = child_bounds.height()/2;
}

void BigQChar::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    QAction* addUnderscript = menu.addAction(QString("Big ") + ch + ": Add underscript");
    connect(addUnderscript, SIGNAL(triggered()), this, SLOT(addUnderscript()));
}

void BigQChar::write(QTextStream& out) const{
    out << MB_CONSTRUCT_SYMBOL << ch;
}

void BigQChar::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->setFont(Globals::bigqchar_font);
    painter->drawText(boundingRect(), Qt::AlignBottom|Qt::AlignHCenter, ch);
}

void BigQChar::addUnderscript(){
    typesetDocument()->undo_stack->push( new AddUnderscript(this) );
}

BigQChar::AddUnderscript::AddUnderscript(BigQChar* out)
    : out(out) {
    Text* t = new Text(out->prev->getScriptLevel() + 1);
    t->next = t->prev = nullptr;

    in = new BigQChar_S(out->ch, new SubPhrase(t), out->allow_overscript);
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

BigQChar::AddUnderscript::~AddUnderscript(){
    if(active) out->deletePostorder();
    else in->deletePostorder();
}

void BigQChar::AddUnderscript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->prev->parent->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void BigQChar::AddUnderscript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->prev->parent->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

#define underscript child
BigQChar_S::BigQChar_S(QChar qchar, SubPhrase* c, bool allow_overscript)
    : UnaryConstruct(c),
      ch(qchar),
      allow_overscript(allow_overscript) {
    updateLayout();
}

void BigQChar_S::updateLayout(){
    QRectF symbol_bounds = Globals::bigqchar_font_metrics.boundingRect(ch);
    w = qMax(symbol_bounds.width(), underscript->w);
    u = symbol_bounds.height()/2;
    d = u + underscript->u + underscript->d;

    symbol_x = (w-symbol_bounds.width())/2;
    underscript->setPos( (w-underscript->w)/2, symbol_bounds.height() );
    update();
}

void BigQChar_S::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    if(allow_overscript){
        QAction* addOverscript = menu.addAction(QString("Big ") + ch + ": Add overscript");
        connect(addOverscript, SIGNAL(triggered()), this, SLOT(addOverscript()));
    }
    QAction* removeUnderscript = menu.addAction(QString("Big ") + ch + ": Remove underscript");
    connect(removeUnderscript, SIGNAL(triggered()), this, SLOT(removeUnderscript()));
}

void BigQChar_S::write(QTextStream& out) const{
    out << MB_CONSTRUCT_SYMBOL << ch;
    child->write(out);
}

void BigQChar_S::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->setFont(Globals::bigqchar_font);
    QRectF symbol_rect = Globals::bigqchar_font_metrics.boundingRect(ch);
    symbol_rect.moveTo(symbol_x, 0);
    painter->drawText(symbol_rect, Qt::AlignBottom|Qt::AlignHCenter, ch);
}

void BigQChar_S::addOverscript(){
    typesetDocument()->undo_stack->push( new AddOverscript(this) );
}

void BigQChar_S::removeUnderscript(){
    typesetDocument()->undo_stack->push( new RemoveUnderscript(this) );
}

BigQChar_S::AddOverscript::AddOverscript(BigQChar_S* out)
    : out(out) {
    Text* t = new Text(out->prev->getScriptLevel() + 1);
    t->next = t->prev = nullptr;

    in = new BigQChar_SN(out->ch, out->child, new SubPhrase(t));
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

BigQChar_S::AddOverscript::~AddOverscript(){
    if(active) delete out;
    else{
        in->first->deletePostorder();
        delete in;
    }
}

void BigQChar_S::AddOverscript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->second->setParentConstruct(*in);
    in->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void BigQChar_S::AddOverscript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->child->setParentConstruct(*out);
    out->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

BigQChar_S::RemoveUnderscript::RemoveUnderscript(BigQChar_S* out)
    : out(out) {
    in = new BigQChar(out->ch, out->allow_overscript);
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

BigQChar_S::RemoveUnderscript::~RemoveUnderscript(){
    if(active) out->deletePostorder();
    else in->deletePostorder();
}

void BigQChar_S::RemoveUnderscript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->prev->parent->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void BigQChar_S::RemoveUnderscript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->prev->parent->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}
#undef underscript

#define overscript first
#define underscript second
BigQChar_SN::BigQChar_SN(QChar qchar, SubPhrase* s, SubPhrase* f)
    : BinaryConstruct(f, s),
      ch(qchar) {
    updateLayout();
}

void BigQChar_SN::updateLayout(){
    QRectF symbol_bounds = Globals::bigqchar_font_metrics.boundingRect(ch);
    w = qMax(symbol_bounds.width(), qMax(underscript->w, overscript->w));
    u = symbol_bounds.height()/2 + overscript->u + overscript->d;
    d = symbol_bounds.height()/2 + underscript->u + underscript->d;

    overscript->setPos( (w-overscript->w)/2, 0 );
    symbol_x = (w-symbol_bounds.width())/2;
    symbol_y = overscript->u + overscript->d;
    qreal y = symbol_y + symbol_bounds.height();
    underscript->setPos( (w-underscript->w)/2, y );
    update();
}

void BigQChar_SN::write(QTextStream& out) const{
    out << MB_CONSTRUCT_SYMBOL << ch;
    second->write(out);
    first->write(out);
}

Text* BigQChar_SN::textUp(const SubPhrase* caller, qreal x) const{
    return (caller==second) ? Algorithm::textAtSetpoint(*first, x) : prev;
}

Text* BigQChar_SN::textDown(const SubPhrase* caller, qreal x) const{
    return (caller==first) ? Algorithm::textAtSetpoint(*second, x) : next;
}

void BigQChar_SN::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    QAction* removeOverscript = menu.addAction(QString("Big ") + ch + ": Remove overscript");
    connect(removeOverscript, SIGNAL(triggered()), this, SLOT(removeOverscript()));
}

void BigQChar_SN::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->setFont(Globals::bigqchar_font);
    QRectF symbol_rect = Globals::bigqchar_font_metrics.boundingRect(ch);
    symbol_rect.moveTo(symbol_x, symbol_y);
    painter->drawText(symbol_rect, Qt::AlignBottom|Qt::AlignHCenter, ch);
}

void BigQChar_SN::removeOverscript(){
    typesetDocument()->undo_stack->push( new RemoveOverscript(this) );
}

BigQChar_SN::RemoveOverscript::RemoveOverscript(BigQChar_SN* out)
    : out(out) {
    in = new BigQChar_S(out->ch, out->second);
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

BigQChar_SN::RemoveOverscript::~RemoveOverscript(){
    if(active){
        out->first->deletePostorder();
        delete out;
    }else{
        delete in;
    }
}

void BigQChar_SN::RemoveOverscript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->child->setParentConstruct(*in);
    in->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void BigQChar_SN::RemoveOverscript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->second->setParentConstruct(*out);
    out->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

#undef overscript
#undef underscript

#include "integral.h"

#include "algorithm.h"
#include "cursor.h"
#include "typesetscene.h"
#include "globals.h"
#include <QMenu>
#include <QPainter>

static constexpr qreal subscript_ratio = 0.8;
static constexpr qreal subscript_offset = -6;
static constexpr qreal superscript_ratio = 0.8;
static constexpr qreal slant = 19;
static const QRectF integral_bounds = QRectF(3, -5, 16, 29);

Integral::Integral(QChar qchar, bool allow_superscript)
    : ch(qchar),
      allow_superscript(allow_superscript) {
    updateLayout();
}

void Integral::updateLayout(){
    w = integral_bounds.width();
    u = d = integral_bounds.height()/2;
}

void Integral::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    QAction* addSubscript = menu.addAction(QString(ch) + ": Add subscript");
    connect(addSubscript, SIGNAL(triggered()), this, SLOT(addSubscript()));
}

void Integral::write(QTextStream& out) const{
    out << ESCAPE << ch;
}

void Integral::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->setFont(Globals::integral_font);
    painter->rotate(slant);
    painter->drawText(integral_bounds, Qt::AlignCenter, ch);
}

void Integral::addSubscript(){
    typesetDocument()->undo_stack->push( new AddSubscript(this) );
}

Integral::AddSubscript::AddSubscript(Integral* out)
    : out(out) {
    Text* t = new Text(out->prev->getScriptLevel() + 1);
    t->next = t->prev = nullptr;

    in = new Integral_S(out->ch, new SubPhrase(t), out->allow_superscript);
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

Integral::AddSubscript::~AddSubscript(){
    if(active) out->deletePostorder();
    else in->deletePostorder();
}

void Integral::AddSubscript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->prev->parent->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void Integral::AddSubscript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->prev->parent->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

Integral_S::Integral_S(QChar qchar, SubPhrase* subscript, bool allow_superscript)
    : UnaryConstruct(subscript),
      ch(qchar),
      allow_superscript(allow_superscript) {
    updateLayout();
}

#define subscript child
void Integral_S::updateLayout(){
    qreal wi = integral_bounds.width();
    qreal hi = integral_bounds.height();

    w = wi + subscript->w + subscript_offset;

    qreal a = subscript_ratio*hi/2;

    qreal hs = subscript->u + subscript->d;
    qreal e = hs > a ? hs - a : 0;
    u = integral_bounds.height()/2;
    d = u + e;

    qreal y = hi + e - hs;

    subscript->setPos(wi + subscript_offset, y);
}

void Integral_S::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    if(allow_superscript){
        QAction* addSuperscript = menu.addAction(QString(ch) + ": Add superscript");
        connect(addSuperscript, SIGNAL(triggered()), this, SLOT(addSuperscript()));
    }
    QAction* removeSubscript = menu.addAction(QString(ch) + ": Remove subscript");
    connect(removeSubscript, SIGNAL(triggered()), this, SLOT(removeSubscript()));
}

void Integral_S::write(QTextStream& out) const{
    out << ESCAPE << ch;
    child->write(out);
}

void Integral_S::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->setFont(Globals::integral_font);
    painter->rotate(slant);
    painter->drawText(integral_bounds, Qt::AlignCenter, ch);
}

void Integral_S::addSuperscript(){
    typesetDocument()->undo_stack->push( new AddSuperscript(this) );
}

void Integral_S::removeSubscript(){
    typesetDocument()->undo_stack->push( new RemoveSubscript(this) );
}

Integral_S::AddSuperscript::AddSuperscript(Integral_S* out)
    : out(out) {
    Text* t = new Text(out->prev->getScriptLevel() + 1);
    t->next = t->prev = nullptr;

    in = new Integral_SN(out->ch, out->child, new SubPhrase(t));
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

Integral_S::AddSuperscript::~AddSuperscript(){
    if(active) delete out;
    else{
        in->second->deletePostorder();
        delete in;
    }
}

void Integral_S::AddSuperscript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->first->setParentConstruct(*in);
    in->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void Integral_S::AddSuperscript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->child->setParentConstruct(*out);
    out->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

Integral_S::RemoveSubscript::RemoveSubscript(Integral_S* out)
    : out(out) {
    in = new Integral(out->ch, out->allow_superscript);
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

Integral_S::RemoveSubscript::~RemoveSubscript(){
    if(active) out->deletePostorder();
    else in->deletePostorder();
}

void Integral_S::RemoveSubscript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->prev->parent->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void Integral_S::RemoveSubscript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->prev->parent->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}
#undef subscript

Integral_SN::Integral_SN(QChar qchar, SubPhrase* subscript, SubPhrase* superscript)
    : BinaryConstruct(subscript, superscript),
      ch(qchar) {
    updateLayout();
}

#define superscript second
#define subscript first
void Integral_SN::updateLayout(){
    qreal wi = integral_bounds.width();
    qreal hi = integral_bounds.height();

    superscript->setPos(wi, 0);
    w = wi + qMax(subscript->w + subscript_offset, superscript->w);

    qreal au = superscript_ratio*hi/2;
    qreal ad = subscript_ratio*hi/2;
    qreal hu = superscript->u + superscript->d;
    qreal hd = subscript->u + subscript->d;
    qreal eu = hu > au ? hu-au : 0;
    qreal ed = hd > ad ? hd-ad : 0;

    symbol_y = eu;
    u = symbol_y + hi/2;

    qreal yd = symbol_y + hi + ed - hd;
    d = hi/2 + ed;

    subscript->setPos(wi + subscript_offset, yd);
}

Text* Integral_SN::textUp(const SubPhrase* caller, qreal x) const{
    return (caller==subscript) ? Algorithm::textAtSetpoint(*superscript, x) : prev;
}

Text* Integral_SN::textDown(const SubPhrase* caller, qreal x) const{
    return (caller==superscript) ? Algorithm::textAtSetpoint(*subscript, x) : next;
}

void Integral_SN::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    QAction* removeSuperscript = menu.addAction(QString(ch) + ": Remove superscript");
    connect(removeSuperscript, SIGNAL(triggered()), this, SLOT(removeSuperscript()));
}

void Integral_SN::write(QTextStream& out) const{
    out << ESCAPE << ch;
    first->write(out);
    second->write(out);
}

void Integral_SN::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->setFont(Globals::integral_font);
    painter->rotate(slant);
    QRectF bounds = integral_bounds.translated(0, symbol_y);
    painter->drawText(bounds, Qt::AlignCenter, ch);
}

void Integral_SN::removeSuperscript(){
    typesetDocument()->undo_stack->push( new RemoveSuperscript(this) );
}

Integral_SN::RemoveSuperscript::RemoveSuperscript(Integral_SN* out)
    : out(out) {
    in = new Integral_S(out->ch, out->first);
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

Integral_SN::RemoveSuperscript::~RemoveSuperscript(){
    if(active){
        out->second->deletePostorder();
        delete out;
    }else{
        delete in;
    }
}

void Integral_SN::RemoveSuperscript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->child->setParentConstruct(*in);
    in->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void Integral_SN::RemoveSuperscript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->first->setParentConstruct(*out);
    out->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

#undef subscript
#undef superscript

#include "integral.h"

#include "algorithm.h"
#include "cursor.h"
#include "document.h"
#include "globals.h"
#include <QMenu>

namespace Typeset{

static constexpr qreal subscript_ratio = 0.8;
static constexpr qreal subscript_offset = -6;
static constexpr qreal superscript_ratio = 0.8;

Integral::Integral(QChar qchar){
    setFlag(QGraphicsItem::ItemHasNoContents);

    big_integral.setText(qchar);
    big_integral.setParentItem(this);
    big_integral.setFlag(QGraphicsItem::ItemIsSelectable, false);
    big_integral.setFlag(QGraphicsItem::ItemStacksBehindParent);
    big_integral.setFont(Globals::integral_font);
    big_integral.setBrush(Globals::construct_brush);

    updateLayout();
}

void Integral::updateTheme(){
    big_integral.setBrush(Globals::construct_brush);
}

void Integral::updateLayout(){
    QRectF child_bounds = big_integral.boundingRect();
    w = child_bounds.width();
    u = d = child_bounds.height()/2;
}

void Integral::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    QAction* addSubscript = menu.addAction(big_integral.text() + ": Add subscript");
    connect(addSubscript, SIGNAL(triggered()), this, SLOT(addSubscript()));
}

void Integral::write(QTextStream& out) const{
    out << ESCAPE << big_integral.text();
}

void Integral::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){
    //DO NOTHING
}

void Integral::addSubscript(){
    typesetDocument()->undo_stack->push( new AddSubscript(this) );
}

Integral::AddSubscript::AddSubscript(Integral* out)
    : out(out) {
    Text* t = new Text(out->prev->getScriptLevel() + 1);
    t->next = t->prev = nullptr;

    in = new Integral_S(out->big_integral.text().front(), new SubPhrase(t));
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

Integral_S::Integral_S(QChar qchar, SubPhrase* subscript)
    : UnaryConstruct(subscript) {
    setFlag(QGraphicsItem::ItemHasNoContents);

    big_integral.setText(qchar);
    big_integral.setParentItem(this);
    big_integral.setFlag(QGraphicsItem::ItemIsSelectable, false);
    big_integral.setFlag(QGraphicsItem::ItemStacksBehindParent);
    big_integral.setFont(Globals::integral_font);
    big_integral.setBrush(Globals::construct_brush);

    updateLayout();
}

#define subscript child
void Integral_S::updateTheme(){
    big_integral.setBrush(Globals::construct_brush);
    subscript->updateTheme();
}

void Integral_S::updateLayout(){
    QRectF integral_bounds = big_integral.boundingRect();
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
    QAction* addSuperscript = menu.addAction(big_integral.text() + ": Add superscript");
    connect(addSuperscript, SIGNAL(triggered()), this, SLOT(addSuperscript()));
    QAction* removeSubscript = menu.addAction(big_integral.text() + ": Remove subscript");
    connect(removeSubscript, SIGNAL(triggered()), this, SLOT(removeSubscript()));
}

void Integral_S::write(QTextStream& out) const{
    out << ESCAPE << big_integral.text();
    child->write(out);
}

void Integral_S::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){
    //DO NOTHING
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

    in = new Integral_SN(out->big_integral.text().front(), out->child, new SubPhrase(t));
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
    in = new Integral(out->big_integral.text().front());
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
    : BinaryConstruct(subscript, superscript) {
    setFlag(QGraphicsItem::ItemHasNoContents);

    big_integral.setText(qchar);
    big_integral.setParentItem(this);
    big_integral.setFlag(QGraphicsItem::ItemIsSelectable, false);
    big_integral.setFlag(QGraphicsItem::ItemStacksBehindParent);
    big_integral.setFont(Globals::integral_font);
    big_integral.setBrush(Globals::construct_brush);

    updateLayout();
}

#define superscript second
#define subscript first
void Integral_SN::updateTheme(){
    big_integral.setBrush(Globals::construct_brush);
    subscript->updateTheme();
    superscript->updateTheme();
}

void Integral_SN::updateLayout(){
    QRectF integral_bounds = big_integral.boundingRect();
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

    qreal yi = eu;
    u = yi + hi/2;

    big_integral.setPos(0, yi);

    qreal yd = yi + hi + ed - hd;
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
    QAction* removeSuperscript = menu.addAction(big_integral.text() + ": Remove superscript");
    connect(removeSuperscript, SIGNAL(triggered()), this, SLOT(removeSuperscript()));
}

void Integral_SN::write(QTextStream& out) const{
    out << ESCAPE << big_integral.text();
    first->write(out);
    second->write(out);
}

void Integral_SN::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){
    //DO NOTHING
}

void Integral_SN::removeSuperscript(){
    typesetDocument()->undo_stack->push( new RemoveSuperscript(this) );
}

Integral_SN::RemoveSuperscript::RemoveSuperscript(Integral_SN* out)
    : out(out) {
    in = new Integral_S(out->big_integral.text().front(), out->first);
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

}

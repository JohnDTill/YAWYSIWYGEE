#include "script.h"

#include "algorithm.h"
#include "cursor.h"

namespace Typeset{

Superscript::Superscript(SubPhrase* child, SubPhrase* superscript)
    : BinaryConstruct(child, superscript) {
    setFlag(QGraphicsItem::ItemHasNoContents);
    updateLayout();
}

#define child first
#define superscript second
void Superscript::updateLayout(){
    d = child->d;
    w = child->w + superscript->w;

    const qreal hs = superscript->u + superscript->d;
    const qreal a = ratio_superscript*child->u;
    const qreal e = (hs > a) ? hs - a : 0;

    u = child->u + e;

    child->setPos(0, e);
    superscript->setPos(child->w, 0);
}

Text* Superscript::textUp(const SubPhrase*, qreal) const{
    return prev;
}

Text* Superscript::textDown(const SubPhrase*, qreal) const{
    return next;
}

void Superscript::write(QTextStream& out) const{
    out << ESCAPE << "^";
    child->write(out);
    superscript->write(out);
}

void Superscript::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
}
#undef child
#undef superscript

Subscript::Subscript(SubPhrase* child, SubPhrase* subscript)
    : BinaryConstruct(child, subscript) {
    setFlag(QGraphicsItem::ItemHasNoContents);
    updateLayout();
}

#define child first
#define subscript second
void Subscript::updateLayout(){
    u = child->u;
    w = child->w + subscript->w;

    const qreal hs = subscript->u + subscript->d;
    const qreal a = ratio_subscript*child->d;
    const qreal e = (hs > a) ? hs - a : 0; //Amount script extends beyond child

    d = child->d + e;

    const qreal ys = (hs > a) ? child->u + (1-ratio_subscript)*child->d : child->u + (1-ratio_subscript)*child->d - hs;
    subscript->setPos(child->w, ys);
}

Text* Subscript::textUp(const SubPhrase*, qreal) const{
    return prev;
}

Text* Subscript::textDown(const SubPhrase*, qreal) const{
    return next;
}

void Subscript::write(QTextStream& out) const{
    out << ESCAPE << "_";
    child->write(out);
    subscript->write(out);
}

void Subscript::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
}
#undef child
#undef subscript

Dualscript::Dualscript(SubPhrase* child, SubPhrase* subscript, SubPhrase* superscript)
    : child(child),
      subscript(subscript),
      superscript(superscript) {
    setFlag(QGraphicsItem::ItemHasNoContents);

    if(child==nullptr) this->child = new SubPhrase();
    if(subscript==nullptr) this->subscript = new SubPhrase();
    if(superscript==nullptr) this->superscript = new SubPhrase();
    child->setParentConstruct(*this);
    subscript->setParentConstruct(*this);
    superscript->setParentConstruct(*this);

    updateLayout();
}

void Dualscript::deletePostorder(){
    child->deletePostorder();
    subscript->deletePostorder();
    superscript->deletePostorder();
    delete this;
}

void Dualscript::select(){
    setSelected(true);
    child->select();
    subscript->select();
    superscript->select();
}

void Dualscript::updateLayout(){
    w = child->w + qMax(subscript->w, superscript->w);

    qreal hs = superscript->u + superscript->d;
    qreal a = ratio_superscript*child->u;
    qreal e = (hs > a) ? hs - a : 0;

    u = child->u + e;

    qreal ym = e;
    child->setPos(0, ym);
    superscript->setPos(child->w, 0);

    w = child->w + subscript->w;

    hs = subscript->u + subscript->d;
    a = ratio_subscript*child->d;
    e = (hs > a) ? hs - a : 0; //Amount script extends beyond child

    d = child->d + e;

    const qreal ys = (hs > a) ? ym + child->u + (1-ratio_subscript)*child->d : child->u + (1-ratio_subscript)*child->d - hs;
    subscript->setPos(child->w, ys);
}

SubPhrase* Dualscript::front() const{
    return child;
}

SubPhrase* Dualscript::back() const{
    return subscript;
}

Text* Dualscript::textRight(const SubPhrase* caller) const{
    if(caller==child) return superscript->front;
    else if(caller==superscript) return subscript->front;
    else return next;
}

Text* Dualscript::textLeft(const SubPhrase* caller) const{
    if(caller==subscript) return superscript->back;
    else if(caller==superscript) return child->back;
    else return prev;
}

Text* Dualscript::textUp(const SubPhrase* caller, qreal x) const{
    return (caller==subscript) ? Algorithm::textAtSetpoint(*superscript, x) : prev;
}

Text* Dualscript::textDown(const SubPhrase* caller, qreal x) const{
    return (caller==superscript) ? Algorithm::textAtSetpoint(*subscript, x) : next;
}

void Dualscript::write(QTextStream& out) const{
    out << ESCAPE << QChar(916);
    child->write(out);
    subscript->write(out);
    superscript->write(out);
}

void Dualscript::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
}

}

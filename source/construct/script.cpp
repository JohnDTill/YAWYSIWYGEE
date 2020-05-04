#include "script.h"

#include "../algorithm.h"
#include "../cursor.h"
#include "../subphrase.h"
#include "../text.h"
#include "../typesetscene.h"

#include <QMenu>
#include <QPainter>

static constexpr qreal ratio_superscript = 1;
static constexpr qreal ratio_subscript = 1;
static constexpr qreal x_offset = -2;

Superscript::Superscript(SubPhrase* superscript)
    : UnaryConstruct(superscript) {
    setFlag(QGraphicsItem::ItemHasNoContents);
    d = 0;
    superscript->setPos(x_offset, 0);
    updateLayout();
}

#define superscript child
void Superscript::updateLayout(){
    w = superscript->w + x_offset;

    const qreal hs = superscript->u + superscript->d;
    const qreal a = ratio_superscript*body_u;
    const qreal e = (hs > a) ? hs - a : 0;

    u = body_u + e;
}

void Superscript::notifyPrevUpdate(){
    body_u = prev->u;

    updateLayout();
    next->notifyPrevUpdate();
}

void Superscript::notifyPrevPrevUpdate(Construct* c){
    body_u = c->u;

    updateLayout();
    next->notifyPrevUpdate();
}

void Superscript::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    QAction* subscriptAction = menu.addAction("Superscript: Add subscript");
    connect(subscriptAction, SIGNAL(triggered()), this, SLOT(addSubscript()));
}

void Superscript::write(QTextStream& out) const{
    out << MB_CONSTRUCT_SYMBOL << "^";
    superscript->write(out);
}

void Superscript::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
}

void Superscript::addSubscript(){
    Cursor* c = typesetDocument()->cursor;
    QString str;
    QTextStream out(&str);
    out << MB_CONSTRUCT_SYMBOL << QChar(916);
    out << MB_OPEN << MB_CLOSE;
    superscript->write(out);
    c->clickConstruct(*this);
    c->paste(str);
}
#undef superscript

Subscript::Subscript(SubPhrase* subscript)
    : UnaryConstruct(subscript) {
    setFlag(QGraphicsItem::ItemHasNoContents);
    u = 0;
    updateLayout();
}

#define subscript child
void Subscript::updateLayout(){
    w = subscript->w + x_offset;

    const qreal hs = subscript->u + subscript->d;
    const qreal a = ratio_subscript*body_d;
    const qreal e = (hs > a) ? hs - a : 0; //Amount script extends beyond child

    d = body_d + e;

    const qreal ys = body_d + e - hs;
    subscript->setPos(x_offset, ys);
}

void Subscript::notifyPrevUpdate(){
    body_d = prev->u;

    updateLayout();
    next->notifyPrevUpdate();
}

void Subscript::notifyPrevPrevUpdate(Construct* c){
    body_d = c->d;

    updateLayout();
    next->notifyPrevUpdate();
}

void Subscript::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    QAction* superscriptAction = menu.addAction("Subscript: Add superscript");
    connect(superscriptAction, SIGNAL(triggered()), this, SLOT(addSuperscript()));
}

void Subscript::write(QTextStream& out) const{
    out << MB_CONSTRUCT_SYMBOL << "_";
    subscript->write(out);
}

void Subscript::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
}

void Subscript::addSuperscript(){
    Cursor* c = typesetDocument()->cursor;
    QString str;
    QTextStream out(&str);
    out << MB_CONSTRUCT_SYMBOL << QChar(916);
    subscript->write(out);
    out << MB_OPEN << MB_CLOSE;
    c->clickConstruct(*this);
    c->paste(str);
}
#undef subscript

static constexpr qreal bar_offset = 3;
static constexpr qreal eval_offset = bar_offset + 2;

Dualscript::Dualscript(SubPhrase* subscript, SubPhrase* superscript, bool eval)
    : BinaryConstruct(superscript, subscript),
      eval(eval) {
    if(!eval) setFlag(QGraphicsItem::ItemHasNoContents);

    superscript->setPos(x_offset + eval*eval_offset, 0);
    updateLayout();
}

#define superscript first
#define subscript second

void Dualscript::updateLayout(){
    w = qMax(subscript->w, superscript->w) + x_offset + eval*eval_offset;

    qreal hs = superscript->u + superscript->d;
    qreal a = ratio_superscript*body_u;
    qreal e = (hs > a) ? hs - a : 0;

    u = body_u + e;

    qreal ym = e;

    hs = subscript->u + subscript->d;
    a = ratio_subscript*body_d;
    e = (hs > a) ? hs - a : 0; //Amount script extends beyond child

    d = body_d + e;

    const qreal ys = ym + (body_u + body_d) + e - hs;
    subscript->setPos(x_offset + eval*eval_offset, ys);
}

void Dualscript::notifyPrevUpdate(){
    body_u = body_d = prev->u;

    updateLayout();
    next->notifyPrevUpdate();
}

void Dualscript::notifyPrevPrevUpdate(Construct* c){
    body_u = c->u;
    body_d = c->d;

    updateLayout();
    next->notifyPrevUpdate();
}

void Dualscript::populateMenu(QMenu& menu, const SubPhrase*){
    if(eval) return;

    menu.addSeparator();
    QAction* superscriptAction = menu.addAction("Dualscript: Remove superscript");
    connect(superscriptAction, SIGNAL(triggered()), this, SLOT(removeSuperscript()));
    QAction* subscriptAction = menu.addAction("Dualscript: Remove subscript");
    connect(subscriptAction, SIGNAL(triggered()), this, SLOT(removeSubscript()));
}

Text* Dualscript::textUp(const SubPhrase* caller, qreal x) const{
    return (caller==subscript) ? Algorithm::textAtSetpoint(*superscript, x) : prev;
}

Text* Dualscript::textDown(const SubPhrase* caller, qreal x) const{
    return (caller==superscript) ? Algorithm::textAtSetpoint(*subscript, x) : next;
}

void Dualscript::write(QTextStream& out) const{
    out << MB_CONSTRUCT_SYMBOL << (eval ? QChar(9482) : QChar(916));
    subscript->write(out);
    superscript->write(out);
}

void Dualscript::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    if(eval) painter->drawLine( QLineF(x_offset + eval*bar_offset, 0, x_offset+eval*bar_offset, h()) );
}

void Dualscript::removeSuperscript(){
    Cursor* c = typesetDocument()->cursor;
    QString str;
    QTextStream out(&str);
    out << MB_CONSTRUCT_SYMBOL << '_';
    subscript->write(out);
    c->clickConstruct(*this);
    c->paste(str);
}

void Dualscript::removeSubscript(){
    Cursor* c = typesetDocument()->cursor;
    QString str;
    QTextStream out(&str);
    out << MB_CONSTRUCT_SYMBOL << '^';
    superscript->write(out);
    c->clickConstruct(*this);
    c->paste(str);
}

#undef superscript
#undef subscript

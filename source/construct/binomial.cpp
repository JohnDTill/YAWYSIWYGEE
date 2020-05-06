#include "binomial.h"

#include "../algorithm.h"
#include "../cursor.h"
#include "../globals.h"
#include "../subphrase.h"
#include <QPainter>
#include <QTextStream>

Binomial::Binomial(SubPhrase* f, SubPhrase* s)
    : BinaryConstruct(f,s) {
    f->setPos(paren_width, 0);
    updateLayout();
}

void Binomial::updateLayout(){
    qreal max_child_width = qMax(first->w, second->w);
    w = max_child_width + 2*paren_width;
    qreal first_h = first->u + first->d;
    h = first_h + second->u + second->d;
    u = d = h/2;

    first->setPos(paren_width + (max_child_width-first->w)/2, 0);
    second->setPos(paren_width + (max_child_width-second->w)/2, first_h);
}

Text* Binomial::textUp(const SubPhrase* caller, qreal x) const{
    return (caller==second) ? Algorithm::textAtSetpoint(*first, x) : prev;
}

Text* Binomial::textDown(const SubPhrase* caller, qreal x) const{
    return (caller==first) ? Algorithm::textAtSetpoint(*second, x) : next;
}

void Binomial::write(QTextStream& out) const{
    out << MB_CONSTRUCT_SYMBOL << "b";
    first->write(out);
    second->write(out);
}

void Binomial::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->drawArc(QRectF(0,0,paren_width,h), 90*16, 180*16);
    painter->drawArc(QRectF(w-paren_width,0,paren_width,h), 270*16, 180*16);
}

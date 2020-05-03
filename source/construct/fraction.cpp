#include "fraction.h"

#include "algorithm.h"
#include "cursor.h"
#include "globals.h"
#include "subphrase.h"
#include <QPainter>

Fraction::Fraction(SubPhrase* num, SubPhrase* den)
    : BinaryConstruct(num, den) {
    updateLayout();
}

#define num first
#define den second
void Fraction::updateLayout(){
    calculateSize();
    num->setPos( (w - num->w)/2, 0 );
    den->setPos( (w - den->w)/2, u + 2*vspace );
}

Text* Fraction::textUp(const SubPhrase* caller, qreal x) const{
    return (caller==second) ? Algorithm::textAtSetpoint(*first, x) : prev;
}

Text* Fraction::textDown(const SubPhrase* caller, qreal x) const{
    return (caller==first) ? Algorithm::textAtSetpoint(*second, x) : next;
}

void Fraction::write(QTextStream& out) const{
    out << ESCAPE << "f";
    first->write(out);
    second->write(out);
}

void Fraction::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->drawLine(QPointF(0,u), QPointF(w,u));
}

void Fraction::calculateSize(){
    w = qMax(num->w, den->w) + bar_extension;
    u = vspace + num->u + num->d;
    d = vspace + den->u + den->d;
}
#undef num
#undef den

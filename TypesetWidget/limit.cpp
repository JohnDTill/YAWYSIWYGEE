#include "limit.h"

#include <QPainter>

namespace Typeset{

static constexpr qreal margin = 1;

Limit::Limit(SubPhrase* var, SubPhrase* to)
    : BinaryConstruct(var, to){
    QRectF word_bounds = Globals::word_font_metrics.boundingRect("lim");
    lw = word_bounds.width();
    lh = word_bounds.height();

    QRectF arrow_bounds = Globals::word_font_metrics.boundingRect("→");
    aw = arrow_bounds.width();
    ah = arrow_bounds.height();

    u = lh/2;

    updateLayout();
}

void Typeset::Limit::updateLayout(){
    qreal bu = qMax(first->u, second->u);
    ay = lh + bu - ah/2;
    d = lh/2 + bu + qMax(first->d, second->d);

    ax = qMax(first->w, (lw-aw)/2) + margin;
    lx = ax - (lw-aw)/2;

    first->setPos(ax - margin - first->w, lh + bu - first->u);
    second->setPos(ax + aw + margin, lh + bu - second->u);

    w = qMax(lx+lw, ax + aw + margin + second->w);
}

Text* Limit::textUp(const SubPhrase*, qreal) const{
    return prev;
}

Text* Limit::textDown(const SubPhrase*, qreal) const{
    return next;
}

void Limit::write(QTextStream& out) const{
    out << ESCAPE << "l";
    first->write(out);
    second->write(out);
}

void Limit::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->setFont(Globals::word_font);
    painter->drawText(QRectF(lx, 0, lw, lh), "lim");
    painter->drawText(QRectF(ax, ay, aw, ah), "→");
}

}

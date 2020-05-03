#include "grouping.h"

#include "globals.h"
#include <QPainter>

namespace Typeset{

Grouping::Grouping(void drawLeft(QPainter*, const qreal&),
                   void drawRight(QPainter*, const qreal&, const qreal&),
                   QChar type,
                   SubPhrase* c)
    : UnaryConstruct(c),
      drawLeft(drawLeft),
      drawRight(drawRight),
      type(type) {
    c->setPos(symbol_width, 0);
    updateLayout();
}

void Grouping::updateLayout(){
    w = child->w + 2*symbol_width;
    u = child->u;
    d = child->d;
}

void Grouping::write(QTextStream& out) const{
    out << ESCAPE << type;
    child->write(out);
}

void Grouping::ANGLE(QPainter* painter, const qreal& height){
    painter->drawLine(QLineF(symbol_width, 0, symbol_width/3, height/2));
    painter->drawLine(QLineF(symbol_width, height, symbol_width/3, height/2));
}

void Grouping::ANGLE(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawLine(QLineF(x + symbol_width*1/3, 0, x + symbol_width, height/2));
    painter->drawLine(QLineF(x + symbol_width*1/3, height, x + symbol_width, height/2));
}

void Grouping::BAR(QPainter* painter, const qreal& height){
    painter->drawLine(QLineF(symbol_width/2, 0, symbol_width/2, height));
}

void Grouping::BAR(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawLine(QLineF(x + symbol_width/2, 0, x + symbol_width/2, height));
}

void Grouping::BLANK(QPainter*, const qreal&){
    //DO NOTHING
}

void Grouping::BLANK(QPainter*, const qreal&, const qreal&){
    //DO NOTHING
}

void Grouping::BRACE(QPainter* painter, const qreal& height){
    painter->drawArc(QRectF(symbol_width/2,0,symbol_width/2,height/2), 90*16, 90*16);
    painter->drawArc(QRectF(0,0,symbol_width/2,height/2), 270*16, 90*16);
    painter->drawArc(QRectF(0,height/2,symbol_width/2,height/2), 0*16, 90*16);
    painter->drawArc(QRectF(symbol_width/2,height/2,symbol_width/2,height/2), 180*16, 90*16);
}

void Grouping::BRACE(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawArc(QRectF(x,0,symbol_width/2,height/2), 0*16, 90*16);
    painter->drawArc(QRectF(x+symbol_width/2,0,symbol_width/2,height/2), 180*16, 90*16);
    painter->drawArc(QRectF(x+symbol_width/2,height/2,symbol_width/2,height/2), 90*16, 90*16);
    painter->drawArc(QRectF(x,height/2,symbol_width/2,height/2), 270*16, 90*16);
}

void Grouping::BRACKET(QPainter* painter, const qreal& height){
    painter->drawLine(QLineF(0, 0, 0, height));
    painter->drawLine(QLineF(0, 0, symbol_width, 0));
    painter->drawLine(QLineF(0, height, symbol_width, height));
}

void Grouping::BRACKET(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawLine(QLineF(x + symbol_width, 0, x + symbol_width, height));
    painter->drawLine(QLineF(x, 0, x + symbol_width, 0));
    painter->drawLine(QLineF(x, height, x + symbol_width, height));
}

void Grouping::CEIL(QPainter* painter, const qreal& height){
    painter->drawLine(QLineF(0, 0, 0, height));
    painter->drawLine(QLineF(0, 0, symbol_width, 0));
}

void Grouping::CEIL(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawLine(QLineF(x + symbol_width, 0, x + symbol_width, height));
    painter->drawLine(QLineF(x, 0, x + symbol_width, 0));
}

void Grouping::DOUBLE_ANGLE(QPainter* painter, const qreal& height){
    painter->drawLine(QLineF(symbol_width, 0, symbol_width/3, height/2));
    painter->drawLine(QLineF(symbol_width, height, symbol_width/3, height/2));
    painter->drawLine(QLineF(symbol_width*2/3, 0, 0, height/2));
    painter->drawLine(QLineF(symbol_width*2/3, height, 0, height/2));
}

void Grouping::DOUBLE_ANGLE(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawLine(QLineF(x + symbol_width*1/3, 0, x + symbol_width, height/2));
    painter->drawLine(QLineF(x + symbol_width*1/3, height, x + symbol_width, height/2));
    painter->drawLine(QLineF(x, 0, x + symbol_width*2/3, height/2));
    painter->drawLine(QLineF(x, height, x + symbol_width*2/3, height/2));
}

void Grouping::DOUBLE_BRACKET(QPainter* painter, const qreal& height){
    painter->drawLine(QLineF(0, 0, 0, height));
    painter->drawLine(QLineF(symbol_width/3, 0, symbol_width/3, height));
    painter->drawLine(QLineF(0, 0, symbol_width, 0));
    painter->drawLine(QLineF(0, height, symbol_width, height));
}

void Grouping::DOUBLE_BRACKET(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawLine(QLineF(x + symbol_width, 0, x + symbol_width, height));
    painter->drawLine(QLineF(x + symbol_width*2/3, 0, x + symbol_width*2/3, height));
    painter->drawLine(QLineF(x, 0, x + symbol_width, 0));
    painter->drawLine(QLineF(x, height, x + symbol_width, height));
}

void Grouping::FLOOR(QPainter* painter, const qreal& height){
    painter->drawLine(QLineF(0, 0, 0, height));
    painter->drawLine(QLineF(0, height, symbol_width, height));
}

void Grouping::FLOOR(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawLine(QLineF(x + symbol_width, 0, x + symbol_width, height));
    painter->drawLine(QLineF(x, height, x + symbol_width, height));
}

void Grouping::NORM(QPainter* painter, const qreal& height){
    painter->drawLine(QLineF(0, 0, 0, height));
    painter->drawLine(QLineF(symbol_width/2, 0, symbol_width/2, height));
}

void Grouping::NORM(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawLine(QLineF(x + symbol_width/2, 0, x + symbol_width/2, height));
    painter->drawLine(QLineF(x + symbol_width, 0, x + symbol_width, height));
}

void Grouping::PARENTHESIS(QPainter* painter, const qreal& height){
    painter->drawArc(QRectF(0,0,symbol_width,height), 90*16, 180*16);
}

void Grouping::PARENTHESIS(QPainter* painter, const qreal& height, const qreal& x){
    painter->drawArc(QRectF(x,0,symbol_width,height), 270*16, 180*16);
}

void Grouping::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);

    qreal h = u+d;
    drawLeft(painter, h);
    drawRight(painter, h , child->w + symbol_width);
}

}

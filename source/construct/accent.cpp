#include "accent.h"

#include "globals.h"
#include <QPainter>

namespace Typeset{

static constexpr qreal accent_height = 3;
static constexpr qreal voffset = 0.5;
static constexpr qreal arrow_width = 2;
static constexpr qreal dot_radius = 0.4;
static constexpr qreal dot_offset = 2.5;

Accent::Accent(void (*drawAccent)(QPainter*, const qreal&), SubPhrase* c)
    : UnaryConstruct(c),
      drawAccent(drawAccent) {
    child->setPos(0, accent_height - voffset);
    updateLayout();
}

void Accent::updateLayout(){
    w = child->w;
    u = child->u + accent_height - voffset;
    d = child->d;
}

void Accent::write(QTextStream& out) const{
    out << ESCAPE;

    if(drawAccent==ARROW) out << QChar(8594); //→
    else if(drawAccent==BREVE) out << QChar(259); //ă
    else if(drawAccent==DOT) out << QChar(551); //ȧ
    else if(drawAccent==DDOT) out << QChar(228); //ä
    else if(drawAccent==DDDOT) out << QChar(8943); //⋯
    else if(drawAccent==HAT) out << QChar(226); //â
    else if(drawAccent==OVERBAR) out << QChar(257); //ā
    else if(drawAccent==TILDE) out << QChar(227); //ã

    child->write(out);
}

void Accent::ARROW(QPainter* painter, const qreal& width){
    painter->drawLine(QLineF(0, voffset + accent_height/2, width, voffset + accent_height/2));
    painter->drawLine(QLineF(width, voffset + accent_height/2, width-arrow_width, voffset));
    painter->drawLine(QLineF(width, voffset + accent_height/2, width-arrow_width, voffset + accent_height));
}

void Accent::BREVE(QPainter* painter, const qreal& width){
    static constexpr int angle = 10;
    painter->drawArc(QRectF(0,voffset-2,width,accent_height), (180+angle)*16, (180-2*angle)*16);
}

static void drawDot(QPainter* painter, qreal x, qreal y){
    QBrush brush = painter->brush();
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    painter->setBrush(brush);
    painter->drawEllipse(QRectF(x-dot_radius, y-dot_radius, 2*dot_radius, 2*dot_radius));
}

void Accent::DOT(QPainter* painter, const qreal& width){
    drawDot(painter, width/2, voffset + accent_height/2);
}

void Accent::DDOT(QPainter* painter, const qreal& width){
    drawDot(painter, width/2 + dot_offset/2, voffset + accent_height/2);
    drawDot(painter, width/2 - dot_offset/2, voffset + accent_height/2);
}

void Accent::DDDOT(QPainter* painter, const qreal& width){
    drawDot(painter, width/2, voffset + accent_height/2);
    drawDot(painter, width/2 + dot_offset, voffset + accent_height/2);
    drawDot(painter, width/2 - dot_offset, voffset + accent_height/2);
}

void Accent::HAT(QPainter* painter, const qreal& width){
    painter->drawLine(QLineF(0, voffset + accent_height, width/2, voffset));
    painter->drawLine(QLineF(width/2, voffset, width, voffset + accent_height));
}

void Accent::OVERBAR(QPainter* painter, const qreal& width){
    painter->drawLine(QLineF(0, voffset + accent_height/2, width, voffset + accent_height/2));
}

void Accent::TILDE(QPainter* painter, const qreal& width){
    qreal xs = width/(4*1.41);
    qreal ys = 0.7*accent_height/(2*1.41);

    QPointF points[9] = {{0, voffset + accent_height*3/4}, //Semi-circle centetered at (width/4, accent_height/2)
                         {width/4 - xs, voffset + accent_height/2 - ys},
                         {width/4, voffset},
                         {width/4 + xs, voffset + accent_height/2 - ys},
                         {width/2, voffset + accent_height/2},
                         {width*3/4 - xs, voffset + accent_height/2 + ys}, //Semi-circle centered at (width*3/4, accent_height/2)
                         {width*3/4, voffset + accent_height},
                         {width*3/4 + xs, voffset + accent_height/2 + ys},
                         {width, voffset + accent_height/4}};

    painter->drawPolyline(points, 9);
}

void Accent::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    drawAccent(painter, w);
}

}

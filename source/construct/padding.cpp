#include "padding.h"

#include <QTextStream>

Padding::Padding(qreal pad){
    w = pad;
    u = d = 0;
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemHasNoContents);
}

void Padding::updateLayout(){
    //DO NOTHING
}

void Padding::write(QTextStream& out) const{
    out << "⁜p⏴" << w << "⏵";
}

void Padding::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){
    //DO NOTHING
}

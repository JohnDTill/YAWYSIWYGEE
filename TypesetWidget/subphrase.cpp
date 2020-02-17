#include "subphrase.h"

#include "construct.h"
#include "globals.h"
#include "text.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace Typeset{

SubPhrase::SubPhrase(Text* f, Text* b, major_integer child_id)
    : Phrase(f, b),
      child_id(child_id) {
    updateLayout();
}

void SubPhrase::setParentConstruct(Construct& c){
    parent = &c;
    setParentItem(&c);
}

void SubPhrase::updateToTop(){
    updateLayout();
    parent->updateToTop();
}

Text* SubPhrase::textRight() const{
    return parent->textRight(this);
}

Text* SubPhrase::textLeft() const{
    return parent->textLeft(this);
}

Text* SubPhrase::textRightUp() const{
    return parent->next;
}

Text* SubPhrase::textLeftUp() const{
    return parent->prev;
}

Text* SubPhrase::textUp(qreal x) const{
    return parent->textUp(this, x);
}

Text* SubPhrase::textDown(qreal x) const{
    return parent->textDown(this, x);
}

bool SubPhrase::isLine() const{
    return false;
}

Line& SubPhrase::getLine(){
    return parent->getLine();
}

void SubPhrase::populateMenu(QMenu& menu){
    parent->populateMenu(menu, this);
}

void SubPhrase::write(QTextStream& out) const{
    out << OPEN;
    writeContents(out);
    out << CLOSE;
}

bool SubPhrase::isEmpty() const{
    return (front == back) && front->isEmpty();
}

void SubPhrase::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    if(isEmpty()){
        QPen p = Globals::empty_box_pen;
        if(front->isSelected()) p.setColor(options->palette.highlightedText().color());
        else p.setColor(options->palette.text().color());
        painter->setPen(p);
        painter->drawRect(QRectF(-1 + padding/2, padding/2, empty_box_width, empty_box_height));
    }
}

QRectF SubPhrase::boundingRect() const{
    return QRectF(-padding, 0, w+padding, u+d+padding);
}

void SubPhrase::calculateSize(){
    if(isEmpty()){
        u = d = empty_box_height/2 + padding;
        w = empty_box_width + 2*padding;
    }else{
        Phrase::calculateSize();
    }

}

}

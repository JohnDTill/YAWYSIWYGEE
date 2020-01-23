#include "underscriptedword.h"

#include "globals.h"

namespace Typeset{

#define underscript child
UnderscriptedWord::UnderscriptedWord(QString str, SubPhrase* c)
    : UnaryConstruct(c) {
    setFlag(QGraphicsItem::ItemHasNoContents);

    word.setText(str);
    word.setParentItem(this);
    word.setFlag(QGraphicsItem::ItemIsSelectable, false);
    word.setFlag(QGraphicsItem::ItemStacksBehindParent);
    word.setFont(Globals::word_font);
    word.setBrush(Globals::construct_brush);

    updateLayout();
}

void UnderscriptedWord::updateTheme(){
    word.setBrush(Globals::construct_brush);
    underscript->updateTheme();
}

void UnderscriptedWord::updateLayout(){
    QRectF word_bounds = word.boundingRect();
    w = qMax(word_bounds.width(), underscript->w);
    u = word_bounds.height()/2;
    d = u + underscript->u + underscript->d;

    word.setPos( (w-word_bounds.width())/2, 0 );
    underscript->setPos( (w-underscript->w)/2, word_bounds.height() );
}

void UnderscriptedWord::write(QTextStream& out) const{
    out << ESCAPE << "w" << OPEN << word.text() << CLOSE;
    child->write(out);
}

void UnderscriptedWord::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){
    //DO NOTHING
}
#undef underscript

}

#include "underscriptedword.h"

#include "globals.h"
#include <QPainter>

namespace Typeset{

#define underscript child
UnderscriptedWord::UnderscriptedWord(QString str, SubPhrase* c)
    : UnaryConstruct(c),
      word(str) {
    updateLayout();
}

void UnderscriptedWord::updateLayout(){
    QRectF word_bounds = Globals::word_font_metrics.boundingRect(word);
    w = qMax(word_bounds.width(), underscript->w);
    u = word_bounds.height()/2;
    d = u + underscript->u + underscript->d;

    word_x = (w-word_bounds.width())/2;
    underscript->setPos( (w-underscript->w)/2, word_bounds.height() );
}

void UnderscriptedWord::write(QTextStream& out) const{
    out << ESCAPE << "w" << OPEN << word << CLOSE;
    child->write(out);
}

void UnderscriptedWord::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);
    painter->setFont(Globals::word_font);
    QRectF word_bounds = Globals::word_font_metrics.boundingRect(word);
    word_bounds.moveTo(word_x, 0);
    painter->drawText(word_bounds, word);
}
#undef underscript

}

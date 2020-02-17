#ifndef UNDERSCRIPTEDWORD_H
#define UNDERSCRIPTEDWORD_H

#include "construct.h"

namespace Typeset{

class UnderscriptedWord : public UnaryConstruct{
private:
    const QString word;
    qreal word_x;

public:
    UnderscriptedWord(QString str, SubPhrase* c = nullptr);
    virtual void updateLayout() override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
};

}

#endif // UNDERSCRIPTEDWORD_H

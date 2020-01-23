#ifndef UNDERSCRIPTEDWORD_H
#define UNDERSCRIPTEDWORD_H

#include "construct.h"

namespace Typeset{

class UnderscriptedWord : public UnaryConstruct{
private:
    QGraphicsSimpleTextItem word;

public:
    UnderscriptedWord(QString str, SubPhrase* c = nullptr);
    virtual void updateTheme() override final;
    virtual void updateLayout() override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override final;
};

}

#endif // UNDERSCRIPTEDWORD_H

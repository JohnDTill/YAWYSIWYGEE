#ifndef PADDING_H
#define PADDING_H

#include "construct.h"

class Padding : public TerminalConstruct{
public:
    Padding(qreal pad);
    virtual void updateLayout() override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override final;
};

#endif // PADDING_H

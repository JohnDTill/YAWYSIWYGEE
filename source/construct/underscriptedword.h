#ifndef UNDERSCRIPTEDWORD_H
#define UNDERSCRIPTEDWORD_H

#include "construct.h"

class UnderscriptedWord : public UnaryConstruct{
private:
    const QString word;
    const QChar code;
    qreal word_x;

public:
    UnderscriptedWord(QString str, QChar code, SubPhrase* c = nullptr);
    virtual void updateLayout() override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
};

#endif // UNDERSCRIPTEDWORD_H

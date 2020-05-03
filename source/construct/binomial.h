#ifndef BINOMIAL_H
#define BINOMIAL_H

#include "construct.h"

class Binomial : public BinaryConstruct{
private:
    qreal h;
    static constexpr qreal paren_width = 6;

public:
    Binomial(SubPhrase* f = nullptr, SubPhrase* s = nullptr);
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
};

#endif // BINOMIAL_H

#ifndef TYPESETFRACTION_H
#define TYPESETFRACTION_H

#include "construct.h"

namespace Typeset{

class SubPhrase;

class Fraction : public BinaryConstruct{
private:
    static constexpr qreal bar_extension = 1;
    static constexpr qreal vspace = 2;

public:
    Fraction(SubPhrase* num = nullptr, SubPhrase* den = nullptr);
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override;

private:
    void calculateSize();
};

}

#endif // TYPESETFRACTION_H

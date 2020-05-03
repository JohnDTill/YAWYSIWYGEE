#ifndef LIMIT_H
#define LIMIT_H

#include "construct.h"

namespace Typeset{

class Limit : public BinaryConstruct{
private:
    qreal lx;
    qreal lh;
    qreal lw;
    qreal ax;
    qreal ay;
    qreal ah;
    qreal aw;

public:
    Limit(SubPhrase* var = nullptr, SubPhrase* to = nullptr);
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase*, qreal) const override final;
    virtual Text* textDown(const SubPhrase*, qreal) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override;
};

}

#endif // LIMIT_H

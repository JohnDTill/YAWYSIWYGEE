#ifndef TYPESETSCRIPT_H
#define TYPESETSCRIPT_H

#include "construct.h"
#include "subphrase.h"

namespace Typeset{

static constexpr qreal ratio_superscript = 1;
static constexpr qreal ratio_subscript = 1;

class Superscript : public BinaryConstruct{
public:
    Superscript(SubPhrase* child = nullptr, SubPhrase* superscript = nullptr);
    virtual void updateTheme() override final;
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase*, qreal) const override final;
    virtual Text* textDown(const SubPhrase*, qreal) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

class Subscript : public BinaryConstruct{
public:
    Subscript(SubPhrase* child = nullptr, SubPhrase* subscript = nullptr);
    virtual void updateTheme() override final;
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase*, qreal) const override final;
    virtual Text* textDown(const SubPhrase*, qreal) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

class Dualscript : public Construct{
private:
    SubPhrase* child;
    SubPhrase* subscript;
    SubPhrase* superscript;

public:
    Dualscript(SubPhrase* child = nullptr, SubPhrase* subscript = nullptr, SubPhrase* superscript = nullptr);
    virtual void deletePostorder() override final;
    virtual void updateTheme() override final;
    virtual void updateLayout() override final;
    virtual SubPhrase* front() const override final;
    virtual SubPhrase* back() const override final;
    virtual Text* textRight(const SubPhrase* caller) const override final;
    virtual Text* textLeft(const SubPhrase* caller) const override final;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

}

#endif // TYPESETSCRIPT_H

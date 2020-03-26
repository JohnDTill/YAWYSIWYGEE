#ifndef TYPESETSCRIPT_H
#define TYPESETSCRIPT_H

#include "construct.h"
#include "subphrase.h"

namespace Typeset{

static constexpr qreal ratio_superscript = 1;
static constexpr qreal ratio_subscript = 1;

class Superscript : public BinaryConstruct{
    Q_OBJECT

public:
    Superscript(SubPhrase* child = nullptr, SubPhrase* superscript = nullptr);
    virtual void updateLayout() override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual Text* textUp(const SubPhrase*, qreal) const override final;
    virtual Text* textDown(const SubPhrase*, qreal) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;

private slots:
    void addSubscript();
};

class Subscript : public BinaryConstruct{
    Q_OBJECT

public:
    Subscript(SubPhrase* child = nullptr, SubPhrase* subscript = nullptr);
    virtual void updateLayout() override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual Text* textUp(const SubPhrase*, qreal) const override final;
    virtual Text* textDown(const SubPhrase*, qreal) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override;

private slots:
    void addSuperscript();
};

class Dualscript : public Construct{
    Q_OBJECT

private:
    SubPhrase* child;
    SubPhrase* subscript;
    SubPhrase* superscript;

public:
    Dualscript(SubPhrase* child = nullptr, SubPhrase* subscript = nullptr, SubPhrase* superscript = nullptr);
    virtual void deletePostorder() override final;
    virtual void select() override final;
    virtual void updateLayout() override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual SubPhrase* front() const override final;
    virtual SubPhrase* back() const override final;
    virtual Text* textRight(const SubPhrase* caller) const override final;
    virtual Text* textLeft(const SubPhrase* caller) const override final;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;

private slots:
    void removeSuperscript();
    void removeSubscript();
};

}

#endif // TYPESETSCRIPT_H

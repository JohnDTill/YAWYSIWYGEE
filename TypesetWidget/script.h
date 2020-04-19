#ifndef TYPESETSCRIPT_H
#define TYPESETSCRIPT_H

#include "construct.h"
#include "subphrase.h"

namespace Typeset{

class Superscript : public UnaryConstruct{
    Q_OBJECT

private:
    qreal body_u;

public:
    Superscript(SubPhrase* superscript = nullptr);
    virtual void updateLayout() override final;
    virtual void notifyPrevUpdate() override final;
    virtual void notifyPrevPrevUpdate(Construct* c) override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;

private slots:
    void addSubscript();
};

class Subscript : public UnaryConstruct{
    Q_OBJECT

private:
    qreal body_d;

public:
    Subscript(SubPhrase* subscript = nullptr);
    virtual void updateLayout() override final;
    virtual void notifyPrevUpdate() override final;
    virtual void notifyPrevPrevUpdate(Construct* c) override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override;

private slots:
    void addSuperscript();
};

class Dualscript : public BinaryConstruct{
    Q_OBJECT

private:
    qreal body_u;
    qreal body_d;

public:
    Dualscript(SubPhrase* subscript = nullptr, SubPhrase* superscript = nullptr);
    virtual void updateLayout() override final;
    virtual void notifyPrevUpdate() override final;
    virtual void notifyPrevPrevUpdate(Construct* c) override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
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

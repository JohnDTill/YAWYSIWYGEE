#ifndef SUMMATION_H
#define SUMMATION_H

#include "construct.h"
#include <QUndoCommand>

namespace Typeset{

class BigQChar_S;
class BigQChar_SN;

class BigQChar : public TerminalConstruct{
    Q_OBJECT

private:
    const QChar ch;

public:
    BigQChar(QChar qchar);
    virtual void updateLayout() override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;

private slots:
    void addUnderscript();

private:
    class AddUnderscript : public QUndoCommand{
    private:
        bool active;
        BigQChar* out;
        BigQChar_S* in;

    public:
        AddUnderscript(BigQChar* out);
        ~AddUnderscript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };
};


class BigQChar_S : public UnaryConstruct{
    Q_OBJECT

private:
    const QChar ch;
    qreal symbol_x;

public:
    BigQChar_S(QChar qchar, SubPhrase* c = nullptr);
    virtual void updateLayout() override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
    friend BigQChar_SN;

private slots:
    void addOverscript();
    void removeUnderscript();

private:
    class AddOverscript : public QUndoCommand{
    private:
        bool active;
        BigQChar_S* out;
        BigQChar_SN* in;

    public:
        AddOverscript(BigQChar_S* out);
        ~AddOverscript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class RemoveUnderscript : public QUndoCommand{
    private:
        bool active;
        BigQChar_S* out;
        BigQChar* in;

    public:
        RemoveUnderscript(BigQChar_S* out);
        ~RemoveUnderscript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };
};


class BigQChar_SN : public BinaryConstruct{
    Q_OBJECT

private:
    const QChar ch;
    qreal symbol_x;
    qreal symbol_y;

public:
    BigQChar_SN(QChar qchar, SubPhrase* s = nullptr, SubPhrase* f = nullptr);
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
    friend BigQChar_S;

private slots:
    void removeOverscript();

private:
    class RemoveOverscript : public QUndoCommand{
    private:
        bool active;
        BigQChar_SN* out;
        BigQChar_S* in;

    public:
        RemoveOverscript(BigQChar_SN* out);
        ~RemoveOverscript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };
};

}

#endif // SUMMATION_H

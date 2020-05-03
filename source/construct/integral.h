#ifndef INTEGRAL_H
#define INTEGRAL_H

#include "construct.h"
#include <QUndoCommand>

class Integral_S;
class Integral_SN;

class Integral : public TerminalConstruct{
    Q_OBJECT

private:
    const QChar ch;
    const bool allow_superscript;

public:
    Integral(QChar qchar, bool allow_superscript = true);
    virtual void updateLayout() override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;

private slots:
    void addSubscript();

private:
    class AddSubscript : public QUndoCommand{
    private:
        bool active;
        Integral* out;
        Integral_S* in;

    public:
        AddSubscript(Integral* out);
        ~AddSubscript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };
};

class Integral_S : public UnaryConstruct{
    Q_OBJECT

private:
    const QChar ch;
    const bool allow_superscript;

public:
    Integral_S(QChar qchar, SubPhrase* subscript = nullptr, bool allow_superscript = true);
    virtual void updateLayout() override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
    friend Integral_SN;

private slots:
    void addSuperscript();
    void removeSubscript();

private:
    class AddSuperscript : public QUndoCommand{
    private:
        bool active;
        Integral_S* out;
        Integral_SN* in;

    public:
        AddSuperscript(Integral_S* out);
        ~AddSuperscript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class RemoveSubscript : public QUndoCommand{
    private:
        bool active;
        Integral_S* out;
        Integral* in;

    public:
        RemoveSubscript(Integral_S* out);
        ~RemoveSubscript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };
};

class Integral_SN : public BinaryConstruct{
    Q_OBJECT

private:
    const QChar ch;
    qreal symbol_y;

public:
    Integral_SN(QChar qchar, SubPhrase* subscript = nullptr, SubPhrase* superscript = nullptr);
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
    friend Integral_S;

private slots:
    void removeSuperscript();

private:
    class RemoveSuperscript : public QUndoCommand{
    private:
        bool active;
        Integral_SN* out;
        Integral_S* in;

    public:
        RemoveSuperscript(Integral_SN* out);
        ~RemoveSuperscript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };
};

#endif // INTEGRAL_H

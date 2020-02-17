#ifndef TYPESETROOT_H
#define TYPESETROOT_H

#include "construct.h"
#include "subphrase.h"
#include <QUndoCommand>

namespace Typeset{

class ScriptedRoot;

static constexpr qreal slope = 4;
static constexpr qreal hspace = 1;
static constexpr qreal extra_height = 1;
static constexpr qreal overbar_extension = 1;
static constexpr qreal x1 = 1;
static constexpr qreal x2 = 4;

class SquareRoot : public UnaryConstruct{
    Q_OBJECT

private:
    qreal height;

public:
    SquareRoot(SubPhrase* c = nullptr);
    virtual void updateLayout() override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* caller = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*options, QWidget*) override final;
    friend ScriptedRoot;

private:
    void calculateSize();

private slots:
    void addScript();

private:
    class AddScript : public QUndoCommand{
    private:
        bool active;
        SquareRoot* out;
        ScriptedRoot* in;

    public:
        AddScript(SquareRoot* out);
        ~AddScript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };
};

class ScriptedRoot : public BinaryConstruct{
    Q_OBJECT

private:
    qreal height;
    qreal xr;
    qreal yr;
    static constexpr qreal script_hoffset = -3;
    static constexpr qreal script_ratio = 1;

public:
    ScriptedRoot(SubPhrase* child = nullptr, SubPhrase* power = nullptr);
    virtual void updateLayout() override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* caller = nullptr) override final;
    virtual Text* textUp(const SubPhrase* caller, qreal) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal) const override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
    friend SquareRoot;

private slots:
    void removeScript();

private:
    class RemoveScript : public QUndoCommand{
    private:
        bool active;
        ScriptedRoot* out;
        SquareRoot* in;

    public:
        RemoveScript(ScriptedRoot* out);
        ~RemoveScript();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };
};

}

#endif // TYPESETROOT_H

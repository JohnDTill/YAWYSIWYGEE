#ifndef CASES_H
#define CASES_H

#include "construct.h"
#include <QUndoCommand>

namespace Typeset{

class Cases : public NaryConstruct{
    Q_OBJECT

private:
    qreal max_val_w;
    static constexpr qreal vspace = 2;
    static constexpr qreal hspace = 12;
    static constexpr qreal bracket_width = 16;
    static constexpr qreal bwh = bracket_width/2;

    major_integer active;

public:
    Cases(const std::vector<SubPhrase*>& data);
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* caller = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;

private:
    void insertRow(major_integer row, SubPhrase* first, SubPhrase* second);
    void removeRow(major_integer row);

    class AddRow : public QUndoCommand{
    private:
        bool active;
        Cases& c;
        const major_integer row;
        std::vector<SubPhrase*> data;

    public:
        AddRow(Cases& c, major_integer row);
        ~AddRow();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class RemoveRow : public QUndoCommand{
    private:
        bool active;
        Cases& c;
        const major_integer row;
        std::vector<SubPhrase*> data;

    public:
        RemoveRow(Cases& c, major_integer row);
        ~RemoveRow();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

private slots:
    void createRowBelow();
    void createRowAbove();
    void deleteRow();
};

}

#endif // CASES_H

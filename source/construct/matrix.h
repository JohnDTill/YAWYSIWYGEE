#ifndef TYPESETBRACKETS_H
#define TYPESETBRACKETS_H

#include "construct.h"
#include <QUndoCommand>

namespace Typeset{

class Matrix : public NaryConstruct{
    Q_OBJECT

private:
    minor_integer rows;
    minor_integer cols;
    qreal h;

    static constexpr qreal hspace = 5;
    static constexpr qreal vspace = 2;
    static constexpr qreal text_hoffset = 3;
    static constexpr qreal bracket_width = 2;
    static constexpr qreal bracket_hoffset = 1.5;
    static constexpr qreal bracket_voffset = 1;

    uint32_t active;

public:
    Matrix(const std::vector<SubPhrase*>& c, minor_integer rows, minor_integer cols);
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* caller = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;

private:
    major_integer getFlatIndex(major_integer row, major_integer col) const;
    major_integer getRow(major_integer flat_index) const;
    major_integer getCol(major_integer flat_index) const;
    void insertRow(major_integer row, const std::vector<SubPhrase*> inserted);
    void removeRow(major_integer row);
    void insertCol(major_integer col, const std::vector<SubPhrase*> inserted);
    void removeCol(major_integer col);

    class AddRow : public QUndoCommand{
    private:
        bool active;
        Matrix& mat;
        const major_integer row;
        std::vector<SubPhrase*> data;

    public:
        AddRow(Matrix& mat, major_integer row);
        ~AddRow();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class AddCol : public QUndoCommand{
    private:
        bool active;
        Matrix& mat;
        const major_integer col;
        std::vector<SubPhrase*> data;

    public:
        AddCol(Matrix& mat, major_integer col);
        ~AddCol();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class RemoveRow : public QUndoCommand{
    private:
        bool active;
        Matrix& mat;
        const major_integer row;
        std::vector<SubPhrase*> data;

    public:
        RemoveRow(Matrix& mat, major_integer row);
        ~RemoveRow();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class RemoveCol : public QUndoCommand{
    private:
        bool active;
        Matrix& mat;
        const major_integer col;
        std::vector<SubPhrase*> data;

    public:
        RemoveCol(Matrix& mat, major_integer col);
        ~RemoveCol();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

private slots:
    void createRowBelow();
    void createRowAbove();
    void createColRight();
    void createColLeft();
    void deleteRow();
    void deleteCol();
};

}

#endif // TYPESETBRACKETS_H

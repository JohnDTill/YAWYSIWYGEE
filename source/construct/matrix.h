#ifndef TYPESETBRACKETS_H
#define TYPESETBRACKETS_H

#include "construct.h"
#include <QUndoCommand>

class Matrix : public NaryConstruct{
    Q_OBJECT

private:
    uint8_t rows;
    uint8_t cols;
    qreal h;

    static constexpr qreal hspace = 5;
    static constexpr qreal vspace = 2;
    static constexpr qreal text_hoffset = 3;
    static constexpr qreal bracket_width = 2;
    static constexpr qreal bracket_hoffset = 1.5;
    static constexpr qreal bracket_voffset = 1;

    uint32_t active;

public:
    Matrix(const std::vector<SubPhrase*>& c, uint8_t rows, uint8_t cols);
    virtual void updateLayout() override final;
    virtual Text* textUp(const SubPhrase* caller, qreal x) const override final;
    virtual Text* textDown(const SubPhrase* caller, qreal x) const override final;
    virtual void populateMenu(QMenu& menu, const SubPhrase* caller = nullptr) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;

private:
    uint16_t getFlatIndex(uint16_t row, uint16_t col) const;
    uint16_t getRow(uint16_t flat_index) const;
    uint16_t getCol(uint16_t flat_index) const;
    void insertRow(uint16_t row, const std::vector<SubPhrase*> inserted);
    void removeRow(uint16_t row);
    void insertCol(uint16_t col, const std::vector<SubPhrase*> inserted);
    void removeCol(uint16_t col);

    class AddRow : public QUndoCommand{
    private:
        bool active;
        Matrix& mat;
        const uint16_t row;
        std::vector<SubPhrase*> data;

    public:
        AddRow(Matrix& mat, uint16_t row);
        ~AddRow();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class AddCol : public QUndoCommand{
    private:
        bool active;
        Matrix& mat;
        const uint16_t col;
        std::vector<SubPhrase*> data;

    public:
        AddCol(Matrix& mat, uint16_t col);
        ~AddCol();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class RemoveRow : public QUndoCommand{
    private:
        bool active;
        Matrix& mat;
        const uint16_t row;
        std::vector<SubPhrase*> data;

    public:
        RemoveRow(Matrix& mat, uint16_t row);
        ~RemoveRow();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class RemoveCol : public QUndoCommand{
    private:
        bool active;
        Matrix& mat;
        const uint16_t col;
        std::vector<SubPhrase*> data;

    public:
        RemoveCol(Matrix& mat, uint16_t col);
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

#endif // TYPESETBRACKETS_H

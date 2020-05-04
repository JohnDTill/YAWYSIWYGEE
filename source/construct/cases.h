#ifndef CASES_H
#define CASES_H

#include "construct.h"
#include <QUndoCommand>

class Cases : public NaryConstruct{
    Q_OBJECT

private:
    qreal max_val_w;
    static constexpr qreal vspace = 2;
    static constexpr qreal hspace = 12;
    static constexpr qreal bracket_width = 16;
    static constexpr qreal bwh = bracket_width/2;

    uint16_t active;

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
    void insertRow(uint16_t row, SubPhrase* first, SubPhrase* second);
    void removeRow(uint16_t row);

    class AddRow : public QUndoCommand{
    private:
        bool active;
        Cases& c;
        const uint16_t row;
        std::vector<SubPhrase*> data;

    public:
        AddRow(Cases& c, uint16_t row);
        ~AddRow();

    protected:
        virtual void redo() final;
        virtual void undo() final;
    };

    class RemoveRow : public QUndoCommand{
    private:
        bool active;
        Cases& c;
        const uint16_t row;
        std::vector<SubPhrase*> data;

    public:
        RemoveRow(Cases& c, uint16_t row);
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

#endif // CASES_H

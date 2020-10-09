#ifndef TYPESETLINE_H
#define TYPESETLINE_H

#include "phrase.h"

class Line : public Phrase{
public:
    Line* prev = nullptr;
    Line* next = nullptr;
    uint32_t line_num;

private:
    static constexpr qreal linebox_hspace = 20;

public:
    Line(Text* f = nullptr, Text* b = nullptr, uint32_t line_num = 1);
    void repositionNextLine();
    void renumberLine(uint32_t n);
    virtual void updateToTop() override final;
    virtual Text* textRight() const override final;
    virtual Text* textLeft() const override final;
    virtual Text* textRightUp() const override final;
    virtual Text* textLeftUp() const override final;
    virtual Text* textUp(qreal x) const override final;
    virtual Text* textDown(qreal x) const override final;
    virtual bool isLine() const override final;
    virtual Line& getLine() override final;
    bool preceeds(Line& l);
    bool preceedsInclusive(Line& l);
    virtual void populateMenu(QMenu&) override final;
    virtual void write(QTextStream& out) const override final;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
    virtual QRectF boundingRect() const override final;
};

void link(Line* a, Line* b);

#endif // TYPESETLINE_H

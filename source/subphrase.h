#ifndef TYPESETPHRASE_H
#define TYPESETPHRASE_H

#include "phrase.h"
class Construct;

class SubPhrase : public Phrase{
public:
    int type() const override {return UserType + 2;}
    Construct* parent;
    uint16_t child_id; //So this subphrase can communicate with its parent

private:
    static constexpr int empty_box_height = 19;
    static constexpr int empty_box_width = 12;
    static constexpr qreal padding = 1;

public:
    SubPhrase(Text* f = nullptr, Text* b = nullptr, uint16_t child_id = 0);
    void setParentConstruct(Construct& c);
    virtual void updateToTop() override final;
    virtual Text* textRight() const override final;
    virtual Text* textLeft() const override final;
    virtual Text* textRightUp() const override final;
    virtual Text* textLeftUp() const override final;
    virtual Text* textUp(qreal x) const override final;
    virtual Text* textDown(qreal x) const override final;
    virtual bool isLine() const override final;
    virtual Line& getLine() override final;
    virtual void populateMenu(QMenu& menu) override final;
    virtual void write(QTextStream& out) const override final;
    bool isEmpty() const;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override final;
    virtual QRectF boundingRect() const override final;
    virtual void calculateSize() override final;
};

#endif // TYPESETPHRASE_H

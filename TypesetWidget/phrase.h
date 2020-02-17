#ifndef PHRASE_H
#define PHRASE_H

#include <QGraphicsItem>
#include <QTextStream>

namespace Typeset{

class Cursor;
class Line;
class Text;

class Phrase : public QGraphicsItem{
public:
    Text* front;
    Text* back;
    qreal w; //width
    qreal u; //distance from horizontal to top
    qreal d; //distance from horizontal to bottom

private:
    static constexpr qreal padding_between_elements = 2.5;

public:
    Phrase(Text* f = nullptr, Text* b = nullptr);
    qreal h() const;
    void select();
    void deletePostorder();
    void updateLayout();
    virtual void updateToTop() = 0;
    virtual Text* textRight() const = 0;
    virtual Text* textLeft() const = 0;
    virtual Text* textRightUp() const = 0;
    virtual Text* textLeftUp() const = 0;
    virtual Text* textUp(qreal x) const = 0;
    virtual Text* textDown(qreal x) const = 0;
    virtual bool isLine() const = 0;
    virtual Line& getLine() = 0;
    virtual void populateMenu(QMenu& menu) = 0;
    virtual void write(QTextStream& out) const = 0;

protected:
    virtual void calculateSize();
    void writeContents(QTextStream& out) const;

private:
    void initializeChildren();
};

}

#endif // PHRASE_H

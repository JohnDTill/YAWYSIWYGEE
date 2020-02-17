#ifndef TYPESETTEXT_H
#define TYPESETTEXT_H

#include <QFont>
#include <QFontDatabase>
#include <QGraphicsTextItem>
#include <QTextStream>
#include <QUndoCommand>

namespace Typeset{

class Construct;
class Document;
class Phrase;

class Text : public QGraphicsTextItem{

public:
    Phrase* parent;
    Construct* next;
    Construct* prev;
    qreal w; //width
    qreal u; //distance from horizontal to top- equal to half height for Text

private:
    static constexpr qreal margin = 1;
    static constexpr qreal subtract_from_height = 2.5;
    static constexpr qreal subtract_from_width = 1;

public:
    Text(uint8_t script_level = 0, QString str = "");
    bool isEmpty() const;
    void setParentPhrase(Phrase* parent);
    static bool isDeepestScriptLevel(uint8_t script_level);
    void updateToTop();
    void populateMenu(QMenu& menu);
    void write(QTextStream& out) const;
    uint8_t getScriptLevel() const;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    void calculateSize();
    Document* typesetDocument() const;
};

}

#endif // TYPESETTEXT_H

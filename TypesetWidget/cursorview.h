#ifndef CURSORVIEW_H
#define CURSORVIEW_H

#include <QGraphicsLineItem>

namespace Typeset{

class Cursor;
class Document;
class Line;
class Text;

class CursorView{
private:
    std::list<QGraphicsItem*> masks;
    static constexpr qreal newline_padding = 2;
    Text* tL_old;
    Text* tR_old;

public:
    CursorView(Document& doc);
    void update(const Cursor& cursor);

private:
    void addMasks(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    void addMasksText(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    void addMasksPhrase(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    void addMasksMultiline(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);

    class SelectionMask : public QGraphicsItem{
        private:
            QRectF region;
            static constexpr qreal margin = 2;

        public:
            SelectionMask(const QRectF& r);

        protected:
            virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*option, QWidget*) override final;
            virtual QRectF boundingRect() const override final;
        };
};

}

#endif // CURSORVIEW_H

#ifndef CURSORVIEW_H
#define CURSORVIEW_H

#include <list>
#include <QtGlobal>
class Cursor;
class TypesetScene;
class Line;
class Text;
class QGraphicsItem;
class QTextCursor;

class CursorView{
private:
    std::list<QGraphicsItem*> masks;
    static constexpr qreal newline_padding = 2;
    Text* tL_old;
    Text* tR_old;

public:
    CursorView(TypesetScene& doc);
    void update(const Cursor& cursor);

private:
    void addMasks(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    void addMasksText(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    void addMasksPhrase(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    void addMasksMultiline(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
};

#endif // CURSORVIEW_H

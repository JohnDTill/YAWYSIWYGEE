#ifndef ERRORVIEW_H
#define ERRORVIEW_H

#include <list>
#include <QtGlobal>
class Cursor;
class TypesetScene;
class Line;
class Text;
class QGraphicsItem;
class QTextCursor;

class ErrorView{
private:
    std::list<QGraphicsItem*> masks;
    static constexpr qreal newline_padding = 2;

public:
    void clear();
    void reportError(Text* tL, int pL, Text* tR, int pR, const QString& msg);

private:
    void addMasksText(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR, const QString& msg);
    void addMasksPhrase(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR, const QString& msg);
    void addMasksMultiline(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR, const QString& msg);
};

#endif // ErrorView_H

#ifndef COMMANDINSERTTEXT_H
#define COMMANDINSERTTEXT_H

#include <QUndoCommand>
class Cursor;
class Text;
class QTextCursor;

class TextCommand : public QUndoCommand{
private:
    const bool is_insertion;
    Cursor& cursor;
    Text* t;
    const QString str;
    const int pL;
    const int pR;

public:
    static QUndoCommand* insert(Cursor& cursor, const QString& str, Text* t, QTextCursor c);
    static QUndoCommand* remove(Cursor& cursor, Text* t, QTextCursor c, QTextCursor ac);

private:
    TextCommand(Cursor& cursor, const QString& str, Text* t, QTextCursor c);
    TextCommand(Cursor& cursor, Text* t, const QString& str, int pL, int pR);
    TextCommand(Cursor& cursor, Text* t, QTextCursor cL, QTextCursor cR);

protected:
    virtual void redo() final;
    virtual void undo() final;

private:
    void insert();
    void remove();
};

#endif // COMMANDINSERTTEXT_H

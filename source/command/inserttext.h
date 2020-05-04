#ifndef COMMANDINSERTTEXT_H
#define COMMANDINSERTTEXT_H

#include <QUndoCommand>
#include <QTextCursor>
class Cursor;
class Text;

class InsertText : public QUndoCommand{
private:
    Cursor& cursor;
    const QString str;
    Text* t;
    QTextCursor c;

public:
    InsertText(Cursor& cursor, const QString& str, Text* t, QTextCursor c);

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDINSERTTEXT_H

#ifndef COMMANDINSERTCHAR_H
#define COMMANDINSERTCHAR_H

#include <QUndoCommand>
#include <QTextCursor>
class Cursor;
class Text;

class InsertChar : public QUndoCommand{
public:
    int pR;
    Text* t;

private:
    Cursor& cursor;
    QTextCursor c;
    QString str;
    int pL;

public:
    InsertChar(Cursor& cursor, QChar ch, Text* t, QTextCursor c);
    void addChar(QChar ch);

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDINSERTCHAR_H

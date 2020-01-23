#ifndef COMMANDINSERTCHAR_H
#define COMMANDINSERTCHAR_H

#include <QUndoCommand>

#include "text.h"
#include <QTextCursor>

namespace Typeset{

class Cursor;

class CommandInsertChar : public QUndoCommand{
public:
    int pR;
    Text* t;

private:
    Cursor& cursor;
    QTextCursor c;
    QString str;
    int pL;

public:
    CommandInsertChar(Cursor& cursor, QChar ch, Text* t, QTextCursor c);
    void addChar(QChar ch);

protected:
    virtual void redo() final;
    virtual void undo() final;
};

}

#endif // COMMANDINSERTCHAR_H

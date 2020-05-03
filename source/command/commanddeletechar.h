#ifndef COMMANDDELETECHAR_H
#define COMMANDDELETECHAR_H

#include <QUndoCommand>

#include "text.h"
#include <QTextCursor>

class Cursor;

class CommandDeleteChar : public QUndoCommand{
public:
    int pL;
    int pR;
    Text* t;

private:
    Cursor& cursor;
    QTextCursor c;
    QString str;

public:
    CommandDeleteChar(Cursor& cursor, bool forward, Text* t, QTextCursor c);
    void removeChar(bool forward);

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDDELETECHAR_H

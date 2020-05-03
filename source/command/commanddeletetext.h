#ifndef COMMANDDELETETEXT_H
#define COMMANDDELETETEXT_H

#include <QUndoCommand>

#include "text.h"
#include <QTextCursor>

class Cursor;

class CommandDeleteText : public QUndoCommand{
private:
    Cursor& cursor;
    Text* t;
    QTextCursor c;
    QString str;
    int pL;
    int pR;

public:
    CommandDeleteText(Cursor& cursor, Text* t, QTextCursor cL, QTextCursor cR);

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDDELETETEXT_H

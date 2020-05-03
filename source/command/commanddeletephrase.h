#ifndef COMMANDDELETE_H
#define COMMANDDELETE_H

#include <QUndoCommand>

#include "text.h"
#include <QTextCursor>

class Cursor;

class CommandDeletePhrase : public QUndoCommand{
private:
    bool active;
    Cursor& cursor;
    Text* tL;
    QTextCursor cL;
    Text* tR;
    QTextCursor cR;
    Construct* tL_next;
    const QString removed_str;
    const QString append_str;
    int pL;

public:
    CommandDeletePhrase(Cursor& cursor, Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    ~CommandDeletePhrase();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDDELETE_H

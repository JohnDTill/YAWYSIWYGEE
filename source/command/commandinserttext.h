#ifndef COMMANDINSERTTEXT_H
#define COMMANDINSERTTEXT_H

#include <QUndoCommand>

#include "text.h"
#include <QTextCursor>

namespace Typeset{

class Cursor;

class CommandInsertText : public QUndoCommand{
private:
    Cursor& cursor;
    const QString str;
    Text* t;
    QTextCursor c;

public:
    CommandInsertText(Cursor& cursor, const QString& str, Text* t, QTextCursor c);

protected:
    virtual void redo() final;
    virtual void undo() final;
};

}

#endif // COMMANDINSERTTEXT_H

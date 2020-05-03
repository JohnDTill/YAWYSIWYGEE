#ifndef COMMANDDELETEMULTILINE_H
#define COMMANDDELETEMULTILINE_H

#include <QUndoCommand>

#include "text.h"
#include "line.h"
#include <QTextCursor>

class Cursor;

class CommandDeleteMultiline : public QUndoCommand{
private:
    bool active;
    Cursor& cursor;
    TypesetScene& doc;
    Text* tL;
    Line* lL;
    QTextCursor cL;
    Text* tR;
    Line* lR;
    QTextCursor cR;
    Construct* tL_next;
    Construct* tR_next;
    Line* lL_next;
    const QString removed_str;
    const QString append_str;
    int pL;

public:
    CommandDeleteMultiline(Cursor& cursor, TypesetScene& doc, Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    ~CommandDeleteMultiline();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDDELETEMULTILINE_H

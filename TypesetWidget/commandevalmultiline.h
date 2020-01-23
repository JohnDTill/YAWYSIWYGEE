#ifndef COMMANDEVALMULTILINE_H
#define COMMANDEVALMULTILINE_H

#include <QUndoCommand>

#include "line.h"
#include "text.h"
#include <QTextCursor>

namespace Typeset{

class Cursor;

class CommandEvalMultiline : public QUndoCommand{
private:
    Cursor& cursor;
    Document& doc;
    bool active;
    Line* lL;
    Text* tL;
    QTextCursor cL;
    QString append_str;
    QString removed_str;
    Construct* tL_next;
    Line* lL_next;
    Line* lR;
    Text* tR;
    QTextCursor cR;

public:
    CommandEvalMultiline(Cursor& cursor, Document& doc, const QString& source, Text* t, QTextCursor c);
    ~CommandEvalMultiline();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

}

#endif // COMMANDEVALUMULTILINE_H

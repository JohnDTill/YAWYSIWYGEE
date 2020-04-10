#ifndef COMMANDEVALPHRASE_H
#define COMMANDEVALPHRASE_H

#include <QUndoCommand>

#include "text.h"
#include <QTextCursor>

namespace Typeset{

class Cursor;

class CommandEvalPhrase : public QUndoCommand{
private:
    Cursor& cursor;
    bool active;
    Text* tL;
    QTextCursor cL;
    QString append_str;
    QString removed_str;
    Construct* tL_next;
    Text* tR;
    QTextCursor cR;
    int pL;

public:
    CommandEvalPhrase(Cursor& cursor, const QString& source, Text* t, QTextCursor c);
    ~CommandEvalPhrase();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

}

#endif // COMMANDEVALPHRASE_H

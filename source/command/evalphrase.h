#ifndef COMMANDEVALPHRASE_H
#define COMMANDEVALPHRASE_H

#include <QUndoCommand>
#include <QTextCursor>
class Construct;
class Cursor;
class Text;

class EvalPhrase : public QUndoCommand{
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
    EvalPhrase(Cursor& cursor, const QString& source, Text* t, QTextCursor c);
    ~EvalPhrase();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDEVALPHRASE_H

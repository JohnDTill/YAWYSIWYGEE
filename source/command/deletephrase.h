#ifndef COMMANDDELETE_H
#define COMMANDDELETE_H

#include <QUndoCommand>
#include <QTextCursor>
class Construct;
class Cursor;
class Text;

class DeletePhrase : public QUndoCommand{
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
    DeletePhrase(Cursor& cursor, Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    ~DeletePhrase();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDDELETE_H

#ifndef COMMANDDELETETEXT_H
#define COMMANDDELETETEXT_H

#include <QUndoCommand>
#include <QTextCursor>
class Cursor;
class Text;

class DeleteText : public QUndoCommand{
private:
    Cursor& cursor;
    Text* t;
    QTextCursor c;
    QString str;
    int pL;
    int pR;

public:
    DeleteText(Cursor& cursor, Text* t, QTextCursor cL, QTextCursor cR);

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDDELETETEXT_H

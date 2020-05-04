#ifndef COMMANDDELETECHAR_H
#define COMMANDDELETECHAR_H

#include <QUndoCommand>
#include <QTextCursor>
class Cursor;
class Text;

class DeleteChar : public QUndoCommand{
public:
    int pL;
    int pR;
    Text* t;

private:
    Cursor& cursor;
    QTextCursor c;
    QString str;

public:
    DeleteChar(Cursor& cursor, bool forward, Text* t, QTextCursor c);
    void removeChar(bool forward);

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDDELETECHAR_H

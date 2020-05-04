#ifndef COMMANDDELETEMULTILINE_H
#define COMMANDDELETEMULTILINE_H

#include <QUndoCommand>
#include <QTextCursor>
class Construct;
class Cursor;
class Line;
class Text;
class TypesetScene;

class DeleteMultiline : public QUndoCommand{
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
    DeleteMultiline(Cursor& cursor, TypesetScene& doc, Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    ~DeleteMultiline();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDDELETEMULTILINE_H

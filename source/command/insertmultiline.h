#ifndef COMMANDINSERTMULTILINE_H
#define COMMANDINSERTMULTILINE_H

#include <QUndoCommand>
#include <QTextCursor>
class Cursor;
class Construct;
class Line;
class Text;
class TypesetScene;

class InsertMultiline : public QUndoCommand{
private:
    Cursor& cursor;
    TypesetScene& doc;
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
    int pL;

public:
    InsertMultiline(Cursor& cursor, TypesetScene& doc, const QString& source, Text* t, QTextCursor c);
    ~InsertMultiline();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDINSERTMULTILINE_H

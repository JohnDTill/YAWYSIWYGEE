#ifndef COMMANDEVALMULTILINE_H
#define COMMANDEVALMULTILINE_H

#include <QUndoCommand>
#include <QTextCursor>
class Construct;
class Cursor;
class Line;
class Text;
class TypesetScene;

class EvalMultiline : public QUndoCommand{
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
    EvalMultiline(Cursor& cursor, TypesetScene& doc, const QString& source, Text* t, QTextCursor c);
    ~EvalMultiline();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDEVALUMULTILINE_H

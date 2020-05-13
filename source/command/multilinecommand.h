#ifndef COMMANDEVALMULTILINE_H
#define COMMANDEVALMULTILINE_H

#include <QUndoCommand>
class Construct;
class Cursor;
class Line;
class Text;
class TypesetScene;
class QTextCursor;

class MultilineCommand : public QUndoCommand{
private:
    const bool is_insertion;
    Cursor& cursor;
    TypesetScene& doc;
    bool owns_data;
    Text* t;
    Construct* cb;
    Line* la;
    Text* e;
    Line* lb;
    QString str_ti;
    Construct* ci;
    Text* ei;
    Line* ln;
    Line* lf;
    Text* ef;
    const int l_a;
    const int l_b;

public:
    static QUndoCommand* insert(Cursor& cursor, TypesetScene& doc, const QString& source, Text* t, QTextCursor c);
    static QUndoCommand* eval(Cursor& cursor, TypesetScene& doc, const QString& source, Text* t, QTextCursor c);
    static QUndoCommand* remove(Cursor& cursor, TypesetScene& doc, Text* t, QTextCursor c, Text* at, QTextCursor ac);

private:
    MultilineCommand(Cursor& cursor, TypesetScene& doc, Line* li, Line* lf, Text* t, QTextCursor c);
    MultilineCommand(Cursor& cursor, TypesetScene& doc, Text* tL, QTextCursor cL, Text* tR, QTextCursor cR);
    ~MultilineCommand();

protected:
    virtual void redo() final;
    virtual void undo() final;

private:
    void insert();
    void remove();
};

#endif // COMMANDEVALUMULTILINE_H

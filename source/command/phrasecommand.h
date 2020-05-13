#ifndef COMMANDEVALPHRASE_H
#define COMMANDEVALPHRASE_H

#include <QUndoCommand>
class Construct;
class Cursor;
class Text;
class QTextCursor;

class PhraseCommand : public QUndoCommand{
private:
    const bool is_insertion;
    Cursor& cursor;
    bool owns_data;
    Text* t;
    Construct* cb;
    const int l_a;
    const int l_b;
    Text* ei;
    Construct* ci;
    QString str_ti;

public:
    static QUndoCommand* eval(Cursor& cursor, const QString& source, Text* t, QTextCursor c);
    static QUndoCommand* remove(Cursor& cursor, Text* text, QTextCursor c, Text* anchor_text, QTextCursor ac);

private:
    PhraseCommand(Cursor& cursor, const QString& source, Text* t, QTextCursor c);
    PhraseCommand(const bool is_insertion, Cursor& cursor, Text* t, QTextCursor cL, Text* ei, QTextCursor cR);
    ~PhraseCommand();

protected:
    virtual void redo() final;
    virtual void undo() final;

private:
    void eval();
    void remove();
};

#endif // COMMANDEVALPHRASE_H

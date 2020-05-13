#include "textcommand.h"

#include "../cursor.h"
#include "../text.h"

QUndoCommand* TextCommand::insert(Cursor& cursor, const QString& str, Text* t, QTextCursor c){
    return new TextCommand(cursor, str, t, c);
}

QUndoCommand* TextCommand::remove(Cursor& cursor, Text* t, QTextCursor c, QTextCursor ac){
    Q_ASSERT(c.position() != ac.position());

    int p = c.position();
    int ap = ac.position();
    c.setPosition(ap, QTextCursor::KeepAnchor);
    QString str = c.selectedText();

    if(p > ap) return new TextCommand(cursor, t, str, ap, p);
    else return new TextCommand(cursor, t, str, p, ap);
}

TextCommand::TextCommand(Cursor& cursor, const QString& str, Text* t, QTextCursor c)
    : is_insertion(true),
      cursor(cursor),
      t(t),
      str(str),
      pL(c.position()),
      pR(pL + str.length()) {}

TextCommand::TextCommand(Cursor& cursor, Text* t, const QString& str, int pL, int pR)
    : is_insertion(false),
      cursor(cursor),
      t(t),
      str(str),
      pL(pL),
      pR(pR) {}

void TextCommand::redo(){
    is_insertion ? insert() : remove();
}

void TextCommand::undo(){
    is_insertion ? remove() : insert();
}

void TextCommand::insert(){
    QTextCursor c = t->textCursor();
    c.setPosition(pL);
    c.insertText(str);
    t->updateToTop();
    cursor.setPosition(*t, c);
}

void TextCommand::remove(){
    QTextCursor c = t->textCursor();
    c.setPosition(pL);
    c.setPosition(c.position() + str.length(), QTextCursor::KeepAnchor);
    c.removeSelectedText();
    t->updateToTop();
    cursor.setPosition(*t, c);
}

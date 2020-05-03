#include "commandinsertchar.h"

#include "cursor.h"

CommandInsertChar::CommandInsertChar(Cursor& cursor, QChar ch, Text* t, QTextCursor c)
    : t(t),
      cursor(cursor),
      c(c) {
    setText("C");
    str = ch;
    pL = c.position();
    pR = pL + 1;
}

void CommandInsertChar::addChar(QChar ch){
    c.setPosition(pR);
    c.insertText(ch);
    str += ch;
    t->updateToTop();
    cursor.setPosition(*t, c);
    pR++;
}

void CommandInsertChar::redo(){
    c.setPosition(pL);
    c.insertText(str);
    t->updateToTop();
    cursor.setPosition(*t, c);
}

void CommandInsertChar::undo(){
    c.setPosition(pL);
    c.setPosition(pR, QTextCursor::KeepAnchor);
    c.removeSelectedText();
    t->updateToTop();
    cursor.setPosition(*t, c);
}

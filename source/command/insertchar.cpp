#include "insertchar.h"

#include "../cursor.h"
#include "../text.h"

InsertChar::InsertChar(Cursor& cursor, QChar ch, Text* t, QTextCursor c)
    : t(t),
      cursor(cursor),
      c(c) {
    setText("C");
    str = ch;
    pL = c.position();
    pR = pL + 1;
}

void InsertChar::addChar(QChar ch){
    c.setPosition(pR);
    c.insertText(ch);
    str += ch;
    t->updateToTop();
    cursor.setPosition(*t, c);
    pR++;
}

void InsertChar::redo(){
    c.setPosition(pL);
    c.insertText(str);
    t->updateToTop();
    cursor.setPosition(*t, c);
}

void InsertChar::undo(){
    c.setPosition(pL);
    c.setPosition(pR, QTextCursor::KeepAnchor);
    c.removeSelectedText();
    t->updateToTop();
    cursor.setPosition(*t, c);
}

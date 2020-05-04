#include "inserttext.h"

#include "../cursor.h"
#include "../text.h"

InsertText::InsertText(Cursor& cursor, const QString& str, Text* t, QTextCursor c)
    : cursor(cursor),
      str(str),
      t(t),
      c(c) {
    //DO NOTHING
}

void InsertText::redo(){
    c.insertText(str);
    t->updateToTop();
    cursor.setPosition(*t, c);
}

void InsertText::undo(){
    c.setPosition(c.position() - str.length(), QTextCursor::KeepAnchor);
    c.removeSelectedText();
    t->updateToTop();
    cursor.setPosition(*t, c);
}

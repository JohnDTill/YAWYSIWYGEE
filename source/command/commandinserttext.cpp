#include "commandinserttext.h"

#include "cursor.h"

CommandInsertText::CommandInsertText(Cursor& cursor, const QString& str, Text* t, QTextCursor c)
    : cursor(cursor),
      str(str),
      t(t),
      c(c) {
    //DO NOTHING
}

void CommandInsertText::redo(){
    c.insertText(str);
    t->updateToTop();
    cursor.setPosition(*t, c);
}

void CommandInsertText::undo(){
    c.setPosition(c.position() - str.length(), QTextCursor::KeepAnchor);
    c.removeSelectedText();
    t->updateToTop();
    cursor.setPosition(*t, c);
}

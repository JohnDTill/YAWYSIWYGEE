#include "deletetext.h"

#include "../cursor.h"
#include "../text.h"

DeleteText::DeleteText(Cursor& cursor, Text* t, QTextCursor cL, QTextCursor cR)
    : cursor(cursor),
      t(t) {
    pL = cL.position();
    pR = cR.position();
    c = cL;
    c.setPosition(cR.position(), QTextCursor::KeepAnchor);
    str = c.selectedText();
    c = cL;
}

void DeleteText::redo(){
    c.setPosition(pL);
    c.setPosition(c.position() + str.length(), QTextCursor::KeepAnchor);
    c.removeSelectedText();
    t->updateToTop();
    cursor.setPosition(*t, c);
}

void DeleteText::undo(){
    c.setPosition(pL);
    c.insertText(str);
    t->updateToTop();
    cursor.setPosition(*t, c);
    c.setPosition(c.position() - str.length());
}

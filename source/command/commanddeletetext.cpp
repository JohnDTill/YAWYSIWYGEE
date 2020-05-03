#include "commanddeletetext.h"

#include "cursor.h"

CommandDeleteText::CommandDeleteText(Cursor& cursor, Text* t, QTextCursor cL, QTextCursor cR)
    : cursor(cursor),
      t(t) {
    pL = cL.position();
    pR = cR.position();
    c = cL;
    c.setPosition(cR.position(), QTextCursor::KeepAnchor);
    str = c.selectedText();
    c = cL;
}

void CommandDeleteText::redo(){
    c.setPosition(pL);
    c.setPosition(c.position() + str.length(), QTextCursor::KeepAnchor);
    c.removeSelectedText();
    t->updateToTop();
    cursor.setPosition(*t, c);
}

void CommandDeleteText::undo(){
    c.setPosition(pL);
    c.insertText(str);
    t->updateToTop();
    cursor.setPosition(*t, c);
    c.setPosition(c.position() - str.length());
}

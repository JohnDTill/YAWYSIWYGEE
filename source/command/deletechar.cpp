#include "deletechar.h"

#include "../cursor.h"
#include "../text.h"

DeleteChar::DeleteChar(Cursor& cursor, bool forward, Text* t, QTextCursor c)
    : t(t),
      cursor(cursor),
      c(c) {
    if(forward){
        pL = c.position();
        c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        pR = c.position();
        str = c.selectedText();
    }else{
        pR = c.position();
        c.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        pL = c.position();
        str = c.selectedText();
    }

    setText("D");
}

void DeleteChar::removeChar(bool forward){
    if(forward){
        c.setPosition(pL);
        c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        pR += c.position() - pL;
        str.append(c.selectedText());
        c.removeSelectedText();
    }else{
        c.setPosition(pL);
        c.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        pL = c.position();
        str.prepend(c.selectedText());
        c.removeSelectedText();
    }

    t->updateToTop();
    cursor.setPosition(*t, c);
}

void DeleteChar::redo(){
    c.setPosition(pL);
    c.setPosition(pR, QTextCursor::KeepAnchor);
    c.removeSelectedText();
    t->updateToTop();
    cursor.setPosition(*t, c);
}

void DeleteChar::undo(){
    c.setPosition(pL);
    c.insertText(str);
    t->updateToTop();
    cursor.setPosition(*t, c);
}

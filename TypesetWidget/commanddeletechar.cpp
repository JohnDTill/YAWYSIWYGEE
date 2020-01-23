#include "commanddeletechar.h"

#include "cursor.h"

namespace Typeset{

CommandDeleteChar::CommandDeleteChar(Cursor& cursor, bool forward, Text* t, QTextCursor c)
    : t(t),
      cursor(cursor),
      c(c) {
    if(forward){
        pL = c.position();
        pR = pL+1;
        c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        str = c.selectedText();
    }else{
        pR = c.position();
        pL = pR-1;
        c.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        str = c.selectedText();
    }

    setText("D");
}

void CommandDeleteChar::removeChar(bool forward){
    if(forward){
        c.setPosition(pL);
        c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        str.append(c.selectedText());
        c.removeSelectedText();
        pR++;
    }else{
        c.setPosition(pL);
        c.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        str.prepend(c.selectedText());
        c.removeSelectedText();
        pL--;
    }

    t->updateToTop();
    cursor.setPosition(*t, c);
}

void CommandDeleteChar::redo(){
    c.setPosition(pL);
    c.setPosition(pR, QTextCursor::KeepAnchor);
    c.removeSelectedText();
    t->updateToTop();
    cursor.setPosition(*t, c);
}

void CommandDeleteChar::undo(){
    c.setPosition(pL);
    c.insertText(str);
    t->updateToTop();
    cursor.setPosition(*t, c);
}

}

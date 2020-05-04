#include "deletephrase.h"

#include "../algorithm.h"
#include "../construct.h"
#include "../cursor.h"
#include "../text.h"

DeletePhrase::DeletePhrase(Cursor& cursor, Text* tL, QTextCursor cL, Text* tR, QTextCursor cR)
    : cursor(cursor),
      tL(tL),
      cL(cL),
      tR(tR),
      cR(cR),
      removed_str( Algorithm::QStringRight(cL) ),
      append_str ( Algorithm::QStringRight(cR) ) {
    tL_next = tL->next;
    pL = cL.position();
}

DeletePhrase::~DeletePhrase(){
    if(!active) return;

    Construct* c = tL_next;
    Construct* stop = tR->next;
    while(c != stop){
        Construct* to_delete = c;
        c = c->next->next;
        delete to_delete->next;
        to_delete->deletePostorder();
    }
}

void DeletePhrase::redo(){
    active = true;

    cL.setPosition(pL);
    cL.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cL.insertText(append_str);
    cL.setPosition(pL);

    for(Construct* c = tL_next; c != tR->next; c = c->next->next){
        c->hide();
        c->next->hide();
    }

    tL->next = tR->next;
    if(tL->next) tL->next->prev = tL;
    else tL->parent->back = tL;

    tL->updateToTop();

    cursor.setPosition(*tL, cL);
}

void DeletePhrase::undo(){
    active = false;

    cL.setPosition(pL);
    cL.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cL.insertText(removed_str);
    cL.setPosition(pL);

    for(Construct* c = tL_next; c != tR->next; c = c->next->next){
        c->show();
        c->next->show();
    }

    link(tL, tL_next);
    if(tR->next) tR->next->prev = tR;
    else tR->parent->back = tR;

    tR->startSignalToNext();
    tL->updateToTop();

    cursor.setPosition(*tR, cR);
}

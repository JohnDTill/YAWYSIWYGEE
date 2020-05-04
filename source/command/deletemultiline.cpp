#include "deletemultiline.h"

#include "../algorithm.h"
#include "../construct.h"
#include "../cursor.h"
#include "../line.h"
#include "../text.h"
#include "../typesetscene.h"

DeleteMultiline::DeleteMultiline(Cursor& cursor, TypesetScene& doc, Text* tL, QTextCursor cL, Text* tR, QTextCursor cR)
    : cursor(cursor),
      doc(doc),
      tL(tL),
      cL(cL),
      tR(tR),
      cR(cR),
      removed_str( Algorithm::QStringRight(cL) ),
      append_str ( Algorithm::QStringRight(cR) ) {
    pL = cL.position();

    lL = &tL->parent->getLine();
    lR = &tR->parent->getLine();
    tL_next = tL->next;
    tR_next = tR->next;
    lL_next = lL->next;
}

DeleteMultiline::~DeleteMultiline(){
    if(!active) return;

    Construct* c = tL_next;
    while(c){
        Construct* to_delete = c;
        c = c->next->next;
        delete to_delete->next;
        to_delete->deletePostorder();
    }

    Line* l = lR;
    while(l != lL){
        Line* to_delete = l;
        l = l->prev;
        to_delete->deletePostorder();
    }
}

void DeleteMultiline::redo(){
    active = true;

    cL.setPosition(pL);
    cL.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cL.insertText(append_str);
    //cL.setPosition(cL.position() - append_str.size());
    cL.setPosition(pL);

    //Hide first line right
    for(Construct* c = tL_next; c; c = c->next->next){
        c->hide();
        c->next->hide();
    }
    //Hide intermediate last lines
    for(Line* l = lL_next; l != lR->next; l = l->next)
        l->hide();

    //Move last line right to first line right
    tL->next = tR->next;
    if(tL->next){
        tL->next->prev = tL;
        for(Construct* c = tL->next; c; c = c->next->next){
            c->setParentItem(lL);
            c->next->setParentPhrase(lL);
            lL->back = c->next;
        }
    }else lL->back = tL;

    tR->next = nullptr;
    lR->back = tR;

    //Connect line after selection to first line
    lL->next = lR->next;
    if(lL->next){
        lL->next->prev = lL;
        lL->next->setParentItem(lL);
        lL->next->renumberLine( lL->line_num + 1 );
    }else{
        doc.back = lL;
    }

    tL->updateToTop();

    cursor.setPosition(*tL, cL);
}

void DeleteMultiline::undo(){
    active = false;

    cL.setPosition(pL);
    cL.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cL.insertText(removed_str);
    cL.setPosition(pL);

    //Show first line right
    tL->next = tL_next;
    lL->back = tL;
    for(Construct* c = tL_next; c; c = c->next->next){
        c->show();
        c->next->show();
        lL->back = c->next;
    }
    //Show intermediate lines
    for(Line* l = lL_next; l != lR->next; l = l->next)
        l->show();

    if(tR_next){
        tR->next = tR_next;
        tR_next->prev = tR;
        for(Construct* c = tR_next; c; c = c->next->next){
            c->setParentItem(lR);
            c->next->setParentPhrase(lR);
            lR->back = c->next;
        }
    }

    //Connect line second selected line
    if(lL->next){
        lL->next->prev = lR;
        lL->next->setParentItem(lR);
        lL->next->renumberLine( lR->line_num + 1 );
    }else{
        doc.back = lR;
    }
    lL->next = lL_next;

    tL->updateToTop();
    tR->startSignalToNext();
    lR->updateToTop();

    cursor.setPosition(*tR, cR);
}

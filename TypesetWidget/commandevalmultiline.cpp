#include "commandevalmultiline.h"

#include "construct.h"
#include "cursor.h"
#include "document.h"
#include "line.h"
#include "parser.h"

namespace Typeset{

CommandEvalMultiline::CommandEvalMultiline(Cursor& cursor, Document& doc, const QString& source, Text* t, QTextCursor c)
    : cursor(cursor),
      doc(doc),
      tL(t),
      cL(c) {
    lL = &tL->parent->getLine();

    std::pair<Line*,Line*> ends = Parser::parseMultiline(source, t->parent->getLine().line_num);

    Text* first_parsed_text = ends.first->front;
    append_str = first_parsed_text->toPlainText();
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    removed_str = c.selectedText();
    tL_next = first_parsed_text->next;
    if(tL_next){
        tL_next->prev = t;
        for(Construct* c = tL_next; c; c = c->next->next){
            c->setParentItem(lL);
            c->next->setParentPhrase(lL);
        }
    }
    delete first_parsed_text;
    lL_next = ends.first->next;
    lL_next->prev = lL;
    lL_next->setParentItem(lL);
    delete ends.first;

    ends.second->next = lL->next;

    lR = ends.second;
    tR = lR->back;
    tR->next = tL->next; //tL->next is getting lost!
    if(tL->next) lR->back = lL->back;
    lR->next = lL->next;
    cR = tR->textCursor();
    cR.movePosition(QTextCursor::End);
    cR.insertText(removed_str);
    cR.setPosition(cR.position() - removed_str.length());
}

CommandEvalMultiline::~CommandEvalMultiline(){
    if(active) return;

    Construct* c = tL_next;
    while(c){
        Construct* to_delete = c;
        c = c->next->next;
        delete to_delete->next;
        to_delete->deletePostorder();
    }

    tR->next = nullptr;
    Line* l = lR->prev;
    while(l != lL){
        Line* to_delete = l;
        l = l->prev;
        to_delete->deletePostorder();
    }
}

void CommandEvalMultiline::redo(){
    active = true;

    cL.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cL.insertText(append_str);
    cL.setPosition(cL.position() - append_str.size());

    //Show first line right
    tL->next = tL_next; //error could boil down to poor understanding of tL_next
    lL->back = tL;
    for(Construct* c = tL_next; c; c = c->next->next){
        c->show();
        c->next->show();
        lL->back = c->next;
    }
    //Show intermediate lines
    for(Line* l = lL_next; l != lR->next; l = l->next)
        l->show();

    if(tR->next){
        tR->next->prev = tR;
        for(Construct* c = tR->next; c; c = c->next->next){
            c->setParentItem(lR);
            c->next->setParentPhrase(lR);
            lR->back = c->next;
        }
    }

    //Connect line second selected line
    if(lR->next){
        lR->next->prev = lR;
        lR->next->setParentItem(lR);
        lR->next->renumberLine( lR->line_num + 1 );
    }else{
        doc.back = lR;
    }
    lL->next = lL_next;

    tL->updateToTop();
    lR->updateToTop();
    cursor.setPosition(*tR, cR);
}

void CommandEvalMultiline::undo(){
    active = false;

    cL.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cL.insertText(removed_str);
    cL.setPosition(cL.position() - removed_str.size());

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

}

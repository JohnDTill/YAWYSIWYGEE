#include "commandinsertmultiline.h"

#include "construct.h"
#include "cursor.h"
#include "scene.h"

namespace Typeset{

CommandInsertMultiline::CommandInsertMultiline(Cursor& cursor, Scene& doc, const QString& source, Text* t, QTextCursor c)
    : cursor(cursor),
      doc(doc),
      tL(t),
      cL(c),
      pL(c.position()) {
    lL = &tL->parent->getLine();
    lL_next = lL->next;
    tL_next = tL->next;
    QStringList lines = source.split('\n');
    append_str = lines.front();
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    removed_str = c.selectedText();
    uint8_t script_level = t->getScriptLevel();
    uint32_t line_num = lL->line_num + 1;
    Line* l = lL;
    for(int i = 1; i < lines.size(); i++){
        Text* text = new Text(script_level, lines[i]);
        text->next = text->prev = nullptr;
        Line* next = new Line(text, text, line_num++);
        link(l, next);
        l = next;
    }
    l->next = lL_next;
    lL_next = lL->next;
    lR = l;
    tR = lR->back;
    tR->next = tL->next;
    cR = tR->textCursor();
    cR.movePosition(QTextCursor::End);
    cR.insertText(removed_str);
    cR.setPosition(cR.position() - removed_str.length());

    tR->calculateSize();
}

CommandInsertMultiline::~CommandInsertMultiline(){
    if(active) return;

    tR->next = nullptr;
    Line* l = lR;
    while(l != lL){
        Line* to_delete = l;
        l = l->prev;
        to_delete->deletePostorder();
    }
}

void CommandInsertMultiline::redo(){
    active = true;

    cL.setPosition(pL);
    cL.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cL.insertText(append_str);
    cL.setPosition(cL.position() - append_str.size());
    tL->next = nullptr;
    if(tL_next){
        tL_next->prev = tR;
        for(Construct* c = tL_next; c; c = c->next->next){
            c->setParentItem(lR);
            c->next->setParentPhrase(lR);
        }
    }
    lL->back = tL;
    for(Line* l = lL_next; l != lR->next; l = l->next)
        l->show();
    if(lR->next){
        lR->next->prev = lR;
        lR->next->setParentItem(lR);
        lR->next->renumberLine( lR->line_num + 1 );
    }else{
        doc.back = lR;
    }
    lL->next = lL_next;

    tL->updateToTop();
    tR->startSignalToNext();
    lR->updateToTop();
    cursor.setPosition(*tR, cR);
}

void CommandInsertMultiline::undo(){
    active = false;

    cL.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cL.insertText(removed_str);
    cL.setPosition(cL.position() - removed_str.size());

    tL->next = tL_next;
    if(tL_next) tL_next->prev = tL;

    for(Construct* c = tL_next; c; c = c->next->next){
        c->setParentItem(lL);
        c->next->setParentPhrase(lL);
        lL->back = c->next;
    }

    for(Line* l = lL_next; l != lR->next; l = l->next)
        l->hide();

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

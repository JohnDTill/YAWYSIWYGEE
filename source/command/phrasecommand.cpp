#include "phrasecommand.h"

#include "../algorithm.h"
#include "../construct.h"
#include "../cursor.h"
#include "../parser.h"
#include "../phrase.h"
#include "../text.h"
#include <QTextDocument>

QUndoCommand* PhraseCommand::eval(Cursor& cursor, const QString& source, Text* t, QTextCursor c){
    return new PhraseCommand(cursor, source, t, c);
}

QUndoCommand* PhraseCommand::remove(Cursor& cursor, Text* text, QTextCursor c, Text* anchor_text, QTextCursor ac){
    return text->x() < anchor_text->x() ?
           new PhraseCommand(false, cursor, text, c, anchor_text, ac) :
           new PhraseCommand(false, cursor, anchor_text, ac, text, c);
}

PhraseCommand::PhraseCommand(Cursor& cursor, const QString& source, Text* t, QTextCursor c)
    : is_insertion(true),
      cursor(cursor),
      t(t),
      cb(t->next),
      l_a(c.position()),
      l_b(t->document()->characterCount()-1 - l_a){
    //Eval inserted phrase
    std::pair<Text*,Text*> ends = Parser::parsePhrase(source, t->getScriptLevel());

    //Take information from first text and delete
    ci = ends.first->next;
    ci->prev = t;
    str_ti = ends.first->toPlainText();
    delete ends.first;

    //Set parent of inserted elements
    for(Construct* c = ci; c; c = c->next->next){
        c->setParentItem(t->parent);
        c->next->setParentPhrase(t->parent);
    }

    //Get str_tb
    QTextCursor tc_t = t->textCursor();
    tc_t.setPosition(l_a);
    tc_t.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    QString str_tb = tc_t.selectedText();

    //Setup last inserted element
    ei = ends.second;
    ei->next = cb;
    ei->setPlainText(ei->toPlainText() + str_tb);
    ei->calculateSize();
}

PhraseCommand::PhraseCommand(const bool is_insertion, Cursor& cursor, Text* t, QTextCursor cL, Text* ei, QTextCursor cR)
    : is_insertion(is_insertion),
      cursor(cursor),
      t(t),
      cb(ei->next),
      l_a(cL.position()),
      l_b(ei->document()->characterCount()-1-cR.position()),
      ei(ei),
      ci(t->next),
      str_ti( Algorithm::QStringRight(cL) ) {}

PhraseCommand::~PhraseCommand(){
    if(!owns_data) return;

    //Delete all inserted elements
    for(Construct* c = ci; c != cb;){
        Construct* temp = c;
        c = c->next->next;
        delete temp->next;
        temp->deletePostorder();
    }
}

void PhraseCommand::redo(){
    is_insertion ? eval() : remove();
}

void PhraseCommand::undo(){
    is_insertion ? remove() : eval();
}

void PhraseCommand::eval(){
    owns_data = false;

    //Connect t to start of inserted text
    t->next = ci;

    //Connect end of inserted text to cb
    if(cb) cb->prev = ei;
    else t->parent->back = ei;

    //Show inserted elements
    for(Construct* c = ci; c != cb; c = c->next->next){
        c->show();
        c->next->show();
    }

    //Swap str_tb with str_ti
    QTextCursor tc_t = t->textCursor();
    tc_t.setPosition(l_a);
    tc_t.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    tc_t.insertText(str_ti);

    //Update graphically
    ei->startSignalToNext();
    t->updateToTop();
    QTextCursor tc_ei = ei->textCursor();
    tc_ei.setPosition(ei->document()->characterCount()-1 - l_b);
    cursor.setPosition(*ei, tc_ei);
}

void PhraseCommand::remove(){
    owns_data = true;

    //Reconnect phrase which was split by insertion
    t->next = cb;
    if(cb) cb->prev = t;
    else t->parent->back = t;

    //Hide inserted elements
    for(Construct* c = ci; c != cb; c = c->next->next){
        c->hide();
        c->next->hide();
    }

    //Get str_tb
    QTextCursor tc_ei = ei->textCursor();
    tc_ei.movePosition(QTextCursor::End);
    tc_ei.setPosition(tc_ei.position() - l_b, QTextCursor::KeepAnchor);
    QString str_tb = tc_ei.selectedText();

    //Swap str_ti with str_tb
    QTextCursor tc_t = t->textCursor();
    tc_t.setPosition(l_a);
    tc_t.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    tc_t.insertText(str_tb);

    //Update graphically
    t->updateToTop();
    tc_t.setPosition(l_a);
    cursor.setPosition(*t, tc_t);
}

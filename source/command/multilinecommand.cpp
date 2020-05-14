#include "multilinecommand.h"

#include "../algorithm.h"
#include "../construct.h"
#include "../cursor.h"
#include "../line.h"
#include "../parser.h"
#include "../text.h"
#include "../typesetscene.h"
#include <QTextDocument>

QUndoCommand* MultilineCommand::insert(Cursor& cursor, TypesetScene& doc, const QString& source, Text* t, QTextCursor c){
    QStringList lines = source.split('\n');
    Q_ASSERT(lines.size() > 1);

    #ifdef __EMSCRIPTEN__ //WASM code isn't splitting correctly, leaves trailing newline
    for(int i = 0; i < lines.size(); i++){
        lines[i].remove('\n');
        lines[i].remove('\r');
        lines[i].remove("\\n");
    }
    #endif

    uint8_t script_level = t->getScriptLevel();
    uint32_t line_num = t->parent->getLine().line_num;
    Text* text = new Text(script_level, lines[0]);
    Line* li = new Line(text, text, line_num++);
    Line* l = li;
    for(int i = 1; i < lines.size(); i++){
        Text* text = new Text(script_level, lines[i]);
        Line* next = new Line(text, text, line_num++);
        link(l, next);
        l = next;
    }

    return new MultilineCommand(cursor, doc, li, l, t, c);
}

QUndoCommand* MultilineCommand::eval(Cursor& cursor, TypesetScene& doc, const QString& source, Text* t, QTextCursor c){
    //Eval inserted lines
    std::pair<Line*,Line*> ends = Parser::parseMultiline(source, t->parent->getLine().line_num);

    return new MultilineCommand(cursor, doc, ends.first, ends.second, t, c);
}

QUndoCommand* MultilineCommand::remove(Cursor& cursor, TypesetScene& doc, Text* t, QTextCursor c, Text* at, QTextCursor ac){
    if(t->parent->getLine().proceeds(at->parent->getLine()))
        return new MultilineCommand(cursor, doc, t, c, at, ac);
    else
        return new MultilineCommand(cursor, doc, at, ac, t, c);
}

MultilineCommand::MultilineCommand(Cursor& cursor, TypesetScene& doc, Line* li, Line* lf, Text* t, QTextCursor c)
    : is_insertion(true),
      cursor(cursor),
      doc(doc),
      t(t),
      cb(t->next),
      la(&t->parent->getLine()),
      e(la->back),
      lb(la->next),
      lf(lf),
      l_a(c.position()),
      l_b(t->document()->characterCount()-1 - l_a) {
    //Take information from first line and delete
    Text* ti = li->front;
    ei = li->back;
    str_ti = ti->toPlainText();
    ci = ti->next;
    if(ci){
        ci->prev = t;
        for(Construct* c = ci; c; c = c->next->next){
            c->setParentItem(la);
            c->next->setParentPhrase(la);
        }
    }
    ln = li->next;
    ln->prev = la;
    ln->setParentItem(la);
    delete ti;
    delete li;

    //Setup last line connections
    ef = lf->back;
    ef->next = ci;
    lf->next = lb;

    //Get str_tb
    QTextCursor tc_t = t->textCursor();
    tc_t.setPosition(l_a);
    tc_t.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    QString str_tb = tc_t.selectedText();

    //Append str_tb to last line
    QTextCursor tc_ef = ef->textCursor();
    tc_ef.movePosition(QTextCursor::End);
    tc_ef.insertText(str_tb);
    ef->calculateSize();
}

MultilineCommand::MultilineCommand(Cursor& cursor, TypesetScene& doc, Text* tL, QTextCursor cL, Text* tR, QTextCursor cR)
    : is_insertion(false),
      cursor(cursor),
      doc(doc),
      t(tL),
      cb(tR->next),
      la(&t->parent->getLine()),
      e(tR->parent->back),
      lb(tR->parent->getLine().next),
      str_ti( Algorithm::QStringRight(cL) ),
      ci(t->next),
      ei(la->back),
      ln(la->next),
      lf(&tR->parent->getLine()),
      ef(tR),
      l_a(cL.position()),
      l_b(ef->document()->characterCount()-1-cR.position()){}

MultilineCommand::~MultilineCommand(){
    if(!owns_data) return;

    //Delete elements in first phrase
    for(Construct* c = ci; c;){
        Construct* temp = c;
        c = c->next->next;
        delete temp->next;
        temp->deletePostorder();
    }

    //Delete inserted lines
    ef->next = nullptr;
    for(Line* l = lf; l != la;){
        Line* temp = l;
        l = l->prev;
        temp->deletePostorder();
    }
}

void MultilineCommand::redo(){
    is_insertion ? insert() : remove();
}

void MultilineCommand::undo(){
    is_insertion ? remove() : insert();
}

void MultilineCommand::insert(){
    owns_data = false;

    //Connect t to start of inserted text
    if(ci){
        t->next = ci;
        la->back = ei;
    }else{
        t->next = nullptr;
        la->back = t;
    }

    //Connect split line right half to last inserted line
    ef->next = cb;
    if(cb){
        cb->prev = ef;
        for(Construct* c = cb; c; c = c->next->next){
            c->setParentItem(lf);
            c->next->setParentPhrase(lf);
        }
        lf->back = e;
    }

    //Connect second inserted line
    la->next = ln;

    //Connect line after insertion
    if(lb){
        lb->prev = lf;
        lb->setParentItem(lf);
        lb->renumberLine(lf->line_num + 1);
    }else{
        doc.back = lf;
    }

    //Show inserted phrase elements
    for(Construct* c = ci; c; c = c->next->next){
        c->show();
        c->next->show();
    }

    //Show inserted lines
    for(Line* l = ln; l != lb; l = l->next) l->show();

    //Append str_ti to t
    QTextCursor tc_t = t->textCursor();
    tc_t.setPosition(l_a);
    tc_t.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    tc_t.insertText(str_ti);

    //Update graphically
    ef->startSignalToNext();
    t->updateToTop();
    lf->updateToTop();
    QTextCursor tc_ef = ef->textCursor();
    tc_ef.setPosition(ef->document()->characterCount()-1 - l_b);
    cursor.setPosition(*ef, tc_ef);
}

void MultilineCommand::remove(){
    owns_data = true;

    //Reconnect phrase which was split by insertion
    t->next = cb;
    if(cb){
        cb->prev = t;
        for(Construct* c = cb; c; c = c->next->next){
            c->setParentItem(la);
            c->next->setParentPhrase(la);
        }
        la->back = e;
        lf->back = ef; //Set last inserted line back for proper iteration in case of deletion
        ef->next = nullptr; //Disconnect last inserted line
    }else la->back = t;

    //Reconnect lines
    la->next = lb;
    if(lb){
        lb->prev = la;
        lb->setParentItem(la);
        lb->renumberLine(la->line_num + 1);
    }else{
        doc.back = la;
    }

    //Hide first phrase inserted elements
    for(Construct* c = ci; c; c = c->next->next){
        c->hide();
        c->next->hide();
    }

    //Hide inserted lines
    for(Line* l = ln; l != lb; l = l->next) l->hide();

    //Get str_tb
    QTextCursor tc_ef = ef->textCursor();
    tc_ef.movePosition(QTextCursor::End);
    tc_ef.setPosition(tc_ef.position() - l_b, QTextCursor::KeepAnchor);
    QString str_tb = tc_ef.selectedText();

    //Append str_tb to t
    QTextCursor tc_t = t->textCursor();
    tc_t.setPosition(l_a);
    tc_t.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    tc_t.insertText(str_tb);
    tc_t.setPosition(l_a);

    //Update graphically
    t->updateToTop();
    cursor.setPosition(*t, tc_t);
}

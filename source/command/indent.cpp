#include "indent.h"

#include "../cursor.h"
#include "../line.h"
#include "../text.h"
#include <QTextCursor>
#include <QTextDocument>

Indent::Indent(Cursor& cursor, Line* ll, Line* end)
    : cursor(cursor),
      ll(ll),
      end(end) {}

void Indent::redo(){
    QTextCursor c;
    Text* t;
    for(Line* lc = ll; lc != end; lc = lc->next){
        t = lc->front;
        c = t->textCursor();
        c.setPosition(0);
        c.insertText(QString(spaces_per_tab, ' '));
        t->updateToTop();
    }

    c.setPosition(0);
    cursor.setPosition(*t, c);
}

void Indent::undo(){
    QTextCursor c;
    Text* t;

    for(Line* lc = ll; lc != end; lc = lc->next){
        t = lc->front;
        c = t->textCursor();
        c.setPosition(0);
        c.setPosition(spaces_per_tab, QTextCursor::KeepAnchor);
        c.removeSelectedText();
        t->updateToTop();
    }

    c.setPosition(0);
    cursor.setPosition(*t, c);
}


Deindent::Deindent(Cursor& cursor, Line* ll, Line* end)
    : cursor(cursor),
      ll(ll),
      end(end),
      found_spaces(findSpaces()) {}

void Deindent::redo(){
    QTextCursor c;
    Text* t;
    std::vector<uint8_t>::size_type i = 0;

    for(Line* lc = ll; lc != end; lc = lc->next){
        t = lc->front;
        c = t->textCursor();
        const uint8_t& num_spaces = spaces[i++];
        c.setPosition(0);
        c.setPosition(num_spaces, QTextCursor::KeepAnchor);
        c.removeSelectedText();
        t->updateToTop();
    }

    c.setPosition(0);
    cursor.setPosition(*t, c);
}

void Deindent::undo(){
    QTextCursor c;
    Text* t;
    std::vector<uint8_t>::size_type i = 0;

    for(Line* lc = ll; lc != end; lc = lc->next){
        t = lc->front;
        c = t->textCursor();
        const uint8_t& num_spaces = spaces[i++];
        c.setPosition(0);
        c.insertText(QString(num_spaces, ' '));
        t->updateToTop();
    }

    c.setPosition(0);
    cursor.setPosition(*t, c);
}

bool Deindent::findSpaces(){
    bool found_spaces = false;

    for(Line* lc = ll; lc != end; lc = lc->next){
        uint8_t num = countSpaces(lc);
        spaces.push_back(num);
        found_spaces = found_spaces || num;
    }

    return found_spaces;
}

uint8_t Deindent::countSpaces(const Line* l) const{
    const QTextDocument* doc = l->front->document();
    for(uint8_t i = 0; i < spaces_per_tab && i < doc->characterCount(); i++)
        if(doc->characterAt(i) != ' ') return i;
    return spaces_per_tab < doc->characterCount() ? spaces_per_tab : doc->characterCount();
}

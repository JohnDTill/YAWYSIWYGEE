#include "alignatsymbol.h"

#include "../construct/padding.h"
#include "../cursor.h"
#include "../line.h"
#include "../text.h"
#include <QTextCursor>

AlignAtSymbol::AlignAtSymbol(Cursor& cursor, Line* ll, Line* lr, const std::vector<qreal>& offsets)
    : cursor(cursor) {
    qreal max_offset = 0;
    for(const qreal& val : offsets) max_offset = std::max(max_offset, val);

    std::vector<qreal>::size_type i = 0;
    for(Line* l = ll; l != lr->next; l = l->next){
        const qreal& val = offsets[i++];
        if(val == max_offset) continue;
        Text* t = new Text();
        Construct* c = new Padding(max_offset - val - 2*Phrase::padding_between_elements - 1);
        link(t,c);
        c->next = l->front;
        t->setParentPhrase(l);
        c->setParentItem(l);
        texts.push_back(t);
    }
}

AlignAtSymbol::~AlignAtSymbol(){
    if(!owns_data) return;

    for(Text* t : texts){
        delete t->next;
        delete t;
    }
}

void AlignAtSymbol::redo(){
    owns_data = false;

    for(Text* t : texts){
        t->show();
        t->next->show();
        t->parent->front = t;
        t->next->next->prev = t->next;
        t->next->updateToTop();
    }
}

void AlignAtSymbol::undo(){
    owns_data = true;

    for(Text* t : texts){
        t->hide();
        t->next->hide();
        t->parent->front = t->next->next;
        t->next->next->prev = nullptr;
        t->parent->updateToTop();
    }

    Text* t = texts.back()->next->next;
    QTextCursor c = t->textCursor();
    c.setPosition(0);
    cursor.setPosition(*t,c);
}

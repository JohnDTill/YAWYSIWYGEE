#include "phrase.h"

#include "construct.h"
#include "text.h"

#ifdef YAWYSIWYGEE_TEST
#include <list>
static std::list<Typeset::Phrase*> all_phrases;
#endif

Phrase::Phrase(Text* f, Text* b)
    : front(f),
      back(b) {
    setFlag(QGraphicsItem::ItemIsSelectable, false);

    if(front==nullptr) front = new Text();
    if(back==nullptr) back = front;

    initializeChildren();
    updateLayout();

    #ifdef YAWYSIWYGEE_TEST
    all_phrases.push_back(this);
    #endif
}

#ifdef YAWYSIWYGEE_TEST
Phrase::~Phrase(){
    all_phrases.remove(this);
}

void Phrase::verify(){
    for(Phrase* p : all_phrases){
        Q_ASSERT(p->front->prev == nullptr);
        Q_ASSERT(p->back->next == nullptr);

        Text* t = p->front;
        Q_ASSERT(t->parent == p);
        for(Construct* c = p->front->next; c; c = t->next){
            t = c->next;
            Q_ASSERT(t->parent == p);
        }
        Q_ASSERT(p->back == t);
    }
}
#endif

qreal Phrase::h() const{
    return u+d;
}

void Phrase::select(){
    setSelected(true);
    front->setSelected(true);
    for(Construct* c = front->next; c; c = c->next->next){
        c->select();
        c->next->setSelected(true);
    }
}

void Phrase::deletePostorder(){
    Construct* c = front->next;
    delete front;
    while(c){
        Construct* prev = c;
        c = c->next->next;
        delete prev->next;
        prev->deletePostorder();
    }
    delete this;
}

void Phrase::updateLayout(){
    calculateSize();

    Text* t = front;
    front->setPos(0, u - t->u);
    qreal x = front->w + padding_between_elements;

    for(Construct* c = t->next; c; c = t->next){
        c->setPos(x, u - c->u);
        x += c->w + padding_between_elements;

        t = c->next;
        t->setPos(x, u - t->u);
        x += t->w + padding_between_elements;
    }
}

void Phrase::calculateSize(){
    Text* t = front;
    w = t->w;
    u = t->u;
    d = t->u;

    for(Construct* c = t->next; c; c = t->next){
        w += (padding_between_elements + c->w);
        u = qMax(u, c->u);
        d = qMax(d, c->d);

        t = c->next;
        w += (padding_between_elements + t->w);
        u = qMax(u, t->u);
        d = qMax(d, t->u);
    }
}

void Phrase::writeContents(QTextStream& out) const{
    front->write(out);
    for(Construct* c = front->next; c; c = c->next->next){
        c->write(out);
        c->next->write(out);
    }
}

void Phrase::initializeChildren(){
    front->setParentPhrase(this);

    for(Construct* c = front->next; c; c = c->next->next){
        c->setParentItem(this);
        c->next->setParentPhrase(this);
    }
}

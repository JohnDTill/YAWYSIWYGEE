#include "construct.h"

#include "algorithm.h"
#include "cursor.h"
#include "subphrase.h"
#include "typesetscene.h"
#include "text.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#ifdef YAWYSIWYGEE_TEST
#include <list>
static std::list<Typeset::Construct*> all_constructs;
#endif

Construct::Construct(){
    setFlag(GraphicsItemFlag::ItemIsSelectable);

    #ifdef YAWYSIWYGEE_TEST
    all_constructs.push_back(this);
    #endif
}

#ifdef YAWYSIWYGEE_TEST
Construct::~Construct(){
    all_constructs.remove(this);
}

void Construct::verify(){
    for(Construct* c : all_constructs){
        Q_ASSERT(c->next);
        Q_ASSERT(c->next->prev == c);
        Q_ASSERT(c->prev);
        Q_ASSERT(c->prev->next);
    }
}
#endif

qreal Construct::h() const{
    return u+d;
}

void Construct::updateToTop(){
    prepareGeometryChange();
    updateLayout();
    next->notifyPrevUpdate();
    prev->parent->updateToTop();
}

Line& Construct::getLine() const{
    return prev->parent->getLine();
}

void Construct::populateMenu(QMenu& menu, const SubPhrase*){
    prev->parent->populateMenu(menu);
}

void Construct::setupPainter(QPainter* painter, const QStyleOptionGraphicsItem* options){
    if(options->state.testFlag(QStyle::StateFlag::State_Selected))
        painter->setPen(options->palette.highlightedText().color());
}

QRectF Construct::boundingRect() const{
    return QRectF(0, 0, w, h());
}

TypesetScene* Construct::typesetDocument() const{
    return static_cast<TypesetScene*>(scene());
}

void link(Text* t, Construct* c){
    t->next = c;
    c->prev = t;
}

void link(Construct* c, Text* t){
    c->next = t;
    t->prev = c;
}

void TerminalConstruct::deletePostorder(){
    delete this;
}

void TerminalConstruct::select(){
    setSelected(true);
}

SubPhrase* TerminalConstruct::front() const{
    return nullptr;
}

SubPhrase* TerminalConstruct::back() const{
    return nullptr;
}

Text* TerminalConstruct::textRight(const SubPhrase*) const{
    return nullptr; //unreachable
}

Text* TerminalConstruct::textLeft(const SubPhrase*) const{
    return nullptr; //unreachable
}

Text* TerminalConstruct::textUp(const SubPhrase*, qreal) const{
    return nullptr; //unreachable
}

Text* TerminalConstruct::textDown(const SubPhrase*, qreal) const{
    return nullptr; //unreachable
}

UnaryConstruct::UnaryConstruct(SubPhrase* c)
    :child(c) {
    if(child==nullptr) child = new SubPhrase();
    child->setParentConstruct(*this);
}

void UnaryConstruct::deletePostorder(){
    child->deletePostorder();
    delete this;
}

void UnaryConstruct::select(){
    setSelected(true);
    child->select();
}

SubPhrase* UnaryConstruct::front() const{
    return child;
}

SubPhrase* UnaryConstruct::back() const{
    return child;
}

Text* UnaryConstruct::textRight(const SubPhrase*) const{
    return next;
}

Text* UnaryConstruct::textLeft(const SubPhrase*) const{
    return prev;
}

Text* UnaryConstruct::textUp(const SubPhrase*, qreal) const{
    return prev;
}

Text* UnaryConstruct::textDown(const SubPhrase*, qreal) const{
    return next;
}

BinaryConstruct::BinaryConstruct(SubPhrase* f, SubPhrase* s)
    : first(f),
      second(s) {
    if(first==nullptr) first = new SubPhrase;
    first->setParentConstruct(*this);
    if(second==nullptr) second = new SubPhrase;
    second->setParentConstruct(*this);
}

void BinaryConstruct::deletePostorder(){
    first->deletePostorder();
    second->deletePostorder();
    delete this;
}

void BinaryConstruct::select(){
    setSelected(true);
    first->select();
    second->select();
}

SubPhrase* BinaryConstruct::front() const{
    return first;
}

SubPhrase* BinaryConstruct::back() const{
    return second;
}

Text* BinaryConstruct::textRight(const SubPhrase* caller) const{
    return (caller==first) ? second->front : next;
}

Text* BinaryConstruct::textLeft(const SubPhrase* caller) const{
    return (caller==second) ? first->back : prev;
}

NaryConstruct::NaryConstruct(const std::vector<SubPhrase*> c)
    : children(c) {
    for(std::vector<SubPhrase*>::size_type i = 0; i < children.size(); i++){
        children[i]->setParentConstruct(*this);
        children[i]->child_id = static_cast<uint16_t>(i);
    }
}

void NaryConstruct::deletePostorder(){
    for(SubPhrase* child : children) child->deletePostorder();
    delete this;
}

void NaryConstruct::select(){
    setSelected(true);
    for(SubPhrase* child : children) child->select();
}

SubPhrase* NaryConstruct::front() const{
    return children.front();
}

SubPhrase* NaryConstruct::back() const{
    return children.back();
}

Text* NaryConstruct::textRight(const SubPhrase* caller) const{
    uint16_t i = caller->child_id + 1;
    return (i < children.size()) ? children[i]->front : next;
}

Text* NaryConstruct::textLeft(const SubPhrase* caller) const{
    uint16_t i = caller->child_id;
    return (i > 0) ? children[i-1]->back : prev;
}

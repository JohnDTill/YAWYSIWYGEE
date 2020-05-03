#include "root.h"

#include "algorithm.h"
#include "cursor.h"
#include "scene.h"
#include "globals.h"
#include "text.h"
#include <QMenu>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace Typeset{

SquareRoot::SquareRoot(SubPhrase* c)
    : UnaryConstruct(c) {
    updateLayout();
}

void SquareRoot::updateLayout(){
    calculateSize();
    child->setPos(hspace + x2 + height/slope, extra_height);
}

void SquareRoot::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    QAction* powerScriptAction = menu.addAction("Root: Add Power Script");
    connect(powerScriptAction, SIGNAL(triggered()), this, SLOT(addScript()));
}

void SquareRoot::write(QTextStream& out) const{
    out << ESCAPE << QChar(8730);
    child->write(out);
}

void SquareRoot::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);

    QPointF p0(0, height-5);
    QPointF p1(x1, height-7);
    QPointF p2(x2, height-2);
    QPointF p3(x2 + height/slope, 0);
    QPointF p4(w, 0);

    painter->drawLines({p0,p1,p1,p2,p2,p3,p3,p4});
}

void SquareRoot::calculateSize(){
    u = child->u + extra_height;
    d = child->d;
    height = d + u;
    w = hspace + overbar_extension + x2 + height/slope + child->w;
}

void SquareRoot::addScript(){
    typesetDocument()->undo_stack->push( new AddScript(this) );
}

SquareRoot::AddScript::AddScript(SquareRoot* out)
    : out(out) {
    Text* t = new Text(out->child->front->getScriptLevel() + 1);
    t->next = t->prev = nullptr;

    in = new ScriptedRoot(out->child, new SubPhrase(t));
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

SquareRoot::AddScript::~AddScript(){
    if(active){
        delete out;
    }else{
        in->first->deletePostorder();
        delete in;
    }
}

void SquareRoot::AddScript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->second->setParentConstruct(*in);
    in->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void SquareRoot::AddScript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->child->setParentConstruct(*out);
    out->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

ScriptedRoot::ScriptedRoot(SubPhrase* child, SubPhrase* power)
    : BinaryConstruct(power, child) {
    updateLayout();
}

#define power first
#define child second
void ScriptedRoot::updateLayout(){
    qreal ur = child->u + extra_height;
    qreal dr = child->d;
    height = ur + dr;
    xr = qMax(power->w + script_hoffset, 0.0);
    w = xr + hspace + overbar_extension + x2 + height/slope + child->w;

    qreal hp = power->u + power->d;
    qreal a = script_ratio*ur;
    qreal e = hp > a ? hp-a : 0;
    yr = e;

    u = ur + e;
    d = dr;

    child->setPos(xr + hspace + x2 + height/slope, yr);
}

void ScriptedRoot::populateMenu(QMenu& menu, const SubPhrase*){
    menu.addSeparator();
    QAction* powerScriptAction = menu.addAction("Root: Remove Power Script");
    connect(powerScriptAction, SIGNAL(triggered()), this, SLOT(removeScript()));
}

Text* ScriptedRoot::textUp(const SubPhrase* caller, qreal) const{
    return (caller==child) ? power->back : prev;
}

Text* ScriptedRoot::textDown(const SubPhrase* caller, qreal) const{
    return (caller==power) ? child->front : next;
}

void ScriptedRoot::write(QTextStream& out) const{
    out << ESCAPE << QChar(8730);
    child->write(out);
    power->write(out);
}

void ScriptedRoot::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);

    QPointF p0(xr, yr+height-5);
    QPointF p1(xr+x1, yr+height-7);
    QPointF p2(xr+x2, yr+height-2);
    QPointF p3(xr+x2 + height/slope, yr);
    QPointF p4(xr+(hspace + overbar_extension + x2 + height/slope + child->w), yr);

    painter->drawLines({p0,p1,p1,p2,p2,p3,p3,p4});
}

void ScriptedRoot::removeScript(){
    typesetDocument()->undo_stack->push( new RemoveScript(this) );
}

#undef power
#undef child

ScriptedRoot::RemoveScript::RemoveScript(ScriptedRoot* out)
    : out(out) {
    in = new SquareRoot(out->second);
    in->setParentItem(out->prev->parent);
    in->next = out->next;
    in->prev = out->prev;
}

ScriptedRoot::RemoveScript::~RemoveScript(){
    if(active){
        out->first->deletePostorder();
        delete out;
    }else{
        delete in;
    }
}

void ScriptedRoot::RemoveScript::redo(){
    active = true;

    out->hide();
    in->show();
    in->next->prev = in->prev->next = in;
    in->child->setParentConstruct(*in);
    in->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

void ScriptedRoot::RemoveScript::undo(){
    active = false;

    in->hide();
    out->show();
    out->next->prev = out->prev->next = out;
    out->second->setParentConstruct(*out);
    out->updateToTop();

    out->typesetDocument()->cursor->setPosition(*out->next, Algorithm::textCursorStart(out->next));
}

}

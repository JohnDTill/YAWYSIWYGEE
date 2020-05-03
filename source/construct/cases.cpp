#include "cases.h"

#include "algorithm.h"
#include "cursor.h"
#include "scene.h"
#include "globals.h"
#include <QMenu>
#include <QPainter>

namespace Typeset{

Cases::Cases(const std::vector<SubPhrase*>& data)
    : NaryConstruct(data){
    Q_ASSERT(data.size()%2 == 0);
    Q_ASSERT(data.size() > 0);
    updateLayout();
}

void Cases::updateLayout(){
    max_val_w = 0;
    qreal max_con_w = 0;
    for(std::vector<SubPhrase*>::size_type i = 0; i < children.size(); i+=2){
        max_val_w = qMax(max_val_w, children[i]->w);
        max_con_w = qMax(max_con_w, children[i+1]->w);
    }

    qreal xc = bracket_width + hspace + max_val_w;
    w = xc + max_con_w;
    qreal y = 0;
    for(std::vector<SubPhrase*>::size_type i = 0; i < children.size(); i+=2){
        qreal u = qMax(children[i]->u, children[i+1]->u);

        children[i]->setPos(bracket_width + (max_val_w - children[i]->w)/2,
                          y + u - children[i]->u);

        children[i+1]->setPos(xc + (max_con_w - children[i+1]->w)/2,
                              y + u - children[i+1]->u);

        y += vspace + u + qMax(children[i]->d, children[i+1]->d);
    }

    qreal h = y - vspace;
    u = d = h/2;
}

Text* Cases::textUp(const SubPhrase* caller, qreal x) const{
    major_integer i = caller->child_id;
    return (i > 1) ? Algorithm::textAtSetpoint(*children[i-2], x) : prev;
}

Text* Cases::textDown(const SubPhrase* caller, qreal x) const{
    major_integer i = caller->child_id + 2;
    return (i < children.size()) ? Algorithm::textAtSetpoint(*children[i], x) : next;
}

void Cases::populateMenu(QMenu& menu, const SubPhrase* caller){
    if(caller == nullptr){
        Construct::populateMenu(menu);
        return;
    }

    active = caller->child_id;

    menu.addSeparator();
    QAction* createRowBelow = menu.addAction("Conditional Value: Create row below");
    QAction* createRowAbove = menu.addAction("Conditional Value: Create row above");
    QAction* deleteRow = menu.addAction("Conditional Value: Delete row");
    connect(createRowBelow, SIGNAL(triggered()), this, SLOT(createRowBelow()));
    connect(createRowAbove, SIGNAL(triggered()), this, SLOT(createRowAbove()));
    connect(deleteRow, SIGNAL(triggered()), this, SLOT(deleteRow()));
    deleteRow->setEnabled(children.size() > 2);
}

void Cases::write(QTextStream& out) const{
    out << ESCAPE << "c";
    for(SubPhrase* c : children) c->write(out);
}

void Cases::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    setupPainter(painter, options);

    painter->drawArc(QRectF(bwh,0,bwh,u), 90*16, 90*16);
    painter->drawArc(QRectF(0,0,bwh,u), 270*16, 90*16);
    painter->drawArc(QRectF(0,u,bwh,u), 0*16, 90*16);
    painter->drawArc(QRectF(bwh,u,bwh,u), 180*16, 90*16);

    qreal x = bracket_width + 2 + max_val_w;
    qreal y = -7;
    for(std::vector<SubPhrase*>::size_type i = 0; i < children.size(); i+=2){
        y += qMax(children[i]->u, children[i+1]->u) + qMax(children[i]->d, children[i+1]->d);
        painter->drawText(QPointF(x, y), ",");
        y += vspace;
    }
}

void Cases::insertRow(major_integer row, SubPhrase* first, SubPhrase* second){
    children.resize(children.size() + 2);

    for(major_integer i = static_cast<major_integer>(children.size()-1); i > 2*row+1; i--){
        children[i] = children[i-2];
        children[i]->child_id = i;
    }

    children[2*row] = first;
    children[2*row + 1] = second;
    first->show();
    second->show();
    first->child_id = 2*row;
    second->child_id = 2*row + 1;

    updateToTop();
}

void Cases::removeRow(major_integer row){
    children[2*row]->hide();
    children[2*row+1]->hide();

    for(major_integer i = 2*row+2; i < children.size(); i++){
        children[i-2] = children[i];
        children[i]->child_id = i-2;
    }

    children.resize(children.size() - 2);
    updateToTop();
}

void Cases::createRowBelow(){
    typesetDocument()->undo_stack->push( new AddRow(*this, active/2+1 ) );
}

void Cases::createRowAbove(){
    typesetDocument()->undo_stack->push( new AddRow(*this, active/2 ) );
}

void Cases::deleteRow(){
    typesetDocument()->undo_stack->push( new RemoveRow(*this, active/2 ) );
}

Cases::AddRow::AddRow(Cases& c, major_integer row)
    : c(c),
      row(row) {
    uint8_t script_level = c.children.front()->front->getScriptLevel();

    for(std::vector<SubPhrase*>::size_type i = 0; i < 2; i++){
        Text* t = new Text(script_level);
        t->next = t->prev = nullptr;
        data.push_back( new SubPhrase(t) );
        data[i]->setParentConstruct(c);
    }
}

Cases::AddRow::~AddRow(){
    if(active) return;
    for(SubPhrase* datum : data) datum->deletePostorder();
}

void Cases::AddRow::redo(){
    active = true;
    c.insertRow(row, data[0], data[1]);
}

void Cases::AddRow::undo(){
    active = false;
    c.removeRow(row);
}

Cases::RemoveRow::RemoveRow(Cases& c, major_integer row)
    : c(c),
      row(row) {
    major_integer start = 2*row;
    for(std::vector<SubPhrase*>::size_type i = start; i < start + 2; i++)
        data.push_back(c.children[i]);
}

Cases::RemoveRow::~RemoveRow(){
    if(!active) return;
    for(SubPhrase* datum : data) datum->deletePostorder();
}

void Cases::RemoveRow::redo(){
    active = true;
    c.removeRow(row);
}

void Cases::RemoveRow::undo(){
    active = false;
    c.insertRow(row, data[0], data[1]);
}

}

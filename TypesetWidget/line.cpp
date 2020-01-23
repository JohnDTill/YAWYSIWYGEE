#include "line.h"

#include "algorithm.h"
#include "construct.h"
#include "document.h"
#include "globals.h"
#include "text.h"

namespace Typeset{

Line::Line(Text* f, Text* b, uint32_t line_num) :
    Phrase(f, b),
    line_num(line_num) {
    setFlag(QGraphicsItem::ItemHasNoContents);

    line_num_box.setParentItem(this);
    line_num_box.setText(QString::number(line_num));
    line_num_box.setFont(Globals::passive_linenum_font);
    line_num_box.setBrush(Globals::line_num_passive_brush);
    line_num_box.setFlag(QGraphicsItem::ItemIsSelectable, false);

    updateLayout();
    repositionLineNumber();
}

void Line::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*){
    //DO NOTHING
}

QRectF Line::boundingRect() const{
    return QRectF(0, y(), w, u+d);
}

void Line::repositionNextLine(){
    next->setPos(0, u+d+line_vspace);
}

void Line::renumberLine(uint32_t n){
    line_num = n;
    line_num_box.setText(QString::number(line_num));
    repositionLineNumber();
    if(next) next->renumberLine(n+1);
}

void Line::setLineNumberVisible(bool show){
    line_num_box.setVisible(show);
}

void Line::updateTheme(){
    Phrase::updateTheme();
    if(line_num_box.font().bold()) line_num_box.setBrush(Globals::line_num_active_brush);
    else line_num_box.setBrush(Globals::line_num_passive_brush);
}

void Line::updateToTop(){
    updateLayout();
    repositionLineNumber();
    if(next) repositionNextLine();
    static_cast<Document*>(scene())->updateSize();
}

Text* Line::textRight() const{
    return next ? next->front : nullptr;
}

Text* Line::textLeft() const{
    return prev ? prev->back : nullptr;
}

Text* Line::textRightUp() const{
    return textRight();
}

Text* Line::textLeftUp() const{
    return textLeft();
}

Text* Line::textUp(qreal x) const{
    return prev ? Algorithm::textAtSetpoint(*prev, x) : nullptr;
}

Text* Line::textDown(qreal x) const{
    return next ? Algorithm::textAtSetpoint(*next, x) : nullptr;
}

bool Line::isLine() const{
    return true;
}

Line& Line::getLine(){
    return *this;
}

void Line::unfocusLineNumber(){
    line_num_box.setFont(Globals::passive_linenum_font);
    line_num_box.setBrush(Globals::line_num_passive_brush);
    repositionLineNumber();
}

void Line::focusLineNumber(){
    line_num_box.setFont(Globals::active_linenum_font);
    line_num_box.setBrush(Globals::line_num_active_brush);
    repositionLineNumber();
}

bool Line::proceeds(Line& l){
    return line_num < l.line_num;
}

bool Line::proceedsInclusive(Line& l){
    return line_num <= l.line_num;
}

void Line::populateMenu(QMenu&){
    //DO NOTHING
}

void Line::write(QTextStream& out) const{
    writeContents(out);
    out << "\n";
}

void Line::repositionLineNumber(){
    QRectF line_bounds = line_num_box.boundingRect();
    line_num_box.setPos(-(linebox_hspace + line_bounds.width()), u - line_bounds.height()/2);
}

void Line::updateLineboxNumber(){
    line_num_box.setText(QString::number(line_num));
    repositionLineNumber();
}

void link(Line* a, Line* b){
    a->next = b;
    b->prev = a;
    b->setParentItem(a);
    a->repositionNextLine();
}

}

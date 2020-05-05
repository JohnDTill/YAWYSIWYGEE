#include "line.h"

#include "algorithm.h"
#include "construct.h"
#include "typesetscene.h"
#include "globals.h"
#include "text.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

Line::Line(Text* f, Text* b, uint32_t line_num) :
    Phrase(f, b),
    line_num(line_num) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    updateLayout();
}

void Line::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*){
    QRectF bounds;
    QString num_str = QString::number(line_num);
    if(options->state.testFlag(QStyle::StateFlag::State_Selected)){
        painter->setFont(Globals::active_linenum_font);
        painter->setPen(scene()->palette().text().color());
        bounds = Globals::active_linenum_font_metrics.boundingRect(num_str);
    }else{
        painter->setFont(Globals::passive_linenum_font);
        painter->setPen(scene()->palette().mid().color());
        bounds = Globals::passive_linenum_font_metrics.boundingRect(num_str);
    }

    qreal num_x = -(linebox_hspace + bounds.width());
    qreal num_y = u - bounds.height()/2;

    bounds.moveTo(num_x, num_y);
    painter->drawText(bounds, Qt::AlignRight, num_str);
}

QRectF Line::boundingRect() const{
    static constexpr qreal extra = 200;
    return QRectF(-extra, 0, w+extra, u+d);
}

void Line::repositionNextLine(){
    next->setPos(0, u+d+line_vspace);
}

void Line::renumberLine(uint32_t n){
    for(Line* l = this; l; l = l->next)
        l->line_num = n++;
}

void Line::updateToTop(){
    prepareGeometryChange();
    updateLayout();
    if(next) repositionNextLine();
    static_cast<TypesetScene*>(scene())->updateSize();
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

void link(Line* a, Line* b){
    a->next = b;
    b->prev = a;
    b->setParentItem(a);
    a->repositionNextLine();
}

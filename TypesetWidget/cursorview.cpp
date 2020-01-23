#include "cursorview.h"

#include "algorithm.h"
#include "cursor.h"
#include "document.h"
#include "globals.h"
#include "phrase.h"
#include "text.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>

namespace Typeset{

CursorView::CursorView(Document& doc){
    selected_lines.push_back(doc.back);
    doc.back->focusLineNumber();
}

void CursorView::update(const Cursor& cursor){
    for(QGraphicsItem* mask : masks) delete mask;
    masks.clear();

    cursor.text->setTextCursor(cursor.cursor);
    cursor.text->setFocus();

    for(Line* l : selected_lines) l->unfocusLineNumber();
    selected_lines.clear();

    Line* lL;
    Line* lR;
    if(cursor.forward()){
        lL = &cursor.anchor_text->parent->getLine();
        lR = &cursor.text->parent->getLine();
    }else{
        lL = &cursor.text->parent->getLine();
        lR = &cursor.anchor_text->parent->getLine();
    }
    for(Line* l = lL; l != lR->next; l = l->next){
        selected_lines.push_back(l);
        l->focusLineNumber();
    }

    if(cursor.hasSelection()){
        if(cursor.forward()) addMasks(cursor.anchor_text, cursor.anchor_cursor, cursor.text, cursor.cursor);
        else addMasks(cursor.text, cursor.cursor, cursor.anchor_text, cursor.anchor_cursor);
    }
}

void CursorView::addMasks(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR){
    if(tL == tR) addMasksText(tL, cL, tR, cR);
    else if(tL->parent == tR->parent) addMasksPhrase(tL, cL, tR, cR);
    else addMasksMultiline(tL, cL, tR, cR);
}

void CursorView::addMasksText(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR){
    QBrush highlight = QApplication::palette().highlight();

    qreal xL = tL->x() + Algorithm::cursorOffset(*tL, cL);
    qreal xR = tR->x() + Algorithm::cursorOffset(*tR, cR);
    QRectF rect(xL, 0, xR-xL, tL->parent->h());
    SelectionMask* item = new SelectionMask(rect);
    masks.push_back(item);
    item->setParentItem(tL->parent);
}

void CursorView::addMasksPhrase(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR){
    QBrush highlight = QApplication::palette().highlight();

    qreal xL = tL->x() + Algorithm::cursorOffset(*tL, cL);
    qreal xR = tR->x() + Algorithm::cursorOffset(*tR, cR);
    QRectF rect(xL, 0, xR-xL, tL->parent->h());
    SelectionMask* item = new SelectionMask(rect);
    masks.push_back(item);
    item->setParentItem(tL->parent);
}

void CursorView::addMasksMultiline(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR){
    QBrush highlight = QApplication::palette().highlight();

    qreal xL = tL->x() + Algorithm::cursorOffset(*tL, cL);
    QRectF rectL(xL, 0, tL->parent->w + newline_padding - xL, tL->parent->h());
    SelectionMask* itemL = new SelectionMask(rectL);
    masks.push_back(itemL);
    itemL->setParentItem(tL->parent);

    if(tR->prev || !cR.atStart()){
        qreal xR = tR->x() + Algorithm::cursorOffset(*tR, cR);
        QRectF rectR(0, 0, xR, tR->parent->h());
        SelectionMask* itemR = new SelectionMask(rectR);
        masks.push_back(itemR);
        itemR->setParentItem(tR->parent);
    }

    for(Line* l = tL->parent->getLine().next; l != tR->parent; l = l->next){
        QRectF rect(0, 0, l->w + newline_padding, l->h());
        SelectionMask* item = new SelectionMask(rect);
        masks.push_back(item);
        item->setParentItem(l);
    }
}

CursorView::SelectionMask::SelectionMask(const QRectF& r){
    setFlag(GraphicsItemFlag::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    if(!Globals::invert_selection_textcolor) setFlag(GraphicsItemFlag::ItemStacksBehindParent);
    region = QRectF(r.x(), r.y()-margin, r.width()+margin, r.height()+2*margin);
}

void CursorView::SelectionMask::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*){
    if(region.width() < margin + 1e-2) return;
    if(Globals::invert_selection_textcolor)
        painter->setCompositionMode(QPainter::RasterOp_SourceOrNotDestination);
    QBrush b = option->palette.highlight();
    painter->setBrush(b);
    QPen p(option->palette.background().color());
    p.setWidth(-1);
    painter->setPen(p);
    painter->drawRect(region);
}

QRectF CursorView::SelectionMask::boundingRect() const{
    return region;
}

}

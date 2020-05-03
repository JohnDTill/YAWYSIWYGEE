#include "cursorview.h"

#include "algorithm.h"
#include "construct.h"
#include "cursor.h"
#include "typesetscene.h"
#include "globals.h"
#include "phrase.h"
#include "text.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>

CursorView::CursorView(TypesetScene& doc){
    tL_old = doc.front->front;
    tR_old = doc.back->back;
}

void CursorView::update(const Cursor& cursor){
    for(QGraphicsItem* mask : masks) delete mask;
    masks.clear();

    QTextCursor cL_old = tL_old->textCursor();
    cL_old.clearSelection();
    tL_old->setTextCursor(cL_old);

    QTextCursor cR_old = tR_old->textCursor();
    cR_old.clearSelection();
    tR_old->setTextCursor(cR_old);

    cursor.text->setTextCursor(cursor.cursor);
    cursor.text->setFocus();

    Line* lL;
    Line* lR;
    if(cursor.forward()){
        lL = &cursor.anchor_text->parent->getLine();
        lR = &cursor.text->parent->getLine();
    }else{
        lL = &cursor.text->parent->getLine();
        lR = &cursor.anchor_text->parent->getLine();
    }

    cursor.doc.clearSelection();
    cursor.text->parent->getLine().setSelected(true);
    if(cursor.hasSelection()){
        if(cursor.forward()) addMasks(cursor.anchor_text, cursor.anchor_cursor, cursor.text, cursor.cursor);
        else addMasks(cursor.text, cursor.cursor, cursor.anchor_text, cursor.anchor_cursor);

        if(cursor.anchor_text==cursor.text){
            QTextCursor c = cursor.anchor_cursor;
            c.setPosition(cursor.cursor.position(), QTextCursor::KeepAnchor);
            cursor.text->setTextCursor(c);

            tL_old = tR_old = cursor.anchor_text;
        }
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

    for(Text* t = tL; t != tR; t = t->next->next)
        t->next->select();

    for(Text* t = tL->next->next; t != tR; t = t->next->next)
        t->setSelected(true);

    int pL = cL.position();
    cL.movePosition(QTextCursor::End);
    cL.setPosition(pL, QTextCursor::KeepAnchor);
    tL->setTextCursor(cL);
    tL_old = tL;

    int pR = cR.position();
    cR.setPosition(0);
    cR.setPosition(pR, QTextCursor::KeepAnchor);
    tR->setTextCursor(cR);
    tR_old = tR;
}

void CursorView::addMasksMultiline(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR){
    QBrush highlight = QApplication::palette().highlight();

    Line* lL = &tL->parent->getLine();
    Line* lR = &tR->parent->getLine();

    qreal xL = tL->x() + Algorithm::cursorOffset(*tL, cL);
    QRectF rectL(xL, 0, lL->w + newline_padding - xL, lL->h());
    SelectionMask* itemL = new SelectionMask(rectL);
    masks.push_back(itemL);
    itemL->setParentItem(lL);

    if(tR->prev || !cR.atStart()){
        qreal xR = tR->x() + Algorithm::cursorOffset(*tR, cR);
        QRectF rectR(0, 0, xR, lR->h());
        SelectionMask* itemR = new SelectionMask(rectR);
        masks.push_back(itemR);
        itemR->setParentItem(lR);
    }

    for(Line* l = lL->next; l != lR; l = l->next){
        QRectF rect(0, 0, l->w + newline_padding, l->h());
        SelectionMask* item = new SelectionMask(rect);
        masks.push_back(item);
        item->setParentItem(l);
        l->select();
    }

    lL->setSelected(true);
    for(Construct* c = tL->next; c; c = c->next->next){
        c->select();
        c->next->setSelected(true);
    }

    lR->setSelected(true);
    for(Construct* c = tR->prev; c; c = c->prev->prev){
        c->select();
        c->prev->setSelected(true);
    }

    int pL = cL.position();
    if(pL == 0){
        tL->setSelected(true);
    }else{
        cL.movePosition(QTextCursor::End);
        cL.setPosition(pL, QTextCursor::KeepAnchor);
        tL->setTextCursor(cL);
        tL->clearFocus();
    }
    tL_old = tL;

    if(cR.atEnd()){
        tR->setSelected(true);
    }else{
        int pR = cR.position();
        cR.setPosition(0);
        cR.setPosition(pR, QTextCursor::KeepAnchor);
        tR->setTextCursor(cR);
    }
    tR_old = tR;
}

CursorView::SelectionMask::SelectionMask(const QRectF& r){
    setFlag(GraphicsItemFlag::ItemStacksBehindParent);
    region = QRectF(r.x(), r.y()-margin, r.width()+margin, r.height()+2*margin);
}

void CursorView::SelectionMask::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*){
    if(region.width() < margin + 1e-2) return;

    //Note: calling QGraphicsScene::setPalette() does not result in correct option->palette.highlight()
    painter->setBrush(scene()->palette().highlight());
    QPen p(scene()->palette().base().color());
    p.setWidth(0);
    painter->setPen(p);
    painter->drawRect(region);
}

QRectF CursorView::SelectionMask::boundingRect() const{
    return region;
}

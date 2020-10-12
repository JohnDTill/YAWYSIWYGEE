#include "errorview.h"

#include "algorithm.h"
#include "construct.h"
#include "cursor.h"
#include "typesetscene.h"
#include "globals.h"
#include "line.h"
#include "phrase.h"
#include "subphrase.h"
#include "text.h"
#include <QGuiApplication>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

static constexpr ushort ushort_max = std::numeric_limits<ushort>::max();
static const QColor color = QColor::fromRgba64(ushort_max, 0, 0, ushort_max/3);

class ErrorMask : public QGraphicsItem{
private:
    QRectF region;
    static constexpr qreal margin = 1;

public:
    ErrorMask(const QRectF& r, const QString& msg){
        setFlag(GraphicsItemFlag::ItemStacksBehindParent);
        region = QRectF(r.x(), r.y()-margin, r.width()+margin, r.height()+2*margin);
        setToolTip("ERROR: " + msg);
    }

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override final{
        //Note: calling QGraphicsScene::setPalette() does not result in correct option->palette.highlight()

        painter->setBrush(color);
        QPen p(color);
        p.setWidth(1);
        painter->setPen(p);
        painter->drawRoundedRect(region, 4, 4);
    }

    virtual QRectF boundingRect() const override final{
        return region;
    }
};

void ErrorView::clear(){
    for(QGraphicsItem* mask : masks) delete mask;
    masks.clear();
}

void ErrorView::reportError(Text* tL, int pL, Text* tR, int pR, const QString& msg){
    QTextCursor cL = tL->textCursor();
    QTextCursor cR = tR->textCursor();
    cL.setPosition(pL);
    cR.setPosition(pR);

    if(tL == tR) addMasksText(tL, cL, tR, cR, msg);
    else if(tL->parent == tR->parent) addMasksPhrase(tL, cL, tR, cR, msg);
    else addMasksMultiline(tL, cL, tR, cR, msg);
}

void ErrorView::addMasksText(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR, const QString& msg){
    qreal xL = tL->x() + Algorithm::cursorOffset(*tL, cL);
    qreal xR = tR->x() + Algorithm::cursorOffset(*tR, cR);
    if(tL == tR && cR.atStart() && !tL->parent->isLine()) xR = xL + SubPhrase::empty_box_width;
    QRectF rect(xL, 0, xR-xL, tL->parent->h());
    ErrorMask* item = new ErrorMask(rect, msg);
    masks.push_back(item);
    item->setParentItem(tL->parent);
}

void ErrorView::addMasksPhrase(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR, const QString& msg){
    qreal xL = tL->x() + Algorithm::cursorOffset(*tL, cL);
    qreal xR = tR->x() + Algorithm::cursorOffset(*tR, cR);
    QRectF rect(xL, 0, xR-xL, tL->parent->h());
    ErrorMask* item = new ErrorMask(rect, msg);
    masks.push_back(item);
    item->setParentItem(tL->parent);
}

void ErrorView::addMasksMultiline(Text* tL, QTextCursor cL, Text* tR, QTextCursor cR, const QString& msg){
    Q_ASSERT(tL->parent->isLine());
    Q_ASSERT(tR->parent->isLine());

    Line* lL = &tL->parent->getLine();
    Line* lR = &tR->parent->getLine();

    qreal xL = tL->x() + Algorithm::cursorOffset(*tL, cL);
    QRectF rectL(xL, 0, lL->w + newline_padding - xL, lL->h());
    ErrorMask* itemL = new ErrorMask(rectL, msg);
    masks.push_back(itemL);
    itemL->setParentItem(lL);

    if(tR->prev || !cR.atStart()){
        qreal xR = tR->x() + Algorithm::cursorOffset(*tR, cR);
        QRectF rectR(0, 0, xR, lR->h());
        ErrorMask* itemR = new ErrorMask(rectR, msg);
        masks.push_back(itemR);
        itemR->setParentItem(lR);
    }

    for(Line* l = lL->next; l != lR; l = l->next){
        QRectF rect(0, 0, l->w + newline_padding, l->h());
        ErrorMask* item = new ErrorMask(rect, msg);
        masks.push_back(item);
        item->setParentItem(l);
    }
}

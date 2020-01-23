#ifndef GROUPING_H
#define GROUPING_H

#include "construct.h"

namespace Typeset{

static constexpr qreal symbol_width = 6;

class Grouping : public UnaryConstruct{
private:
    void (*drawLeft)(QPainter*, const qreal&);
    void (*drawRight)(QPainter*, const qreal&, const qreal&);
    QChar left_type;
    QChar right_type;

public:
    Grouping(void drawLeft(QPainter*, const qreal&),
             void drawRight(QPainter*, const qreal&, const qreal&),
             QChar left_type,
             QChar right_type,
             SubPhrase* c = nullptr);
    virtual void updateTheme() override final;
    virtual void updateLayout() override final;
    virtual void write(QTextStream& out) const override final;

    static void ANGLE(QPainter* painter, const qreal& height);
    static void ANGLE(QPainter* painter, const qreal& height, const qreal& x);
    static void BAR(QPainter* painter, const qreal& height);
    static void BAR(QPainter* painter, const qreal& height, const qreal& x);
    static void BLANK(QPainter*, const qreal&);
    static void BLANK(QPainter*, const qreal&, const qreal&);
    static void BRACE(QPainter* painter, const qreal& height);
    static void BRACE(QPainter* painter, const qreal& height, const qreal& x);
    static void BRACKET(QPainter* painter, const qreal& height);
    static void BRACKET(QPainter* painter, const qreal& height, const qreal& x);
    static void CEIL(QPainter* painter, const qreal& height);
    static void CEIL(QPainter* painter, const qreal& height, const qreal& x);
    static void DOUBLE_ANGLE(QPainter* painter, const qreal& height);
    static void DOUBLE_ANGLE(QPainter* painter, const qreal& height, const qreal& x);
    static void DOUBLE_BRACKET(QPainter* painter, const qreal& height);
    static void DOUBLE_BRACKET(QPainter* painter, const qreal& height, const qreal& x);
    static void FLOOR(QPainter* painter, const qreal& height);
    static void FLOOR(QPainter* painter, const qreal& height, const qreal& x);
    static void NORM(QPainter* painter, const qreal& height);
    static void NORM(QPainter* painter, const qreal& height, const qreal& x);
    static void PARENTHESIS(QPainter* painter, const qreal& height);
    static void PARENTHESIS(QPainter* painter, const qreal& height, const qreal& x);

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override final;
};

}

#endif // GROUPING_H

#ifndef GROUPING_H
#define GROUPING_H

#include "construct.h"

class Grouping : public UnaryConstruct{
private:
    static constexpr qreal symbol_width = 6;
    void (*drawLeft)(QPainter*, const qreal&);
    void (*drawRight)(QPainter*, const qreal&, const qreal&);
    QChar type;

public:
    Grouping(void drawLeft(QPainter*, const qreal&),
             void drawRight(QPainter*, const qreal&, const qreal&),
             QChar type,
             SubPhrase* c = nullptr);
    virtual void updateLayout() override final;
    virtual void write(QTextStream& out) const override final;

    static void BAR(QPainter* painter, const qreal& height);
    static void BAR(QPainter* painter, const qreal& height, const qreal& x);
    static void BRACKET(QPainter* painter, const qreal& height);
    static void BRACKET(QPainter* painter, const qreal& height, const qreal& x);
    static void CEIL(QPainter* painter, const qreal& height);
    static void CEIL(QPainter* painter, const qreal& height, const qreal& x);
    static void FLOOR(QPainter* painter, const qreal& height);
    static void FLOOR(QPainter* painter, const qreal& height, const qreal& x);
    static void NORM(QPainter* painter, const qreal& height);
    static void NORM(QPainter* painter, const qreal& height, const qreal& x);
    static void PARENTHESIS(QPainter* painter, const qreal& height);
    static void PARENTHESIS(QPainter* painter, const qreal& height, const qreal& x);

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget*) override final;
};

#endif // GROUPING_H

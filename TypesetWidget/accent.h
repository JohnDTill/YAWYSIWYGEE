#ifndef ACCENT_H
#define ACCENT_H

#include "construct.h"

namespace Typeset{

static constexpr qreal accent_height = 3;
static constexpr qreal voffset = 0.5;
static constexpr qreal arrow_width = 2;
static constexpr qreal dot_radius = 0.4;
static constexpr qreal dot_offset = 2.5;

class Accent : public UnaryConstruct{
private:
    void (*drawAccent)(QPainter*, const qreal&);

public:
    Accent(void (*drawAccent)(QPainter*, const qreal&), SubPhrase* c = nullptr);
    virtual void updateTheme() override final;
    virtual void updateLayout() override final;
    virtual void write(QTextStream& out) const override final;

    static void ARROW(QPainter* painter, const qreal& width);
    static void BREVE(QPainter* painter, const qreal& width);
    static void DOT(QPainter* painter, const qreal& width);
    static void DDOT(QPainter* painter, const qreal& width);
    static void DDDOT(QPainter* painter, const qreal& width);
    static void HAT(QPainter* painter, const qreal& width);
    static void OVERBAR(QPainter* painter, const qreal& width);
    static void TILDE(QPainter* painter, const qreal& width);

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override final;
};

}

#endif // ACCENT_H

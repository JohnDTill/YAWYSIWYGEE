#ifndef GLOBALS_H
#define GLOBALS_H

#include "MathBran/include/QMathBran_Macros.h"
#include <QGraphicsItem>
#include <QtGlobal>
class QFont;
class QFontMetrics;
class QPen;

static constexpr qreal line_vspace = 4;
static constexpr qreal linebox_offet = 10;
static constexpr int spaces_per_tab = 4;

enum TypesetType{
    LINE = QGraphicsItem::UserType + 1,
    SUBPHRASE = QGraphicsItem::UserType + 2,
    TEXT = QGraphicsItem::UserType + 3,
    CONSTRUCT = QGraphicsItem::UserType + 4,
};

class Globals{
    public:
        static bool vertical_integrals;
        static QPen empty_box_pen;

        static QFont bigqchar_font;
        static QFontMetrics bigqchar_font_metrics;
        static QFont word_font;
        static QFontMetrics word_font_metrics;
        static constexpr uint8_t deepest_script_level = 2;
        static constexpr int font_sizes[deepest_script_level+1] = {15, 12, 11};
        static QFont fonts[deepest_script_level+1];
        static QFontMetrics font_metrics[deepest_script_level+1];
        static QFont passive_linenum_font;
        static QFont active_linenum_font;
        static QFontMetrics passive_linenum_font_metrics;
        static QFontMetrics active_linenum_font_metrics;
        static void initGlobals();

        static QFont glyph_font;
        static QFont bigint_font;
        static QFontMetrics bigint_font_metrics;
};

#endif // GLOBALS_H

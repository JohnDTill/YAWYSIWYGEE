#ifndef GLOBALS_H
#define GLOBALS_H

#include <QFontDatabase>
#include <QPen>

#include <QDebug>
#define DO_THIS(message) \
    qDebug() << "DO THIS: " << message << ", file " << __FILE__ << " line " << __LINE__;

#include <QMessageBox>
#define FATAL_ERROR(message) {\
    QMessageBox messageBox; \
    messageBox.critical(nullptr, "Error", QString("Fatal Error: ") + message); \
    messageBox.setFixedSize(500,200); \
    exit(0);}

namespace Typeset{

static constexpr qreal line_vspace = 4;
static constexpr qreal linebox_width = 50;
static constexpr qreal linebox_offet = 10;
static constexpr qreal horizontal_scroll_padding = 8;
static constexpr qreal linebox_hspace = 20;
static constexpr qreal margin_top = 3;
static constexpr qreal margin_bot = 8;
static const QChar ESCAPE = 8284; //⁜
static const QChar OPEN = 9204; //⏴
static const QChar CLOSE = 9205; //⏵

class Globals{
    public:
        static QBrush background_brush;
        static QColor construct_color;
        static QBrush construct_brush;
        static QPen construct_pen;
        static QColor left_pane_color;
        static QPen left_pane_pen;
        static QBrush left_pane_brush;
        static QColor text_color;
        static QBrush line_num_active_brush;
        static QBrush line_num_passive_brush;
        static QPen empty_box_pen;
        static bool invert_selection_textcolor;

        static QFont bigqchar_font;
        static QFont integral_font;
        static QFont word_font;
        static constexpr uint8_t deepest_script_level = 2;
        static constexpr int font_sizes[deepest_script_level+1] = {15, 12, 11};
        static QFont fonts[deepest_script_level+1];
        static QFont passive_linenum_font;
        static QFont active_linenum_font;
        static void initGlobals();
};

}

#endif // GLOBALS_H

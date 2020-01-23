#include "globals.h"

namespace Typeset{

QBrush Globals::background_brush = QBrush(Qt::GlobalColor::white);
QColor Globals::construct_color = Qt::GlobalColor::black;
QBrush Globals::construct_brush = QBrush(construct_color);
QPen Globals::construct_pen = QPen(construct_color);
QColor Globals::left_pane_color = QColor::fromRgbF(0.9,0.9,0.9);
QPen Globals::left_pane_pen = QPen(left_pane_color);
QBrush Globals::left_pane_brush = QBrush(left_pane_color, Qt::SolidPattern);
QColor Globals::text_color = Qt::GlobalColor::black;
QBrush Globals::line_num_active_brush = QBrush(Qt::black);
QBrush Globals::line_num_passive_brush = QBrush(Qt::darkGray);
QPen Globals::empty_box_pen;
bool Globals::invert_selection_textcolor = true;

QFont Globals::bigqchar_font;
QFont Globals::integral_font;
QFont Globals::word_font;
constexpr int Globals::font_sizes[];
QFont Globals::fonts[];
QFont Globals::passive_linenum_font;
QFont Globals::active_linenum_font;

void Globals::initGlobals(){
    int id = QFontDatabase::addApplicationFont(":/Font/Quivira.otf");
    if(id==-1) FATAL_ERROR("Failed to load font.")

    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont default_font = QFont(family);

    bigqchar_font = default_font;
    bigqchar_font.setPointSize(24);

    integral_font = default_font;
    integral_font.setPointSize(25);
    integral_font.setItalic(true);

    word_font = default_font;
    word_font.setPointSize(15);

    for(uint8_t i = 0; i <= deepest_script_level; i++){
        fonts[i] = default_font;
        fonts[i].setPointSize(font_sizes[i]);
    }

    empty_box_pen = QPen(Qt::PenStyle::DotLine);
    empty_box_pen.setDashPattern({1,2});
    empty_box_pen.setColor(construct_color);

    passive_linenum_font = fonts[0];
    active_linenum_font = passive_linenum_font;
    active_linenum_font.setBold(true);
}

}

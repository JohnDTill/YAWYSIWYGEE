#include "globals.h"

inline void initResources(){
    Q_INIT_RESOURCE(qtypesetobjectresource);
}

bool Globals::vertical_integrals = false;

QPen Globals::empty_box_pen;

QFont Globals::bigqchar_font;
QFontMetrics Globals::bigqchar_font_metrics = QFontMetrics(bigqchar_font);
QFont Globals::integral_font;
QFontMetrics Globals::integral_font_metrics = QFontMetrics(integral_font);
QFont Globals::word_font;
QFontMetrics Globals::word_font_metrics = QFontMetrics(word_font);
constexpr int Globals::font_sizes[];
QFont Globals::fonts[];
QFont Globals::passive_linenum_font;
QFont Globals::active_linenum_font;
QFontMetrics Globals::passive_linenum_font_metrics = QFontMetrics(Globals::passive_linenum_font);
QFontMetrics Globals::active_linenum_font_metrics = QFontMetrics(Globals::active_linenum_font);
QFontMetrics Globals::font_metrics[] = {QFontMetrics(Globals::fonts[0]),
                                        QFontMetrics(Globals::fonts[1]),
                                        QFontMetrics(Globals::fonts[2])};

void Globals::initGlobals(){
    initResources();
    int id = QFontDatabase::addApplicationFont(":/Font/Quivira.otf");
    if(id==-1) FATAL_ERROR("Failed to load font.")

    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont default_font = QFont(family);

    bigqchar_font = default_font;
    bigqchar_font.setPixelSize(24);
    bigqchar_font_metrics = QFontMetrics(bigqchar_font);

    integral_font = default_font;
    integral_font.setPixelSize(25);
    integral_font_metrics = QFontMetrics(integral_font);

    word_font = default_font;
    word_font.setPixelSize(15);
    word_font_metrics = QFontMetrics(word_font);

    for(uint8_t i = 0; i <= deepest_script_level; i++){
        fonts[i] = default_font;
        fonts[i].setPixelSize(font_sizes[i]);
        font_metrics[i] = QFontMetrics(fonts[i]);
    }

    empty_box_pen = QPen(Qt::PenStyle::DotLine);
    empty_box_pen.setDashPattern({1,2});

    passive_linenum_font = fonts[0];
    active_linenum_font = passive_linenum_font;
    active_linenum_font.setBold(true);

    Globals::passive_linenum_font_metrics = QFontMetrics(passive_linenum_font);
    Globals::active_linenum_font_metrics = QFontMetrics(active_linenum_font);
}

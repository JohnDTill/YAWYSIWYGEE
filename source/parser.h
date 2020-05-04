#ifndef TYPESETLOADER_H
#define TYPESETLOADER_H

#include <stdint.h>
#include <tuple>
class Line;
class Text;
class TypesetScene;
class QString;
class QTextStream;

namespace Parser{
    TypesetScene* parseDocument(const QString& source, bool allow_write = true, bool show_line_numbers = true);
    std::pair<Text*,Text*> parsePhrase(const QString& source, uint8_t script_level = 0);
    std::pair<Line*,Line*> parseMultiline(const QString& source, uint32_t line_num = 1);
};

#endif // TYPESETLOADER_H

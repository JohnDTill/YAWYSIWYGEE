#ifndef TYPESETLOADER_H
#define TYPESETLOADER_H

#include <QString>
#include <QTextStream>

class QPainter;

class Construct;
class TypesetScene;
class Line;
class SubPhrase;
class Text;

class Parser{

public:
    static TypesetScene* parseDocument(QTextStream& source, bool allow_write = true, bool show_line_numbers = true);
    static std::pair<Text*,Text*> parsePhrase(const QString& source, uint8_t script_level = 0);
    static std::pair<Line*,Line*> parseMultiline(const QString& source, uint32_t line_num = 1);

private:
    static void consume(const QString& source, QString::size_type& curr, QChar c);
    static bool match(const QString& source, QString::size_type& curr, QChar c);
    static bool peek(const QString& source, const QString::size_type& curr, QChar c);
    static bool scanToCloseSymbol(const QString& source, QString::size_type& curr);
    static bool matchEscapeSequence(const QString& source, QString::size_type& curr);
    static int parseInteger(const QString& source, QString::size_type& curr, int minimum_value = 1);

    static Line* parseLine(const QString& source, QString::size_type& curr, uint8_t& script_level, const uint32_t& line_num);
    static SubPhrase* parseSubPhrase(const QString& source, QString::size_type& curr, uint8_t& script_level, uint32_t child_id = 0);
    static Text* parseTextInSubPhrase(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Text* parseTextInLine(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseConstruct(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseFraction(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseMatrix(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseRoot(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseSuperscript(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseSubscript(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseDualscript(const QString& source, QString::size_type& curr, uint8_t& script_level, bool eval = false);
    static Construct* parseBigQChar(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseIntegral(const QString& source, QString::size_type& curr, uint8_t& script_level, bool allow_superscript);
    static Construct* parseUnderscriptedWord(QString word, const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseLimit(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseBinomial(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseCases(const QString& source, QString::size_type& curr, uint8_t& script_level);
    static Construct* parseGrouping(void (*LEFT_SYMBOL)(QPainter*, const qreal&), void (*RIGHT_SYMBOL)(QPainter*, const qreal&, const qreal&), const QString& source, QString::size_type& curr, uint8_t& script_level);
    template<void AccentType(QPainter*,const qreal&)> static Construct* parseAccent(const QString& source, QString::size_type& curr, uint8_t& script_level);
};

#endif // TYPESETLOADER_H

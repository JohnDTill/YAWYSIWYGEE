#include "parser.h"

#include "typesetscene.h"
#include "line.h"
#include "text.h"
#include "MathBran/include/QMathBran.h"

#include "construct/accent.h"
#include "construct/bigqchar.h"
#include "construct/binomial.h"
#include "construct/cases.h"
#include "construct/fraction.h"
#include "construct/grouping.h"
#include "construct/integral.h"
#include "construct/limit.h"
#include "construct/matrix.h"
#include "construct/root.h"
#include "construct/script.h"
#include "construct/underscriptedword.h"

#include <QMessageBox>
#define PARSER_ERROR(message) {\
    QMessageBox messageBox; \
    messageBox.critical(nullptr, "Error", QString("Fatal Error: ") + message + "\n" \
                                 "Source:\n" + \
                                 source); \
    messageBox.setFixedSize(500,200); \
    exit(0);}

static SubPhrase* parseSubPhrase(const QString& source,
                                 QString::size_type& curr,
                                 uint8_t& script_level,
                                 uint32_t child_id = 0);

static void consume(const QString& source, QString::size_type& curr, QChar c){
    if(curr >= source.size() || source[curr++] != c){
        if(curr >= source.size()){
            PARSER_ERROR("parser reached end of source without terminating")
        }else{
            curr--;
            PARSER_ERROR( "parser expected '" + c + "', recieved '" + source[curr] +
                          "' at position " + QString::number(curr))
        }
    }
}

static bool match(const QString& source, QString::size_type& curr, QChar c){
    if(curr < source.size() && source[curr] == c){
        curr++;
        return true;
    }else{
        return false;
    }
}

static bool peek(const QString& source, const QString::size_type& curr, QChar c){
    return (curr < source.size() && source[curr] == c);
}

static bool scanToCloseSymbol(const QString& source, QString::size_type& curr){
    bool escaped = false;

    for(;;){
        if(curr == source.size()){
            return false;
        }else if(source[curr] == MB_CLOSE && !escaped){
            curr++;
            return true;
        }else if(source[curr] == MB_CONSTRUCT_SYMBOL){
            escaped = !escaped;
        }else{
            escaped = false;
        }

        curr++;
    }
}

static bool matchEscapeSequence(const QString& source, QString::size_type& curr){
    if(curr >= source.size()-1) return false;
    if(source[curr] != MB_CONSTRUCT_SYMBOL) return false;

    switch(source[curr+1].unicode()){
        case MB_USHORT_CONSTRUCT_SYMBOL:
            curr += 2;
            return true;
        case MB_USHORT_OPEN:
            curr += 2;
            return true;
        case MB_USHORT_CLOSE:
            curr += 2;
            return true;
        default:
            return false;
    }
}

static int parseInteger(const QString& source, QString::size_type& curr, int minimum_value){
    consume(source, curr, MB_OPEN);
    int start = curr;
    if(!scanToCloseSymbol(source, curr))
        PARSER_ERROR( "parser reached end of file while scanning for close symbol")

    QString number_string = source.mid(start, curr-start-1);

    bool parse_success;
    int val = number_string.toInt(&parse_success);

    if(!parse_success)
        PARSER_ERROR( "failed to parse integer, invalid number: " + number_string)
    else if(val < minimum_value)
        PARSER_ERROR( "Parsed integer argument of " + number_string +
                     ", minimum value is " + QString::number(minimum_value))

    return val;
}

static Construct* parseFraction(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* num = parseSubPhrase(source, curr, script_level);
    return new Fraction(num, parseSubPhrase(source, curr, script_level));
}

static Construct* parseMatrix(const QString &source, QString::size_type& curr, uint8_t& script_level){
    int rows = parseInteger(source, curr, 1);
    int cols = parseInteger(source, curr, 1);

    std::vector<SubPhrase*>::size_type size =
            static_cast<std::vector<SubPhrase*>::size_type>(rows*cols);

    std::vector<SubPhrase*> phrases;
    phrases.resize(size);

    if(peek(source,curr,MB_OPEN)){
        for(std::vector<SubPhrase*>::size_type i = 0; i < size; i++)
            phrases[i] = parseSubPhrase(source, curr, script_level, static_cast<uint32_t>(i));
    }else{
        for(std::vector<SubPhrase*>::size_type i = 0; i < size; i++){
            Text* t = new Text(script_level);
            phrases[i] = new SubPhrase(t);
        }
    }

    return new Matrix(phrases,
                      static_cast<minor_integer>(rows),
                      static_cast<minor_integer>(cols));
}

static Construct* parseRoot(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* child = parseSubPhrase(source, curr, script_level);

    if(peek(source, curr, MB_OPEN)){
        script_level++;
        SubPhrase* script = parseSubPhrase(source, curr, script_level);
        script_level--;
        return new ScriptedRoot(child, script);
    }else{
        return new SquareRoot(child);
    }
}

static Construct* parseSuperscript(const QString& source, QString::size_type& curr, uint8_t& script_level){
    bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
    if(!deepest_script_level) script_level++;
    SubPhrase* script = parseSubPhrase(source, curr, script_level);
    if(!deepest_script_level) script_level--;

    return new Superscript(script);
}

static Construct* parseSubscript(const QString& source, QString::size_type& curr, uint8_t& script_level){
    bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
    if(!deepest_script_level) script_level++;
    SubPhrase* script = parseSubPhrase(source, curr, script_level);
    if(!deepest_script_level) script_level--;

    return new Subscript(script);
}

static Construct* parseDualscript(const QString& source,
                                  QString::size_type& curr,
                                  uint8_t& script_level,
                                  bool eval){
    bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
    if(!deepest_script_level) script_level++;
    SubPhrase* sub = parseSubPhrase(source, curr, script_level);
    SubPhrase* sup = parseSubPhrase(source, curr, script_level);
    if(!deepest_script_level) script_level--;

    return new Dualscript(sub, sup, eval);
}

static Construct* parseBigQChar(const QString& source, QString::size_type& curr, uint8_t& script_level){
    QChar qchar = source[curr-1];

    if(peek(source, curr, MB_OPEN)){
        script_level++;
        SubPhrase* underscript = parseSubPhrase(source, curr, script_level);

        if(peek(source, curr, MB_OPEN)){
            SubPhrase* overscript = parseSubPhrase(source, curr, script_level);
            script_level--;
            return new BigQChar_SN(qchar, underscript, overscript);
        }else{
            script_level--;
            return new BigQChar_S(qchar, underscript);
        }
    }else{
        return new BigQChar(qchar);
    }
}

static Construct* parseIntegral(const QString& source,
                                QString::size_type& curr,
                                uint8_t& script_level,
                                bool allow_superscript){
    QChar qchar = source[curr-1];

    if(peek(source, curr, MB_OPEN)){
        script_level++;
        SubPhrase* subscript = parseSubPhrase(source, curr, script_level);

        if(allow_superscript && peek(source, curr, MB_OPEN)){
            SubPhrase* superscript = parseSubPhrase(source, curr, script_level);
            script_level--;
            return new Integral_SN(qchar, subscript, superscript);
        }else{
            script_level--;
            return new Integral_S(qchar, subscript, allow_superscript);
        }
    }else{
        return new Integral(qchar, allow_superscript);
    }
}

static Construct* parseUnderscriptedWord(QString word,
                                         const QString& source,
                                         QString::size_type& curr,
                                         uint8_t& script_level){
    QChar code = source[curr-1];
    script_level++;
    SubPhrase* underscript = parseSubPhrase(source, curr, script_level);
    script_level--;

    return new UnderscriptedWord(word, code, underscript);
}

static Construct* parseLimit(const QString& source, QString::size_type& curr, uint8_t& script_level){
    script_level++;
    SubPhrase* lhs = parseSubPhrase(source, curr, script_level);
    SubPhrase* rhs = parseSubPhrase(source, curr, script_level);
    script_level--;

    return new Limit(lhs, rhs);
}

static Construct* parseBinomial(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* top = parseSubPhrase(source, curr, script_level);
    return new Binomial(top, parseSubPhrase(source, curr, script_level));
}

static Construct* parseCases(const QString& source, QString::size_type& curr, uint8_t& script_level){
    std::vector<SubPhrase*> data;

    do{
        data.push_back( parseSubPhrase(source, curr, script_level) );
        data.push_back( parseSubPhrase(source, curr, script_level) );
    } while(peek(source, curr, MB_OPEN));

    return new Cases(data);
}

static Construct* parseGrouping(void (*LEFT_SYMBOL)(QPainter*, const qreal&),
                                 void (*RIGHT_SYMBOL)(QPainter*, const qreal&, const qreal&),
                                 const QString& source,
                                 QString::size_type& curr,
                                 uint8_t& script_level){
    QChar type = source[curr-1];
    return new Grouping(LEFT_SYMBOL, RIGHT_SYMBOL, type, parseSubPhrase(source, curr, script_level));
}

static Construct* parseConstruct(const QString& source, QString::size_type& curr, uint8_t& script_level){
    switch(source[curr++].unicode()){
        case MB_USHORT_ACCENT_ARROW: return new Accent(Accent::ARROW, parseSubPhrase(source, curr, script_level));
        case MB_USHORT_ACCENT_BREVE: return new Accent(Accent::BREVE, parseSubPhrase(source, curr, script_level));
        case MB_USHORT_ACCENT_DOT: return new Accent(Accent::DOT, parseSubPhrase(source, curr, script_level));
        case MB_USHORT_ACCENT_DOUBLE_DOTS:
            return new Accent(Accent::DDOT, parseSubPhrase(source, curr, script_level));
        case MB_USHORT_ACCENT_TRIPLE_DOTS:
            return new Accent(Accent::DDDOT, parseSubPhrase(source, curr, script_level));
        case MB_USHORT_ACCENT_HAT: return new Accent(Accent::HAT, parseSubPhrase(source, curr, script_level));
        case MB_USHORT_ACCENT_BAR: return new Accent(Accent::OVERBAR, parseSubPhrase(source, curr, script_level));
        case MB_USHORT_ACCENT_TILDE: return new Accent(Accent::TILDE, parseSubPhrase(source, curr, script_level));
        case MB_USHORT_SUMMATION: return parseBigQChar(source, curr, script_level);
        case MB_USHORT_PRODUCT: return parseBigQChar(source, curr, script_level);
        case MB_USHORT_COPRODUCT: return parseBigQChar(source, curr, script_level);
        case MB_USHORT_INTERSECTION: return parseBigQChar(source, curr, script_level);
        case MB_USHORT_UNION: return parseBigQChar(source, curr, script_level);
        case MB_USHORT_UNION_PLUS: return parseBigQChar(source, curr, script_level);
        case MB_USHORT_BINOMIAL_COEFFICIENTS:  return parseBinomial(source, curr, script_level);
        case MB_USHORT_CASES:  return parseCases(source, curr, script_level);
        case MB_USHORT_FRACTION:  return parseFraction(source, curr, script_level);
        case MB_USHORT_GROUPING_PARENTHESIS:
            return parseGrouping(Grouping::PARENTHESIS, Grouping::PARENTHESIS, source, curr, script_level);
        case MB_USHORT_GROUPING_BRACKETS:
            return parseGrouping(Grouping::BRACKET, Grouping::BRACKET, source, curr, script_level);
        case MB_USHORT_GROUPING_BARS:
            return parseGrouping(Grouping::BAR, Grouping::BAR, source, curr, script_level);
        case MB_USHORT_GROUPING_DOUBLE_BARS:
            return parseGrouping(Grouping::NORM, Grouping::NORM, source, curr, script_level);
        case MB_USHORT_GROUPING_CEIL:
            return parseGrouping(Grouping::CEIL, Grouping::CEIL, source, curr, script_level);
        case MB_USHORT_GROUPING_FLOOR:
            return parseGrouping(Grouping::FLOOR, Grouping::FLOOR, source, curr, script_level);
        case MB_USHORT_INTEGRAL: return parseIntegral(source, curr, script_level, true);
        case MB_USHORT_DOUBLE_INTEGRAL: return parseIntegral(source, curr, script_level, false);
        case MB_USHORT_TRIPLE_INTEGRAL: return parseIntegral(source, curr, script_level, false);
        case MB_USHORT_CONTOUR_INTEGRAL: return parseIntegral(source, curr, script_level, true);
        case MB_USHORT_CLOSED_SURFACE_INTEGRAL: return parseIntegral(source, curr, script_level, false);
        case MB_USHORT_CLOSED_VOLUME_INTEGRAL: return parseIntegral(source, curr, script_level, false);
        case MB_USHORT_MATRIX: return parseMatrix(source, curr, script_level);
        case MB_USHORT_ROOT: return parseRoot(source, curr, script_level);
        case MB_USHORT_SUBSCRIPT:  return parseSubscript(source, curr, script_level);
        case MB_USHORT_SUPERSCRIPT:  return parseSuperscript(source, curr, script_level);
        case MB_USHORT_DUALSCRIPT:  return parseDualscript(source, curr, script_level, false);
        case MB_USHORT_EVALSCRIPT: return parseDualscript(source, curr, script_level, true);
        case MB_USHORT_UNDERSCRIPTED_MAX: return parseUnderscriptedWord("max", source, curr, script_level);
        case MB_USHORT_UNDERSCRIPTED_MIN: return parseUnderscriptedWord("min", source, curr, script_level);
        case MB_USHORT_UNDERSCRIPTED_SUP: return parseUnderscriptedWord("sup", source, curr, script_level);
        case MB_USHORT_UNDERSCRIPTED_INF: return parseUnderscriptedWord("inf", source, curr, script_level);
        case MB_USHORT_LIMIT: return parseLimit(source, curr, script_level);
        default: PARSER_ERROR("invalid construct code: " + source[--curr])
    }
}

static Text* parseTextInSubPhrase(const QString& source, QString::size_type& curr, uint8_t& script_level){
    int start = curr;

    for(;;){
        if(peek(source, curr, MB_CLOSE)) break;
        else if(peek(source, curr, MB_CONSTRUCT_SYMBOL) && !matchEscapeSequence(source, curr)) break;
        else if(peek(source, curr, MB_OPEN)) PARSER_ERROR("unexpected '" + MB_OPEN + "' in code")
        else if(++curr >= source.size()) PARSER_ERROR("parser reached end of source without terminating")
    }

    QString str = source.mid(start, curr - start);
    MathBran::removeEscapes(str);

    return new Text(script_level, str);
}

static SubPhrase* parseSubPhrase(const QString& source,
                                 QString::size_type& curr,
                                 uint8_t& script_level,
                                 uint32_t child_id){
    consume(source, curr, MB_OPEN);

    Text* front = parseTextInSubPhrase(source, curr, script_level);
    Text* text = front;

    while( !match(source, curr, MB_CLOSE) ){
        consume(source, curr, MB_CONSTRUCT_SYMBOL);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInSubPhrase(source, curr, script_level);
        link(construct, text);

        construct->prev->startSignalToNext();
    }

    return new SubPhrase(front, text, child_id);
}

static Text* parseTextInLine(const QString& source, QString::size_type& curr, uint8_t& script_level){
    int start = curr;

    while(curr < source.size()){
        if(peek(source, curr, MB_CONSTRUCT_SYMBOL) && !matchEscapeSequence(source, curr)) break;
        else if(peek(source, curr, MB_OPEN)) PARSER_ERROR("unexpected '" + MB_OPEN + "' in code")
        else if(peek(source, curr, MB_CLOSE)) PARSER_ERROR("unexpected '" + MB_CLOSE + "' in code")
        else curr++;
    }

    QString str = source.mid(start, curr - start);
    MathBran::removeEscapes(str);

    return new Text(script_level, str);
}

static Line* parseLine(const QString& source,
                       QString::size_type& curr,
                       uint8_t& script_level,
                       const uint32_t& line_num){
    Text* front = parseTextInLine(source, curr, script_level);
    Text* text = front;

    while( curr < source.size() ){
        consume(source, curr, MB_CONSTRUCT_SYMBOL);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInLine(source, curr, script_level);
        link(construct, text);

        construct->prev->startSignalToNext();
    }

    return new Line(front, text, line_num);
}

TypesetScene* Parser::parseDocument(QTextStream& source, bool allow_write, bool show_line_numbers){
    source.seek(0);

    QString line = source.readLine();
    QString::size_type curr = 0;
    uint8_t script_level = 0;
    uint32_t line_num = 1;
    Line* front = parseLine(line, curr, script_level, line_num);
    Line* l = front;
    line = source.readLine();

    while(!line.isNull()){
        curr = 0;
        line_num++;
        Line* next = parseLine(line, curr, script_level, line_num);
        link(l, next);
        l = next;
        line = source.readLine();
    }

    return new TypesetScene(allow_write, show_line_numbers, front, l);
}

std::pair<Text*, Text*> Parser::parsePhrase(const QString& source, uint8_t script_level){
    QString::size_type curr = 0;
    Text* front = parseTextInLine(source, curr, script_level);
    Text* text = front;

    while( curr < source.size() ){
        consume(source, curr, MB_CONSTRUCT_SYMBOL);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInLine(source, curr, script_level);
        link(construct, text);
    }

    return std::pair<Text*,Text*>(front,text);
}

std::pair<Line*, Line*> Parser::parseMultiline(const QString& source, uint32_t line_num){
    QStringList lines = source.split('\n');

    QString::size_type curr = 0;
    uint8_t script_level = 0;
    Line* front = parseLine(lines.front(), curr, script_level, line_num);
    Line* l = front;

    for(int i = 1; i < lines.size(); i++){
        QString line = lines[i];
        curr = 0;
        line_num++;
        Line* next = parseLine(line, curr, script_level, line_num);
        link(l, next);
        l = next;
    }

    return std::pair<Line*, Line*>(front, l);
}

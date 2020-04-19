#include "parser.h"

#include "document.h"
#include "line.h"
#include "text.h"

#include "accent.h"
#include "bigqchar.h"
#include "binomial.h"
#include "cases.h"
#include "fraction.h"
#include "grouping.h"
#include "integral.h"
#include "limit.h"
#include "matrix.h"
#include "root.h"
#include "script.h"
#include "underscriptedword.h"

#include <QStringList>
#include <vector>

#include <QMessageBox>
#define PARSER_ERROR(message) {\
    QMessageBox messageBox; \
    messageBox.critical(nullptr, "Error", QString("Fatal Error: ") + message + "\n" \
                                 "Source:\n" + \
                                 source); \
    messageBox.setFixedSize(500,200); \
    exit(0);}

namespace Typeset{

static constexpr ushort ESCAPE_UNICODE = 8284;
static constexpr ushort OPEN_UNICODE = 9204;
static constexpr ushort CLOSE_UNICODE = 9205;
static const QString ESCAPED_ESCAPE = QString(ESCAPE) + ESCAPE;
static const QString ESCAPED_OPEN = QString(ESCAPE) + OPEN;
static const QString ESCAPED_CLOSE = QString(ESCAPE) + CLOSE;

bool Parser::containsConstruct(const QString& source){
    for(QString::size_type curr = 0; curr < source.size(); curr++){
        if(source[curr] == ESCAPE){
            if(++curr >= source.size()) return false;
            else if(source[curr] == ESCAPE) continue;
            else if(source[curr] == OPEN) continue;
            else if(source[curr] == CLOSE) continue;
            else return true;
        }
    }

    return false;
}

bool Parser::isValidCode(const QString& source){
    QStringList lines = source.split('\n');
    if(lines.last().isEmpty()) lines.removeLast();
    if(lines.isEmpty()) return false;

    for(QString line : lines){
        QString::size_type curr = 0;
        if( !validateLine(line, curr) ) return false;
    }

    return true;
}

bool Parser::isValidCode(QTextStream& source){
    source.seek(0);

    QString line = source.readLine();
    while(!line.isNull()){
        QString::size_type curr = 0;
        if( !validateLine(line, curr) ) return false;
        line = source.readLine();
    }

    return true;
}

bool Parser::shouldParseAsCode(const QString& source){
    return containsConstruct(source) && isValidCode(source);
}

Document* Parser::parseDocument(QTextStream& source, bool allow_write, bool show_line_numbers){
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

    return new Document(allow_write, show_line_numbers, front, l);
}

std::pair<Text*, Text*> Parser::parsePhrase(const QString& source, uint8_t script_level){
    QString::size_type curr = 0;
    Text* front = parseTextInLine(source, curr, script_level);
    Text* text = front;

    while( curr < source.size() ){
        consume(source, curr, ESCAPE);

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

void Parser::consume(const QString& source, QString::size_type& curr, QChar c){
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

bool Parser::match(const QString& source, QString::size_type& curr, QChar c){
    if(curr < source.size() && source[curr] == c){
        curr++;
        return true;
    }else{
        return false;
    }
}

bool Parser::peek(const QString& source, const QString::size_type& curr, QChar c){
    return (curr < source.size() && source[curr] == c);
}

void Parser::step(const QString& source, QString::size_type& curr){
    if(++curr >= source.size())
        PARSER_ERROR("parser reached end of source without terminating")
}

void Parser::consumeToCloseBracket(const QString& source, QString::size_type& curr){
    if(!scanToCloseSymbol(source, curr))
        PARSER_ERROR( "parser reached end of file while scanning for close symbol")
}

bool Parser::scanToCloseSymbol(const QString& source, QString::size_type& curr){
    bool escaped = false;

    for(;;){
        if(curr == source.size()){
            return false;
        }else if(source[curr] == CLOSE && !escaped){
            curr++;
            return true;
        }else if(source[curr] == ESCAPE){
            escaped = !escaped;
        }else{
            escaped = false;
        }

        curr++;
    }
}

bool Parser::matchEscapeChar(const QString& source, QString::size_type& curr){
    if(curr >= source.size()) return false;

    switch(source[curr].unicode()){
        case ESCAPE_UNICODE:
            curr++;
            return true;
        case OPEN_UNICODE:
            curr++;
            return true;
        case CLOSE_UNICODE:
            curr++;
            return true;
        default:
            return false;
    }
}

bool Parser::matchEscapeSequence(const QString& source, QString::size_type& curr){
    if(curr >= source.size()-1) return false;
    if(source[curr] != ESCAPE) return false;

    switch(source[curr+1].unicode()){
        case ESCAPE_UNICODE:
            curr += 2;
            return true;
        case OPEN_UNICODE:
            curr += 2;
            return true;
        case CLOSE_UNICODE:
            curr += 2;
            return true;
        default:
            return false;
    }
}

QString Parser::applyEscapes(QString& text){
    return text.replace(ESCAPE, ESCAPED_ESCAPE)
               .replace(OPEN, ESCAPED_OPEN)
               .replace(CLOSE, ESCAPED_CLOSE);
}

QString Parser::removeEscapes(QString& text){
    return text.replace(ESCAPED_ESCAPE, QString(ESCAPE))
               .replace(ESCAPED_OPEN, QString(OPEN))
               .replace(ESCAPED_CLOSE, QString(CLOSE));
}

bool Parser::validateLine(const QString& source, QString::size_type& curr){
    while( curr < source.size() ){
        if( match(source, curr, ESCAPE) && !matchEscapeChar(source, curr) ){
            if( !validateConstruct(source, curr) ) return false;
        }else if(match(source, curr, OPEN)){
            return false;
        }else if(match(source, curr, CLOSE)){
            return false;
        }else{
            curr++;
        }
    }

    return true;
}

bool Parser::validateSubPhrase(const QString& source, QString::size_type& curr){
    if( !match(source, curr, OPEN) ) return false;

    while( !match(source, curr, CLOSE) ){
        if(curr >= source.size()){
            return false;
        }else if( match(source, curr, ESCAPE) && !matchEscapeChar(source, curr) ){
            if( !validateConstruct(source, curr) ) return false;
        }else if(match(source, curr, OPEN)){
            return false;
        }else{
            curr++;
        }
    }

    return true;
}

bool Parser::validateSubPhrases(const QString& source, QString::size_type& curr, const uint32_t num_phrases){
    for(uint32_t i = 0; i < num_phrases; i++)
        if( !validateSubPhrase(source,curr) ) return false;

    return true;
}

bool Parser::validateConstruct(const QString& source, QString::size_type& curr){
    if(curr == source.size()) return false;

    switch(source[curr++].unicode()){
        case 8594: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //→
        case 259:  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //ă
        case 551:  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //ȧ
        case 228:  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //ä
        case 8943: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //⋯
        case 226:  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //â
        case 257:  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //ā
        case 227:  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //ã
        case 8721: return validateIntegralOrBigQChar(source, curr); //∑
        case 8719: return validateIntegralOrBigQChar(source, curr); //∏
        case 8720: return validateIntegralOrBigQChar(source, curr); //∐
        case 8898: return validateIntegralOrBigQChar(source, curr); //⋂
        case 8899: return validateIntegralOrBigQChar(source, curr); //⋃
        case 10756: return validateIntegralOrBigQChar(source, curr); //⨄
        case 'b':  return !peek(source, curr, OPEN) || validateSubPhrases(source, curr, 2);
        case 'c':  return validateCases(source, curr);
        case 'f':  return !peek(source, curr, OPEN) || validateSubPhrases(source, curr, 2);
        case '(':  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case '[':  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case '{':  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case '|':  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case 8214: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //‖
        case 9482: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //┊
        case 10216: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //⟨
        case 8968: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //⌈
        case 8970: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //⌊
        case 10218: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //⟪
        case 10214: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //⟦
        case 8747: return validateIntegralOrBigQChar(source, curr); //∫
        case 8748: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //∬
        case 8749: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //∭
        case 8750: return validateIntegralOrBigQChar(source, curr); //∮
        case 8751: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //∯
        case 8752: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr); //∰
        case 8862: return validateMatrix(source, curr); //⊞
        case 8730: return validateRoot(source, curr); //√
        case '^':  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case '_':  return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case 916:  return !peek(source, curr, OPEN) || validateSubPhrases(source, curr, 2); //Δ
        case 8593: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case 8595: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case 8599: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case 8600: return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
        case 'l': return !peek(source, curr, OPEN) || validateSubPhrases(source, curr, 2);
        default:   return false;
    }
}

bool Parser::validateRoot(const QString& source, QString::size_type& curr){
    if(!peek(source, curr, OPEN)) return true;
    if(!validateSubPhrase(source, curr)) return false;
    if(peek(source, curr, OPEN)) return validateSubPhrase(source, curr);
    else return true;
}

bool Parser::validateIntegralOrBigQChar(const QString& source, QString::size_type& curr){
    if( peek(source, curr, OPEN) && !validateSubPhrase(source, curr) ) return false;
    if( peek(source, curr, OPEN) && !validateSubPhrase(source, curr) ) return false;

    return true;
}

bool Parser::validateCases(const QString& source, QString::size_type& curr){
    if(!peek(source, curr, OPEN)) return true;

    do{
        if(!validateSubPhrases(source, curr, 2)) return false;
    } while(peek(source, curr, OPEN));

    return true;
}

bool Parser::validateMatrix(const QString& source, QString::size_type& curr){
    int rows, cols;
    if( !peek(source, curr, OPEN) ) return true;
    if( !parseInteger(rows, source, curr) ) return false;
    if( !peek(source, curr, OPEN) ) return true;
    if( !parseInteger(cols, source, curr) ) return false;
    if( !peek(source, curr, OPEN) ) return true;

    return validateSubPhrases(source, curr, static_cast<uint32_t>(rows)*static_cast<uint32_t>(cols));
}

int Parser::parseInteger(const QString& source, QString::size_type& curr, int minimum_value){
    consume(source, curr, OPEN);
    int start = curr;
    consumeToCloseBracket(source, curr);

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

bool Parser::parseInteger(int& val, const QString& source, QString::size_type& curr, int minimum_value){
    if( !match(source, curr, OPEN) ) return false;
    int start = curr;
    if( !scanToCloseSymbol(source, curr) ) return false;
    bool parse_success;
    val = source.mid(start, curr-start-1).toInt(&parse_success);

    return parse_success && val >= minimum_value;
}

QString Parser::parseQString(const QString& source, QString::size_type& curr){
    consume(source, curr, OPEN);
    int start = curr;
    consumeToCloseBracket(source, curr);

    return source.mid(start, curr-start-1);
}

Text* Parser::parseTextInSubPhrase(const QString& source, QString::size_type& curr, uint8_t& script_level){
    int start = curr;

    for(;;){
        if(peek(source, curr, CLOSE)) break;
        else if(peek(source, curr, ESCAPE) && !matchEscapeSequence(source, curr)) break;
        else if(peek(source, curr, OPEN)) PARSER_ERROR("unexpected '" + OPEN + "' in code")
        else step(source, curr);
    }

    QString str = source.mid(start, curr - start);
    removeEscapes(str);

    return new Text(script_level, str);
}

Text* Parser::parseTextInLine(const QString& source, QString::size_type& curr, uint8_t& script_level){
    int start = curr;

    while(curr < source.size()){
        if(peek(source, curr, ESCAPE) && !matchEscapeSequence(source, curr)) break;
        else if(peek(source, curr, OPEN)) PARSER_ERROR("unexpected '" + OPEN + "' in code")
        else if(peek(source, curr, CLOSE)) PARSER_ERROR("unexpected '" + CLOSE + "' in code")
        else curr++;
    }

    QString str = source.mid(start, curr - start);
    removeEscapes(str);

    return new Text(script_level, str);
}

SubPhrase* Parser::parseSubPhrase(const QString& source, QString::size_type& curr, uint8_t& script_level, uint32_t child_id){
    consume(source, curr, OPEN);

    Text* front = parseTextInSubPhrase(source, curr, script_level);
    Text* text = front;

    while( !match(source, curr, CLOSE) ){
        consume(source, curr, ESCAPE);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInSubPhrase(source, curr, script_level);
        link(construct, text);

        construct->prev->startSignalToNext();
    }

    return new SubPhrase(front, text, child_id);
}

Line* Parser::parseLine(const QString& source, QString::size_type& curr, uint8_t& script_level, const uint32_t& line_num){
    Text* front = parseTextInLine(source, curr, script_level);
    Text* text = front;

    while( curr < source.size() ){
        consume(source, curr, ESCAPE);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInLine(source, curr, script_level);
        link(construct, text);

        construct->prev->startSignalToNext();
    }

    return new Line(front, text, line_num);
}

Construct* Parser::parseConstruct(const QString& source, QString::size_type& curr, uint8_t& script_level){
    switch(source[curr++].unicode()){
        case 8594: return parseAccent<Accent::ARROW>(source, curr, script_level); //→
        case 259: return parseAccent<Accent::BREVE>(source, curr, script_level); //ă
        case 551: return parseAccent<Accent::DOT>(source, curr, script_level); //ȧ
        case 228: return parseAccent<Accent::DDOT>(source, curr, script_level); //ä
        case 8943: return parseAccent<Accent::DDDOT>(source, curr, script_level); //⋯
        case 226: return parseAccent<Accent::HAT>(source, curr, script_level); //â
        case 257: return parseAccent<Accent::OVERBAR>(source, curr, script_level); //ā
        case 227: return parseAccent<Accent::TILDE>(source, curr, script_level); //ã
        case 8721: return parseBigQChar(source, curr, script_level); //∑
        case 8719: return parseBigQChar(source, curr, script_level); //∏
        case 8720: return parseBigQChar(source, curr, script_level); //∐
        case 8898: return parseBigQChar(source, curr, script_level); //⋂
        case 8899: return parseBigQChar(source, curr, script_level); //⋃
        case 10756: return parseBigQChar(source, curr, script_level); //⨄
        case 'b':  return parseBinomial(source, curr, script_level);
        case 'c':  return parseCases(source, curr, script_level);
        case 'f':  return parseFraction(source, curr, script_level);
        case '(':  return parseGrouping(Grouping::PARENTHESIS, Grouping::PARENTHESIS, source, curr, script_level);
        case '[':  return parseGrouping(Grouping::BRACKET, Grouping::BRACKET, source, curr, script_level);
        case '{':  return parseGrouping(Grouping::BRACE, Grouping::BRACE, source, curr, script_level);
        case '|':  return parseGrouping(Grouping::BAR, Grouping::BAR, source, curr, script_level);
        case 8214: return parseGrouping(Grouping::NORM, Grouping::NORM, source, curr, script_level); //‖
        case 9482: return parseGrouping(Grouping::BLANK, Grouping::BAR, source, curr, script_level); //┊
        case 10216: return parseGrouping(Grouping::ANGLE, Grouping::ANGLE, source, curr, script_level); //⟨
        case 8968: return parseGrouping(Grouping::CEIL, Grouping::CEIL, source, curr, script_level); //⌈
        case 8970: return parseGrouping(Grouping::FLOOR, Grouping::FLOOR, source, curr, script_level); //⌊
        case 10218: return parseGrouping(Grouping::DOUBLE_ANGLE, Grouping::DOUBLE_ANGLE, source, curr, script_level); //⟪
        case 10214: return parseGrouping(Grouping::DOUBLE_BRACKET, Grouping::DOUBLE_BRACKET, source, curr, script_level); //⟦
        case 8747: return parseIntegral(source, curr, script_level, true); //∫
        case 8748: return parseIntegral(source, curr, script_level, false); //∬
        case 8749: return parseIntegral(source, curr, script_level, false); //∭
        case 8750: return parseIntegral(source, curr, script_level, true); //∮
        case 8751: return parseIntegral(source, curr, script_level, false); //∯
        case 8752: return parseIntegral(source, curr, script_level, false); //∰
        case 8862: return parseMatrix(source, curr, script_level); //⊞
        case 8730: return parseRoot(source, curr, script_level); //√
        case '_':  return parseSubscript(source, curr, script_level);
        case '^':  return parseSuperscript(source, curr, script_level);
        case 916:  return parseDualscript(source, curr, script_level); //Δ
        case 8593: return parseUnderscriptedWord("max", source, curr, script_level);
        case 8595: return parseUnderscriptedWord("min", source, curr, script_level);
        case 8599: return parseUnderscriptedWord("sup", source, curr, script_level);
        case 8600: return parseUnderscriptedWord("inf", source, curr, script_level);
        case 'l': return parseLimit(source, curr, script_level);
        default:   PARSER_ERROR("invalid construct code: " + source[--curr])
    }
}

Construct* Parser::parseFraction(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* num;
    SubPhrase* den;
    if(peek(source, curr, OPEN)){
        num = parseSubPhrase(source, curr, script_level);
        den = parseSubPhrase(source, curr, script_level);
    }else{
        num = new SubPhrase(new Text(script_level));
        den = new SubPhrase(new Text(script_level));
    }

    return new Fraction(num, den);
}

Construct* Parser::parseMatrix(const QString &source, QString::size_type& curr, uint8_t& script_level){
    int rows, cols;
    if(peek(source, curr, OPEN)){
        rows = parseInteger(source, curr);
        if(peek(source, curr, OPEN)){
            cols = parseInteger(source, curr);
        }else{
            cols = 1;
        }
    }else{
        rows = cols = 3;
    }

    std::vector<SubPhrase*>::size_type size =
            static_cast<std::vector<SubPhrase*>::size_type>(rows*cols);

    std::vector<SubPhrase*> phrases;
    phrases.resize(size);

    if(peek(source,curr,OPEN)){
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

Construct* Parser::parseRoot(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* child;
    if(peek(source, curr, OPEN)){
        child = parseSubPhrase(source, curr, script_level);
    }else{
        Text* t = new Text(script_level);
        child = new SubPhrase(t);
    }

    if(peek(source, curr, OPEN)){
        script_level++;
        SubPhrase* script = parseSubPhrase(source, curr, script_level);
        script_level--;
        return new ScriptedRoot(child, script);
    }else{
        return new SquareRoot(child);
    }
}

Construct* Parser::parseSuperscript(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* script;
    if(peek(source, curr, OPEN)){
        bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
        if(!deepest_script_level) script_level++;
        script = parseSubPhrase(source, curr, script_level);
        if(!deepest_script_level) script_level--;
    }else{
        bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
        if(!deepest_script_level) script_level++;
        script = new SubPhrase(new Text(script_level));
        if(!deepest_script_level) script_level--;
    }

    return new Superscript(script);
}

Construct* Parser::parseSubscript(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* script;
    if(peek(source, curr, OPEN)){
        bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
        if(!deepest_script_level) script_level++;
        script = parseSubPhrase(source, curr, script_level);
        if(!deepest_script_level) script_level--;
    }else{
        bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
        if(!deepest_script_level) script_level++;
        script = new SubPhrase(new Text(script_level));
        if(!deepest_script_level) script_level--;
    }

    return new Subscript(script);
}

Construct* Parser::parseDualscript(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* sub;
    SubPhrase* sup;
    if(peek(source, curr, OPEN)){
        bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
        if(!deepest_script_level) script_level++;
        sub = parseSubPhrase(source, curr, script_level);
        sup = parseSubPhrase(source, curr, script_level);
        if(!deepest_script_level) script_level--;
    }else{
        bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
        if(!deepest_script_level) script_level++;
        sub = new SubPhrase(new Text(script_level));
        sup = new SubPhrase(new Text(script_level));
        if(!deepest_script_level) script_level--;
    }

    return new Dualscript(sub, sup);
}

Construct* Parser::parseBigQChar(const QString& source, QString::size_type& curr, uint8_t& script_level){
    QChar qchar = source[curr-1];

    if(peek(source, curr, OPEN)){
        script_level++;
        SubPhrase* underscript = parseSubPhrase(source, curr, script_level);

        if(peek(source, curr, OPEN)){
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

Construct* Parser::parseIntegral(const QString& source, QString::size_type& curr, uint8_t& script_level, bool allow_superscript){
    QChar qchar = source[curr-1];

    if(peek(source, curr, OPEN)){
        script_level++;
        SubPhrase* subscript = parseSubPhrase(source, curr, script_level);

        if(allow_superscript && peek(source, curr, OPEN)){
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

Construct* Parser::parseUnderscriptedWord(QString word, const QString& source, QString::size_type& curr, uint8_t& script_level){
    QChar code = source[curr-1];

    script_level++;
    SubPhrase* underscript;
    if(peek(source, curr, OPEN)){
        underscript = parseSubPhrase(source, curr, script_level);
    }else{
        underscript = new SubPhrase(new Text(script_level));
    }
    script_level--;

    return new UnderscriptedWord(word, code, underscript);
}

Construct* Parser::parseLimit(const QString& source, QString::size_type& curr, uint8_t& script_level){
    script_level++;
    SubPhrase* lhs;
    SubPhrase* rhs;
    if(peek(source, curr, OPEN)){
        lhs = parseSubPhrase(source, curr, script_level);
        rhs = parseSubPhrase(source, curr, script_level);
    }else{
        lhs = new SubPhrase(new Text(script_level));
        rhs = new SubPhrase(new Text(script_level));
    }
    script_level--;

    return new Limit(lhs, rhs);
}

Construct* Parser::parseBinomial(const QString& source, QString::size_type& curr, uint8_t& script_level){
    if(peek(source, curr, OPEN)){
        SubPhrase* top = parseSubPhrase(source, curr, script_level);
        return new Binomial(top, parseSubPhrase(source, curr, script_level));
    }else{
        return new Binomial(new SubPhrase(new Text(script_level)), new SubPhrase(new Text(script_level)));
    }
}

Construct* Parser::parseCases(const QString& source, QString::size_type& curr, uint8_t& script_level){
    std::vector<SubPhrase*> data;

    if(!peek(source, curr, OPEN)){
        for(int i = 0; i < 4; i++)
            data.push_back( new SubPhrase(new Text(script_level)) );

        return new Cases(data);
    }else{
        do{
            data.push_back( parseSubPhrase(source, curr, script_level) );
            data.push_back( parseSubPhrase(source, curr, script_level) );
        } while(peek(source, curr, OPEN));

        return new Cases(data);
    }
}

Construct* Parser::parseGrouping(void (*LEFT_SYMBOL)(QPainter*, const qreal&),
                                 void (*RIGHT_SYMBOL)(QPainter*, const qreal&, const qreal&),
                                 const QString& source,
                                 QString::size_type& curr,
                                 uint8_t& script_level){
    QChar type = source[curr-1];

    SubPhrase* child;
    if(peek(source,curr,OPEN)){
        child = parseSubPhrase(source, curr, script_level);
    }else{
        child = new SubPhrase(new Text(script_level));
    }

    return new Grouping(LEFT_SYMBOL, RIGHT_SYMBOL, type, child);
}

template<void AccentType(QPainter*,const qreal&)> Construct* Parser::parseAccent(const QString& source, QString::size_type& curr, uint8_t& script_level){
    if(peek(source,curr,OPEN))
        return new Accent( AccentType, parseSubPhrase(source, curr, script_level) );
    else
        return new Accent( AccentType, new SubPhrase(new Text(script_level)) );
}

}

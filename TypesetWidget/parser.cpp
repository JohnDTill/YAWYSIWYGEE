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
static const QStringList underscripted_word_whitelist = {"lim", "max", "min", "sup", "inf"};

//DO THIS: better error handling in parser, e.g. error messages for failed isValidCode,
//         line numbers, error highlighting, etc...
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
    l->prev = nullptr;
    line = source.readLine();

    while(!line.isNull()){
        curr = 0;
        line_num++;
        Line* next = parseLine(line, curr, script_level, line_num);
        link(l, next);
        l = next;
        line = source.readLine();
    }
    l->next = nullptr;

    return new Document(allow_write, show_line_numbers, front, l);
}

std::pair<Text*, Text*> Parser::parsePhrase(const QString& source, uint8_t script_level){
    QString::size_type curr = 0;
    Text* front = parseTextInLine(source, curr, script_level);
    Text* text = front;
    text->prev = nullptr;

    while( curr < source.size() ){
        consume(source, curr, ESCAPE);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInLine(source, curr, script_level);
        link(construct, text);
    }
    text->next = nullptr;

    return std::pair<Text*,Text*>(front,text);
}

std::pair<Line*, Line*> Parser::parseMultiline(const QString& source, uint32_t line_num){
    QStringList lines = source.split('\n');

    QString::size_type curr = 0;
    uint8_t script_level = 0;
    Line* front = parseLine(lines.front(), curr, script_level, line_num);
    Line* l = front;
    l->prev = nullptr;

    for(int i = 1; i < lines.size(); i++){
        QString line = lines[i];
        curr = 0;
        line_num++;
        Line* next = parseLine(line, curr, script_level, line_num);
        link(l, next);
        l = next;
    }
    l->next = nullptr;

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
        case '(':  return validateGrouping(source, curr);
        case '[':  return validateGrouping(source, curr);
        case '{':  return validateGrouping(source, curr);
        case '|':  return validateGrouping(source, curr);
        case 8214: return validateGrouping(source, curr); //‖
        case 9482: return validateGrouping(source, curr); //┊
        case 10216: return validateGrouping(source, curr); //⟨
        case 8968: return validateGrouping(source, curr); //⌈
        case 8970: return validateGrouping(source, curr); //⌊
        case 10218: return validateGrouping(source, curr); //⟪
        case 10214: return validateGrouping(source, curr); //⟦
        case 8747: return validateIntegralOrBigQChar(source, curr); //∫
        case 8748: return validateIntegralOrBigQChar(source, curr); //∬
        case 8749: return validateIntegralOrBigQChar(source, curr); //∭
        case 8750: return validateIntegralOrBigQChar(source, curr); //∮
        case 8751: return validateIntegralOrBigQChar(source, curr); //∯
        case 8752: return validateIntegralOrBigQChar(source, curr); //∰
        case 8862: return validateMatrix(source, curr); //⊞
        case 8730: return validateRoot(source, curr); //√
        case '^':  return validateSubPhrases(source, curr, 2);
        case '_':  return validateSubPhrases(source, curr, 2);
        case 916:  return validateSubPhrases(source, curr, 3); //Δ
        case 'w':  return validateUnderscriptedWord(source, curr);
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

bool Parser::validateUnderscriptedWord(const QString& source, QString::size_type& curr){
    if(!match(source, curr, OPEN)) return false;
    int start = curr;
    if(!scanToCloseSymbol(source, curr)) return false;

    QString word = source.mid(start, curr-start-1);
    if(!underscripted_word_whitelist.contains(word)) return false;

    return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
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

bool Parser::validateGrouping(const QString& source, QString::size_type& curr){
    if(curr >= source.size()) return false;

    switch(source[curr++].unicode()){
        case ')':  break;
        case ']':  break;
        case '}':  break;
        case '|':  break;
        case 8214: break; //‖
        case 9482: break; //┊
        case 10217: break; //⟩
        case 8969: break; //⌉
        case 8971: break; //⌋
        case 10219: break; //⟫
        case 10215: break; //⟧
        default:   return false;
    }

    return !peek(source, curr, OPEN) || validateSubPhrase(source, curr);
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
    text->prev = nullptr;

    while( !match(source, curr, CLOSE) ){
        consume(source, curr, ESCAPE);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInSubPhrase(source, curr, script_level);
        link(construct, text);
    }
    text->next = nullptr;

    return new SubPhrase(front, text, child_id);
}

Line* Parser::parseLine(const QString& source, QString::size_type& curr, uint8_t& script_level, const uint32_t& line_num){
    Text* front = parseTextInLine(source, curr, script_level);
    Text* text = front;
    text->prev = nullptr;

    while( curr < source.size() ){
        consume(source, curr, ESCAPE);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInLine(source, curr, script_level);
        link(construct, text);
    }
    text->next = nullptr;

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
        case '(':  return parseGrouping(Grouping::PARENTHESIS, source, curr, script_level);
        case '[':  return parseGrouping(Grouping::BRACKET,source, curr, script_level);
        case '{':  return parseGrouping(Grouping::BRACE, source, curr, script_level);
        case '|':  return parseGrouping(Grouping::BAR, source, curr, script_level);
        case 8214: return parseGrouping(Grouping::NORM, source, curr, script_level); //‖
        case 9482: return parseGrouping(Grouping::BLANK, source, curr, script_level); //┊
        case 10216: return parseGrouping(Grouping::ANGLE, source, curr, script_level); //⟨
        case 8968: return parseGrouping(Grouping::CEIL, source, curr, script_level); //⌈
        case 8970: return parseGrouping(Grouping::FLOOR, source, curr, script_level); //⌊
        case 10218: return parseGrouping(Grouping::DOUBLE_ANGLE, source, curr, script_level); //⟪
        case 10214: return parseGrouping(Grouping::DOUBLE_BRACKET, source, curr, script_level); //⟦
        case 8747: return parseIntegral(source, curr, script_level); //∫
        case 8748: return parseIntegral(source, curr, script_level); //∬
        case 8749: return parseIntegral(source, curr, script_level); //∭
        case 8750: return parseIntegral(source, curr, script_level); //∮
        case 8751: return parseIntegral(source, curr, script_level); //∯
        case 8752: return parseIntegral(source, curr, script_level); //∰
        case 8862: return parseMatrix(source, curr, script_level); //⊞
        case 8730: return parseRoot(source, curr, script_level); //√
        case '_':  return parseSubscript(source, curr, script_level);
        case '^':  return parseSuperscript(source, curr, script_level);
        case 916:  return parseDualscript(source, curr, script_level); //Δ
        case 'w':  return parseUnderscriptedWord(source, curr, script_level);
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
        Text* tn = new Text(script_level);
        Text* td = new Text(script_level);
        tn->next = tn->prev = td->next = td->prev = nullptr;
        num = new SubPhrase(tn);
        den = new SubPhrase(td);
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
            t->next = t->prev = nullptr;
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
        t->next = t->prev = nullptr;
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
    SubPhrase* main = parseSubPhrase(source, curr, script_level);
    bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
    if(!deepest_script_level) script_level++;
    SubPhrase* script = parseSubPhrase(source, curr, script_level);
    if(!deepest_script_level) script_level--;

    return new Superscript(main, script);
}

Construct* Parser::parseSubscript(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* main = parseSubPhrase(source, curr, script_level);
    bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
    if(!deepest_script_level) script_level++;
    SubPhrase* script = parseSubPhrase(source, curr, script_level);
    if(!deepest_script_level) script_level--;

    return new Subscript(main, script);
}

Construct* Parser::parseDualscript(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* main = parseSubPhrase(source, curr, script_level);
    bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
    if(!deepest_script_level) script_level++;
    SubPhrase* sub = parseSubPhrase(source, curr, script_level);
    SubPhrase* sup = parseSubPhrase(source, curr, script_level);
    if(!deepest_script_level) script_level--;

    return new Dualscript(main, sub, sup);
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

Construct* Parser::parseIntegral(const QString& source, QString::size_type& curr, uint8_t& script_level){
    QChar qchar = source[curr-1];

    if(peek(source, curr, OPEN)){
        script_level++;
        SubPhrase* subscript = parseSubPhrase(source, curr, script_level);

        if(peek(source, curr, OPEN)){
            SubPhrase* superscript = parseSubPhrase(source, curr, script_level);
            script_level--;
            return new Integral_SN(qchar, subscript, superscript);
        }else{
            script_level--;
            return new Integral_S(qchar, subscript);
        }
    }else{
        return new Integral(qchar);
    }
}

Construct* Parser::parseUnderscriptedWord(const QString& source, QString::size_type& curr, uint8_t& script_level){
    QString word = parseQString(source, curr);
    if(!underscripted_word_whitelist.contains(word))
        PARSER_ERROR("UnderscriptedWord whitelist does not contain \"" + word + "\"")

    script_level++;
    SubPhrase* underscript;
    if(peek(source, curr, OPEN)){
        underscript = parseSubPhrase(source, curr, script_level);
    }else{
        Text* t = new Text(script_level);
        t->next = t->prev = nullptr;
        underscript = new SubPhrase(t);
    }
    script_level--;

    return new UnderscriptedWord(word, underscript);
}

Construct* Parser::parseBinomial(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* top;
    SubPhrase* bot;
    if(peek(source, curr, OPEN)){
        top = parseSubPhrase(source, curr, script_level);
        bot = parseSubPhrase(source, curr, script_level);
    }else{
        Text* tn = new Text(script_level);
        Text* td = new Text(script_level);
        tn->next = tn->prev = td->next = td->prev = nullptr;
        top = new SubPhrase(tn);
        bot = new SubPhrase(td);
    }

    return new Binomial(top, bot);
}

Construct* Parser::parseCases(const QString& source, QString::size_type& curr, uint8_t& script_level){
    std::vector<SubPhrase*> data;

    if(!peek(source, curr, OPEN)){
        for(int i = 0; i < 4; i++){
            Text* t = new Text(script_level);
            t->next = t->prev = nullptr;
            data.push_back( new SubPhrase(t) );
        }

        return new Cases(data);
    }else{
        do{
            data.push_back( parseSubPhrase(source, curr, script_level) );
            data.push_back( parseSubPhrase(source, curr, script_level) );
        } while(peek(source, curr, OPEN));

        return new Cases(data);
    }
}

Construct* Parser::parseGrouping(void (*LEFT_SYMBOL)(QPainter*, const qreal&), const QString& source, QString::size_type& curr, uint8_t& script_level){
    if(curr >= source.size()) PARSER_ERROR("Reached end-of-file before right Grouping symbol")

    void (*RIGHT_SYMBOL)(QPainter*, const qreal&, const qreal&);
    switch(source[curr++].unicode()){
        case ')':  RIGHT_SYMBOL = Grouping::PARENTHESIS; break;
        case ']':  RIGHT_SYMBOL = Grouping::BRACKET; break;
        case '}':  RIGHT_SYMBOL = Grouping::BRACE; break;
        case '|':  RIGHT_SYMBOL = Grouping::BAR; break;
        case 8214: RIGHT_SYMBOL = Grouping::NORM; break; //‖
        case 9482: RIGHT_SYMBOL = Grouping::BLANK; break; //┊
        case 10217: RIGHT_SYMBOL = Grouping::ANGLE; break; //⟩
        case 8969: RIGHT_SYMBOL = Grouping::CEIL; break; //⌉
        case 8971: RIGHT_SYMBOL = Grouping::FLOOR; break; //⌋
        case 10219: RIGHT_SYMBOL = Grouping::DOUBLE_ANGLE; break; //⟫
        case 10215: RIGHT_SYMBOL = Grouping::DOUBLE_BRACKET; break; //⟧
        default: PARSER_ERROR("Invalid right grouping")
    }

    QChar left_type = source[curr-2];
    QChar right_type = source[curr-1];

    SubPhrase* child;
    if(peek(source,curr,OPEN)){
        child = parseSubPhrase(source, curr, script_level);
    }else{
        Text* t = new Text(script_level);
        t->next = t->prev = nullptr;
        child = new SubPhrase(t);
    }

    return new Grouping(LEFT_SYMBOL, RIGHT_SYMBOL, left_type, right_type, child);
}

template<void AccentType(QPainter*,const qreal&)> Construct* Parser::parseAccent(const QString& source, QString::size_type& curr, uint8_t& script_level){
    SubPhrase* child;
    if(peek(source,curr,OPEN)){
        child = parseSubPhrase(source, curr, script_level);
    }else{
        Text* t = new Text(script_level);
        t->next = t->prev = nullptr;
        child = new SubPhrase(t);
    }

    return new Accent( AccentType, child );
}

}

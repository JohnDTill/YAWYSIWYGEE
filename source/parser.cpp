#include "parser.h"

#include "globals.h"
#include "line.h"
#include "subphrase.h"
#include "text.h"
#include "typesetscene.h"
#include "construct/constructs.h"
#include "MathBran/include/QMathBran.h"

//Note: A precondition of using the parser is that the source is valid MathBran.
//      Each public parser function uses Q_ASSERT(MathBran::isWellFormed(source)) to check this.
//      All other Q_ASSERT calls detect bugs in the parser (or MathBran validation).

inline void consume(const QString& source, int& curr, QChar c){
    Q_ASSERT(curr < source.size());
    Q_ASSERT(source[curr] == c);
    curr++;
}

static bool matchEscapeSequence(const QString& source, int& curr){
    Q_ASSERT(curr+1 < source.size());
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

static ushort parseDimension(const QString& source, int& curr){
    consume(source, curr, MB_OPEN);
    int dim_start = curr;
    while(++curr < source.size() && source[curr] != MB_CLOSE);
    Q_ASSERT(curr < source.size());
    bool success;
    ushort val = source.midRef(dim_start, curr - dim_start).toUShort(&success);
    Q_ASSERT(success && val > 0 || val <= 255);
    curr++;

    return val;
}

static SubPhrase* subphrase(const QString& source, int& curr, uint8_t& script_level, uint32_t child_id = 0);

static Construct* fraction(const QString& source, int& curr, uint8_t& script_level){
    SubPhrase* num = subphrase(source, curr, script_level);
    return new Fraction(num, subphrase(source, curr, script_level));
}

static Construct* matrix(const QString &source, int& curr, uint8_t& script_level){
    ushort rows = parseDimension(source, curr);
    ushort cols = parseDimension(source, curr);
    ushort size = rows*cols;
    std::vector<SubPhrase*> phrases(size);

    if(curr < source.size() && source[curr] == MB_OPEN)
        for(ushort i = 0; i < size; i++) phrases[i] = subphrase(source, curr, script_level, i);
    else
        for(ushort i = 0; i < size; i++) phrases[i] = new SubPhrase(new Text(script_level));

    return new Matrix(phrases, rows, cols);
}

static Construct* root(const QString& source, int& curr, uint8_t& script_level){
    SubPhrase* child = subphrase(source, curr, script_level);

    if(curr < source.size() && source[curr] == MB_OPEN){
        script_level++;
        SubPhrase* script = subphrase(source, curr, script_level);
        script_level--;
        return new ScriptedRoot(child, script);
    }else{
        return new SquareRoot(child);
    }
}

static Construct* superscript(const QString& source, int& curr, uint8_t& script_level){
    bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
    if(!deepest_script_level) script_level++;
    SubPhrase* script = subphrase(source, curr, script_level);
    if(!deepest_script_level) script_level--;

    return new Superscript(script);
}

static Construct* subscript(const QString& source, int& curr, uint8_t& script_level){
    bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
    if(!deepest_script_level) script_level++;
    SubPhrase* script = subphrase(source, curr, script_level);
    if(!deepest_script_level) script_level--;

    return new Subscript(script);
}

static Construct* dualscript(const QString& source, int& curr, uint8_t& script_level, bool eval){
    bool deepest_script_level = Text::isDeepestScriptLevel(script_level);
    if(!deepest_script_level) script_level++;
    SubPhrase* sub = subphrase(source, curr, script_level);
    SubPhrase* sup = subphrase(source, curr, script_level);
    if(!deepest_script_level) script_level--;

    return new Dualscript(sub, sup, eval);
}

static Construct* bigSym(const QString& source, int& curr, uint8_t& script_level, bool allow_overscript){
    QChar qchar = source[curr-1];
    if(curr < source.size() && source[curr] == MB_OPEN){
        script_level++;
        SubPhrase* underscript = subphrase(source, curr, script_level);

        if(allow_overscript && curr < source.size() && source[curr] == MB_OPEN){
            SubPhrase* overscript = subphrase(source, curr, script_level);
            script_level--;
            return new BigQChar_SN(qchar, underscript, overscript);
        }else{
            script_level--;
            return new BigQChar_S(qchar, underscript, allow_overscript);
        }
    }else{
        return new BigQChar(qchar, allow_overscript);
    }
}

static Construct* integral(const QString& source, int& curr, uint8_t& script_level, bool allow_superscript){
    if(Globals::vertical_integrals) return bigSym(source, curr, script_level, allow_superscript);

    QChar qchar = source[curr-1];
    if(curr < source.size() && source[curr] == MB_OPEN){
        script_level++;
        SubPhrase* subscript = subphrase(source, curr, script_level);

        if(allow_superscript && curr < source.size() && source[curr] == MB_OPEN){
            SubPhrase* superscript = subphrase(source, curr, script_level);
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

static Construct* underscriptWord(const QString& word, const QString& source, int& curr, uint8_t& script_level){
    QChar code = source[curr-1];
    script_level++;
    SubPhrase* underscript = subphrase(source, curr, script_level);
    script_level--;

    return new UnderscriptedWord(word, code, underscript);
}

static Construct* limit(const QString& source, int& curr, uint8_t& script_level){
    script_level++;
    SubPhrase* lhs = subphrase(source, curr, script_level);
    SubPhrase* rhs = subphrase(source, curr, script_level);
    script_level--;

    return new Limit(lhs, rhs);
}

static Construct* binomial(const QString& source, int& curr, uint8_t& script_level){
    SubPhrase* top = subphrase(source, curr, script_level);
    return new Binomial(top, subphrase(source, curr, script_level));
}

static Construct* cases(const QString& source, int& curr, uint8_t& script_level){
    std::vector<SubPhrase*> data;

    do{
        data.push_back( subphrase(source, curr, script_level) );
        data.push_back( subphrase(source, curr, script_level) );
    } while(curr < source.size() && source[curr] == MB_OPEN);

    return new Cases(data);
}

static Construct* group(void (*LEFT_SYMBOL)(QPainter*, const qreal&),
                        void (*RIGHT_SYMBOL)(QPainter*, const qreal&, const qreal&),
                        const QString& source,
                        int& curr,
                        uint8_t& script_level){
    QChar type = source[curr-1];
    return new Group(LEFT_SYMBOL, RIGHT_SYMBOL, type, subphrase(source, curr, script_level));
}

static Construct* parseConstruct(const QString& source, int& curr, uint8_t& script_level){
    switch(source[curr++].unicode()){
    case MB_USHORT_ACCENT_ARROW: return new Accent(Accent::ARROW, subphrase(source, curr, script_level));
    case MB_USHORT_ACCENT_BREVE: return new Accent(Accent::BREVE, subphrase(source, curr, script_level));
    case MB_USHORT_ACCENT_DOT: return new Accent(Accent::DOT, subphrase(source, curr, script_level));
    case MB_USHORT_ACCENT_DOUBLE_DOTS: return new Accent(Accent::DDOT, subphrase(source, curr, script_level));
    case MB_USHORT_ACCENT_TRIPLE_DOTS: return new Accent(Accent::DDDOT, subphrase(source, curr, script_level));
    case MB_USHORT_ACCENT_HAT: return new Accent(Accent::HAT, subphrase(source, curr, script_level));
    case MB_USHORT_ACCENT_BAR: return new Accent(Accent::OVERBAR, subphrase(source, curr, script_level));
    case MB_USHORT_ACCENT_TILDE: return new Accent(Accent::TILDE, subphrase(source, curr, script_level));
    case MB_USHORT_SUMMATION: return bigSym(source, curr, script_level, true);
    case MB_USHORT_PRODUCT: return bigSym(source, curr, script_level, true);
    case MB_USHORT_COPRODUCT: return bigSym(source, curr, script_level, true);
    case MB_USHORT_INTERSECTION: return bigSym(source, curr, script_level, true);
    case MB_USHORT_UNION: return bigSym(source, curr, script_level, true);
    case MB_USHORT_UNION_PLUS: return bigSym(source, curr, script_level, true);
    case MB_USHORT_BINOMIAL_COEFFICIENTS:  return binomial(source, curr, script_level);
    case MB_USHORT_CASES:  return cases(source, curr, script_level);
    case MB_USHORT_FRACTION:  return fraction(source, curr, script_level);
    case MB_USHORT_GROUPING_PARENTHESIS: return group(Group::PAREN, Group::PAREN, source, curr, script_level);
    case MB_USHORT_GROUPING_BRACKETS: return group(Group::BRACKET, Group::BRACKET, source, curr, script_level);
    case MB_USHORT_GROUPING_BARS: return group(Group::BAR, Group::BAR, source, curr, script_level);
    case MB_USHORT_GROUPING_DOUBLE_BARS: return group(Group::NORM, Group::NORM, source, curr, script_level);
    case MB_USHORT_GROUPING_CEIL: return group(Group::CEIL, Group::CEIL, source, curr, script_level);
    case MB_USHORT_GROUPING_FLOOR: return group(Group::FLOOR, Group::FLOOR, source, curr, script_level);
    case MB_USHORT_INTEGRAL: return integral(source, curr, script_level, true);
    case MB_USHORT_DOUBLE_INTEGRAL: return integral(source, curr, script_level, false);
    case MB_USHORT_TRIPLE_INTEGRAL: return integral(source, curr, script_level, false);
    case MB_USHORT_CONTOUR_INTEGRAL: return integral(source, curr, script_level, true);
    case MB_USHORT_CLOSED_SURFACE_INTEGRAL: return integral(source, curr, script_level, false);
    case MB_USHORT_CLOSED_VOLUME_INTEGRAL: return integral(source, curr, script_level, false);
    case MB_USHORT_MATRIX: return matrix(source, curr, script_level);
    case MB_USHORT_ROOT: return root(source, curr, script_level);
    case MB_USHORT_SUBSCRIPT:  return subscript(source, curr, script_level);
    case MB_USHORT_SUPERSCRIPT:  return superscript(source, curr, script_level);
    case MB_USHORT_DUALSCRIPT:  return dualscript(source, curr, script_level, false);
    case MB_USHORT_EVALSCRIPT: return dualscript(source, curr, script_level, true);
    case MB_USHORT_UNDERSCRIPTED_MAX: return underscriptWord("max", source, curr, script_level);
    case MB_USHORT_UNDERSCRIPTED_MIN: return underscriptWord("min", source, curr, script_level);
    case MB_USHORT_UNDERSCRIPTED_SUP: return underscriptWord("sup", source, curr, script_level);
    case MB_USHORT_UNDERSCRIPTED_INF: return underscriptWord("inf", source, curr, script_level);
    case MB_USHORT_LIMIT: return limit(source, curr, script_level);
    default: Q_ASSERT(false); return nullptr;
    }
}

static Text* parseTextInSubPhrase(const QString& source, int& curr, uint8_t& script_level){
    int start = curr;

    for(;;){
        Q_ASSERT(source[curr] != MB_OPEN);
        Q_ASSERT(source[curr] != '\n');
        Q_ASSERT(curr < source.size());

        if(source[curr] == MB_CLOSE) break;
        else if(source[curr] == MB_CONSTRUCT_SYMBOL && !matchEscapeSequence(source, curr)) break;
        else curr++;
    }

    QString str = source.mid(start, curr - start);
    return new Text(script_level, MathBran::removeEscapes(str));
}

static SubPhrase* subphrase(const QString& source, int& curr, uint8_t& script_level, uint32_t child_id){
    consume(source, curr, MB_OPEN);

    Text* front = parseTextInSubPhrase(source, curr, script_level);
    Text* text = front;

    while( source[curr] != MB_CLOSE ){
        consume(source, curr, MB_CONSTRUCT_SYMBOL);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInSubPhrase(source, curr, script_level);
        link(construct, text);

        construct->prev->startSignalToNext();
    }
    curr++;

    return new SubPhrase(front, text, child_id);
}

static Text* parseTextInLine(const QString& source, int& curr, uint8_t& script_level){
    int start = curr;

    while(curr < source.size() && source[curr] != '\n'){
        if(source[curr] == MB_CONSTRUCT_SYMBOL && !matchEscapeSequence(source, curr)){
            break;
        }else{
            Q_ASSERT(source[curr] != MB_OPEN);
            Q_ASSERT(source[curr] != MB_CLOSE);
            curr++;
        }
    }

    QString str = source.mid(start, curr - start);
    return new Text(script_level, MathBran::removeEscapes(str));
}

static Line* line(const QString& source, int& curr, uint8_t& script_level, const uint32_t& line_num){
    Text* front = parseTextInLine(source, curr, script_level);
    Text* text = front;

    while( curr < source.size() && source[curr] != '\n'){
        consume(source, curr, MB_CONSTRUCT_SYMBOL);

        Construct* construct = parseConstruct(source, curr, script_level);
        link(text, construct);

        text = parseTextInLine(source, curr, script_level);
        link(construct, text);

        construct->prev->startSignalToNext();
    }

    return new Line(front, text, line_num);
}

std::pair<Text*, Text*> Parser::parsePhrase(const QString& source, uint8_t script_level){
    Q_ASSERT(MathBran::isWellFormed(source));
    Q_ASSERT(!source.contains('\n'));

    int curr = 0;
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
    Q_ASSERT(MathBran::isWellFormed(source));
    Q_ASSERT(source.contains('\n'));

    int curr = 0;
    uint8_t script_level = 0;
    Line* front = line(source, curr, script_level, line_num);
    Line* l = front;

    while(curr < source.size()){
        curr++; //Skip newline
        line_num++;
        Line* next = line(source, curr, script_level, line_num);
        link(l, next);
        l = next;
    }

    return std::pair<Line*, Line*>(front, l);
}

TypesetScene* Parser::parseDocument(const QString& source, bool allow_write, bool show_line_numbers){
    Q_ASSERT(MathBran::isWellFormed(source));

    int curr = 0;
    uint8_t script_level = 0;
    uint32_t line_num = 1;
    Line* front = line(source, curr, script_level, line_num);
    Line* l = front;

    while(++curr < source.size()){ //Skip newline, final newline omitted
        line_num++;
        Line* next = line(source, curr, script_level, line_num);
        link(l, next);
        l = next;
    }

    return new TypesetScene(allow_write, show_line_numbers, front, l);
}

#ifndef QMATHBRAN
#define QMATHBRAN

#include "QMathBran_Macros.h"
#include <QString>

namespace MathBran{

inline QString& applyEscapes(QString& str){
    return str.replace(MB_CONSTRUCT_SYMBOL, MB_ESCAPED_CONSTRUCT_SYMBOL)
              .replace(MB_OPEN, MB_ESCAPED_OPEN)
              .replace(MB_CLOSE, MB_CLOSE);
}

inline QString& removeEscapes(QString& str){
    return str.replace(MB_ESCAPED_CONSTRUCT_SYMBOL, MB_CONSTRUCT_SYMBOL)
              .replace(MB_ESCAPED_OPEN, MB_OPEN)
              .replace(MB_CLOSE, MB_CLOSE);
}

inline bool containsConstruct(const QString& source){
    for(QString::size_type curr = 0; curr < source.size()-1; curr++){
        if(source[curr] == MB_CONSTRUCT_SYMBOL){
            switch (source[++curr].unicode()) {
                case MB_USHORT_CONSTRUCT_SYMBOL:
                case MB_USHORT_OPEN:
                case MB_USHORT_CLOSE:
                    break;
                default:
                    return true;
            }
        }
    }

    return false;
}

static bool testInvalidConstruct(const QString& source, QString::size_type& curr);

static bool testInvalidSubphrase(const QString& source, QString::size_type& curr){
    if(++curr == source.size()) return true;
    if(source[curr] != MB_OPEN) return true;

    while(++curr != source.size()){
        switch (source[curr].unicode()) {
            case MB_USHORT_CONSTRUCT_SYMBOL:
                if(testInvalidConstruct(source, curr)) return true;
                break;
            case MB_USHORT_OPEN:
            case '\n': //No newlines in nested phrases
                return true;
            case MB_USHORT_CLOSE:
                return false;
        }
    }

    return true;
}

static bool testInvalidOptionalSubphrase(const QString& source, QString::size_type& curr){
    return curr+1 < source.size() &&
           source[curr+1] == MB_OPEN &&
           testInvalidSubphrase(source, curr);
}

static bool testInvalidCases(const QString& source, QString::size_type& curr){
    do
        if(testInvalidSubphrase(source, curr) || testInvalidSubphrase(source, curr)) return true;
    while(curr+1 < source.size() && source[curr+1] == MB_OPEN);

    return false;
}

static bool testInvalidMatrix(const QString& source, QString::size_type& curr){
    if(++curr == source.size()) return true;
    if(source[curr] != MB_OPEN) return true;
    QString::size_type dim_start = ++curr;
    while(++curr < source.size() && source[curr] != MB_CLOSE);
    if(curr == source.size()) return true;
    bool success;
    ushort rows = source.midRef(dim_start, curr - dim_start).toUShort(&success);
    if(!success || rows==0 || rows > 255) return true;

    if(++curr == source.size()) return true;
    if(source[curr] != MB_OPEN) return true;
    dim_start = ++curr;
    while(++curr < source.size() && source[curr] != MB_CLOSE);
    if(curr == source.size()) return true;
    ushort cols = source.midRef(dim_start, curr - dim_start).toUShort(&success);
    if(!success || cols==0 || cols > 255) return true;

    if(curr+1 == source.size() || source[curr+1] != MB_OPEN) return false; //Elements are optional

    for(ushort i = rows*cols; i > 0; i--)
        if(testInvalidSubphrase(source, curr)) return true;

    return false;
}

static bool testInvalidConstruct(const QString& source, QString::size_type& curr){
    if(++curr == source.size()) return true;

    switch(source[curr].unicode()){
        //Escaped characters
        case MB_USHORT_CONSTRUCT_SYMBOL:
        case MB_USHORT_OPEN:
        case MB_USHORT_CLOSE:
            return false;

        //Unary constructs
        case MB_USHORT_ACCENT_ARROW:
        case MB_USHORT_ACCENT_BAR:
        case MB_USHORT_ACCENT_BREVE:
        case MB_USHORT_ACCENT_DOT:
        case MB_USHORT_ACCENT_DOUBLE_DOTS:
        case MB_USHORT_ACCENT_TRIPLE_DOTS:
        case MB_USHORT_ACCENT_HAT:
        case MB_USHORT_ACCENT_TILDE:
        case MB_USHORT_GROUPING_BARS:
        case MB_USHORT_GROUPING_DOUBLE_BARS:
        case MB_USHORT_GROUPING_BRACKETS:
        case MB_USHORT_GROUPING_CEIL:
        case MB_USHORT_GROUPING_FLOOR:
        case MB_USHORT_GROUPING_PARENTHESIS:
        case MB_USHORT_SUBSCRIPT:
        case MB_USHORT_SUPERSCRIPT:
        case MB_USHORT_UNDERSCRIPTED_INF:
        case MB_USHORT_UNDERSCRIPTED_MAX:
        case MB_USHORT_UNDERSCRIPTED_MIN:
        case MB_USHORT_UNDERSCRIPTED_SUP:
            return testInvalidSubphrase(source, curr);

        //Binary constructs
        case MB_USHORT_BINOMIAL_COEFFICIENTS:
        case MB_USHORT_DUALSCRIPT:
        case MB_USHORT_EVALSCRIPT:
        case MB_USHORT_FRACTION:
        case MB_USHORT_LIMIT:
            return testInvalidSubphrase(source, curr) || testInvalidSubphrase(source, curr);

        //Optional unary constructs
        case MB_USHORT_DOUBLE_INTEGRAL:
        case MB_USHORT_TRIPLE_INTEGRAL:
        case MB_USHORT_CLOSED_SURFACE_INTEGRAL:
        case MB_USHORT_CLOSED_VOLUME_INTEGRAL:
            return testInvalidOptionalSubphrase(source, curr);

        //Distinctly optional binary constructs
        case MB_USHORT_SUMMATION:
        case MB_USHORT_PRODUCT:
        case MB_USHORT_COPRODUCT:
        case MB_USHORT_INTERSECTION:
        case MB_USHORT_UNION:
        case MB_USHORT_UNION_PLUS:
        case MB_USHORT_INTEGRAL:
        case MB_USHORT_CONTOUR_INTEGRAL:
            return testInvalidOptionalSubphrase(source, curr) || testInvalidOptionalSubphrase(source, curr);

        //Unique constructs
        case MB_USHORT_ROOT: return testInvalidSubphrase(source, curr) ||
                                    testInvalidOptionalSubphrase(source, curr);
        case MB_USHORT_CASES: return testInvalidCases(source, curr);
        case MB_USHORT_MATRIX: return testInvalidMatrix(source, curr);

        //Unrecognized construct code
        default: return true;
    }
}

inline bool isWellFormed(const QString& source){
    for(QString::size_type curr = 0; curr < source.size(); curr++){
        switch (source[curr].unicode()) {
            case MB_USHORT_CONSTRUCT_SYMBOL:
                if(testInvalidConstruct(source, curr)) return false;
                break;
            case MB_USHORT_OPEN:
            case MB_USHORT_CLOSE:
                return false;
        }
    }

    return true;
}

};

#endif

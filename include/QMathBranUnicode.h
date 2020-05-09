#ifndef QMATHBRANUNICODE_H
#define QMATHBRANUNICODE_H

#include "QMathBran.h"
#include "QMathBranUnicode_Macros.h"
#include <QTextStream>

namespace MathBran {

static bool validateSubscript(const QString& source, int& curr){
    if(source[curr] == MB_CLOSE) return false;
    while(source[curr] != MB_CLOSE){
        switch (source[curr++].unicode()) {
            MATHBRAN_SUBSCRIPT_ALLOWED_CASES
            case ' ':
                break;
            default: return false;
        }
    }
    curr++;

    return true;
}

static bool validateSuperscript(const QString& source, int& curr){
    if(source[curr] == MB_CLOSE) return false;
    while(source[curr] != MB_CLOSE){
        switch (source[curr++].unicode()) {
            MATHBRAN_SUPERSCRIPT_ALLOWED_CASES
            case ' ':
                break;
            case MB_USHORT_CONSTRUCT_SYMBOL:
                if(source[curr++] == MB_INTEGRAL && source[curr] != MB_OPEN) break;
                else return false;
            default: return false;
        }
    }
    curr++;

    return true;
}

inline bool validateAccent(const QString& source, int& curr){
    if(source[curr] == MB_CLOSE) return false;
    if(source[curr++] == MB_CONSTRUCT_SYMBOL){
        switch (source[curr++].unicode()) {
            case MB_USHORT_ACCENT_ARROW:
            case MB_USHORT_ACCENT_BAR:
            case MB_USHORT_ACCENT_BREVE:
            case MB_USHORT_ACCENT_DOT:
            case MB_USHORT_ACCENT_DOUBLE_DOTS:
            case MB_USHORT_ACCENT_HAT:
            case MB_USHORT_ACCENT_TILDE:
                if(validateAccent(source,++curr)) break;
                return false;
            default:
                return false;
        }
        if(source[curr] != MB_CLOSE) return false;
    }
    curr++;

    return true;
}

inline bool validateExtendableAccent(const QString& source, int& curr){
    if(source[curr] == MB_CLOSE){
        return false;
    }else if(source[curr] == MB_CONSTRUCT_SYMBOL){
        return validateAccent(source, curr);
    }else{
        while(source[curr] != MB_CLOSE)
            if(source[curr++] == MB_CONSTRUCT_SYMBOL) return false;
        curr++;

        return true;
    }
}

inline bool canConvertToUnicode(const QString& source){
    Q_ASSERT(isWellFormed(source));

    int curr = 0;
    while(curr < source.size()){
        if(source[curr++] == MB_CONSTRUCT_SYMBOL){
            switch (source[curr++].unicode()) {
                case MB_USHORT_GROUPING_BARS:
                case MB_USHORT_GROUPING_CEIL:
                case MB_USHORT_GROUPING_FLOOR:
                case MB_USHORT_GROUPING_BRACKETS:
                case MB_USHORT_GROUPING_DOUBLE_BARS:
                case MB_USHORT_GROUPING_PARENTHESIS:
                    curr++;
                    break;
                case MB_USHORT_SUBSCRIPT:
                    if(validateSubscript(source,++curr)) break;
                    return false;
                case MB_USHORT_SUPERSCRIPT:
                    if(validateSuperscript(source,++curr)) break;
                    return false;
                case MB_USHORT_ACCENT_BAR:
                    if(validateExtendableAccent(source,++curr)) break;
                    return false;
                case MB_USHORT_ACCENT_ARROW:
                case MB_USHORT_ACCENT_BREVE:
                case MB_USHORT_ACCENT_DOT:
                case MB_USHORT_ACCENT_DOUBLE_DOTS:
                case MB_USHORT_ACCENT_HAT:
                case MB_USHORT_ACCENT_TILDE:
                    if(validateAccent(source,++curr)) break;
                    return false;
                case MB_USHORT_INTEGRAL:
                case MB_USHORT_DOUBLE_INTEGRAL:
                case MB_USHORT_TRIPLE_INTEGRAL:
                case MB_USHORT_CONTOUR_INTEGRAL:
                case MB_USHORT_CLOSED_SURFACE_INTEGRAL:
                case MB_USHORT_CLOSED_VOLUME_INTEGRAL:
                case MB_USHORT_SUMMATION:
                case MB_USHORT_PRODUCT:
                case MB_USHORT_COPRODUCT:
                case MB_USHORT_INTERSECTION:
                case MB_USHORT_UNION:
                case MB_USHORT_UNION_PLUS:
                    if(source[curr] == MB_OPEN && !validateSubscript(source, ++curr)) return false;
                    if(source[curr] == MB_OPEN) return false;
                    break;

                default: return false;
            }
        }
    }

    return true;
}

static void writeSubscript(const QString& source, int& curr, QTextStream& out){
    while(source[curr] != MB_CLOSE){
        switch (source[curr++].unicode()) {
            MATHBRAN_SYMBOL_TO_SUBSCRIPT_MAP
            case ' ': out << ' '; break;
        }
    }
    curr++;
}

static void writeSuperscript(const QString& source, int& curr, QTextStream& out){
    while(source[curr] != MB_CLOSE){
        switch (source[curr++].unicode()) {
            MATHBRAN_SYMBOL_TO_SUPERSCRIPT_MAP
            case ' ': out << ' '; break;
            case MB_USHORT_CONSTRUCT_SYMBOL:
                curr++;
                out << QChar(7604);
                break;
        }
    }
    curr++;
}

static void writeAccent(const QString& source, int& curr, QTextStream& out, const QChar& diacritic){
    if(source[curr] == MB_CONSTRUCT_SYMBOL){
        curr++;
        switch (source[curr++].unicode()) {
            case MB_USHORT_ACCENT_ARROW: writeAccent(source, ++curr, out, QChar(8407)); break;
            case MB_USHORT_ACCENT_BAR: writeAccent(source, ++curr, out, QChar(773)); break;
            case MB_USHORT_ACCENT_BREVE: writeAccent(source, ++curr, out, QChar(774)); break;
            case MB_USHORT_ACCENT_DOT: writeAccent(source, ++curr, out, QChar(775)); break;
            case MB_USHORT_ACCENT_DOUBLE_DOTS: writeAccent(source, ++curr, out, QChar(776)); break;
            case MB_USHORT_ACCENT_HAT: writeAccent(source, ++curr, out, QChar(770)); break;
            case MB_USHORT_ACCENT_TILDE: writeAccent(source, ++curr, out, QChar(771)); break;
        }
    }else{
        out << source[curr++];
    }
    out << diacritic;
    curr++;
}

static void writeOverbar(const QString& source, int& curr, QTextStream& out){
    if(source[curr] == MB_CONSTRUCT_SYMBOL){
        writeAccent(source, curr, out, QChar(773));
    }else{
        while(source[curr] != MB_CLOSE)
            out << source[curr++] << QChar(773);
        curr++;
    }
}

static void writeGrouping(const QString& source, int& curr, QTextStream& out, const QChar& closer){
    QString::size_type start = curr;
    while(source[curr] != MB_CLOSE){
        if(source[curr++] == MB_CONSTRUCT_SYMBOL){
            out << source.midRef(start, curr-start-1);
            switch (source[curr++].unicode()) {
                case MB_USHORT_GROUPING_BARS:
                    out << '|'; writeGrouping(source, ++curr, out, '|'); break;
                case MB_USHORT_GROUPING_CEIL:
                    out << QChar(8968); writeGrouping(source, ++curr, out, QChar(8969)); break;
                case MB_USHORT_GROUPING_FLOOR:
                    out << QChar(8970); writeGrouping(source, ++curr, out, QChar(8971)); break;
                case MB_USHORT_GROUPING_BRACKETS:
                    out << '['; writeGrouping(source, ++curr, out, ']'); break;
                case MB_USHORT_GROUPING_DOUBLE_BARS:
                    out << QChar(8214); writeGrouping(source, ++curr, out, QChar(8214)); break;
                case MB_USHORT_GROUPING_PARENTHESIS:
                    out << '('; writeGrouping(source, ++curr, out, ')'); break;
                case MB_USHORT_SUBSCRIPT: writeSubscript(source, ++curr, out); break;
                case MB_USHORT_SUPERSCRIPT: writeSuperscript(source, ++curr, out); break;
                case MB_USHORT_ACCENT_BAR: writeOverbar(source, ++curr, out); break;
                case MB_USHORT_ACCENT_ARROW: writeAccent(source, ++curr, out, QChar(8407)); break;
                case MB_USHORT_ACCENT_BREVE: writeAccent(source, ++curr, out, QChar(774)); break;
                case MB_USHORT_ACCENT_DOT: writeAccent(source, ++curr, out, QChar(775)); break;
                case MB_USHORT_ACCENT_DOUBLE_DOTS: writeAccent(source, ++curr, out, QChar(776)); break;
                case MB_USHORT_ACCENT_HAT: writeAccent(source, ++curr, out, QChar(770)); break;
                case MB_USHORT_ACCENT_TILDE: writeAccent(source, ++curr, out, QChar(771)); break;

                case MB_USHORT_INTEGRAL:
                case MB_USHORT_DOUBLE_INTEGRAL:
                case MB_USHORT_TRIPLE_INTEGRAL:
                case MB_USHORT_CONTOUR_INTEGRAL:
                case MB_USHORT_CLOSED_SURFACE_INTEGRAL:
                case MB_USHORT_CLOSED_VOLUME_INTEGRAL:
                case MB_USHORT_SUMMATION:
                case MB_USHORT_PRODUCT:
                case MB_USHORT_COPRODUCT:
                case MB_USHORT_INTERSECTION:
                case MB_USHORT_UNION:
                case MB_USHORT_UNION_PLUS:
                    out << source[curr-1];
                    if(source[curr] == MB_OPEN) writeSubscript(source, ++curr, out);
                    break;
            }
            start = curr;
        }
    }
    out << source.midRef(start, curr-start) << closer;
    curr++;
}

inline QString toUnicode(const QString& source){
    Q_ASSERT(canConvertToUnicode(source));

    QString unicode;
    QTextStream out(&unicode);
    QString::size_type start = 0;
    QString::size_type curr = 0;
    while(curr < source.size()){
        if(source[curr++] == MB_CONSTRUCT_SYMBOL){
            out << source.midRef(start, curr-start-1);
            switch (source[curr++].unicode()) {
                case MB_USHORT_GROUPING_BARS:
                    out << '|'; writeGrouping(source, ++curr, out, '|'); break;
                case MB_USHORT_GROUPING_CEIL:
                    out << QChar(8968); writeGrouping(source, ++curr, out, QChar(8969)); break;
                case MB_USHORT_GROUPING_FLOOR:
                    out << QChar(8970); writeGrouping(source, ++curr, out, QChar(8971)); break;
                case MB_USHORT_GROUPING_BRACKETS:
                    out << '['; writeGrouping(source, ++curr, out, ']'); break;
                case MB_USHORT_GROUPING_DOUBLE_BARS:
                    out << QChar(8214); writeGrouping(source, ++curr, out, QChar(8214)); break;
                case MB_USHORT_GROUPING_PARENTHESIS:
                    out << '('; writeGrouping(source, ++curr, out, ')'); break;
                case MB_USHORT_SUBSCRIPT: writeSubscript(source, ++curr, out); break;
                case MB_USHORT_SUPERSCRIPT: writeSuperscript(source, ++curr, out); break;
                case MB_USHORT_ACCENT_BAR: writeOverbar(source, ++curr, out); break;
                case MB_USHORT_ACCENT_ARROW: writeAccent(source, ++curr, out, QChar(8407)); break;
                case MB_USHORT_ACCENT_BREVE: writeAccent(source, ++curr, out, QChar(774)); break;
                case MB_USHORT_ACCENT_DOT: writeAccent(source, ++curr, out, QChar(775)); break;
                case MB_USHORT_ACCENT_DOUBLE_DOTS: writeAccent(source, ++curr, out, QChar(776)); break;
                case MB_USHORT_ACCENT_HAT: writeAccent(source, ++curr, out, QChar(770)); break;
                case MB_USHORT_ACCENT_TILDE: writeAccent(source, ++curr, out, QChar(771)); break;

                case MB_USHORT_INTEGRAL:
                case MB_USHORT_DOUBLE_INTEGRAL:
                case MB_USHORT_TRIPLE_INTEGRAL:
                case MB_USHORT_CONTOUR_INTEGRAL:
                case MB_USHORT_CLOSED_SURFACE_INTEGRAL:
                case MB_USHORT_CLOSED_VOLUME_INTEGRAL:
                case MB_USHORT_SUMMATION:
                case MB_USHORT_PRODUCT:
                case MB_USHORT_COPRODUCT:
                case MB_USHORT_INTERSECTION:
                case MB_USHORT_UNION:
                case MB_USHORT_UNION_PLUS:
                    out << source[curr-1];
                    if(source[curr] == MB_OPEN) writeSubscript(source, ++curr, out);
                    break;
            }
            start = curr;
        }
    }
    out << source.midRef(start, curr-start);

    return unicode;
}

}

#endif // QMATHBRANUNICODE_H

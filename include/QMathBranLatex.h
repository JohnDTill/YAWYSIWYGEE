#ifndef QMATHBRANLATEX
#define QMATHBRANLATEX

#include "QMathBran.h"
#include "QMathBranLatex_Macros.h"
#include <QTextStream>

namespace MathBran{

static void writeLatexSubphrase(const QString& source, QString::size_type& curr, QTextStream& out);

static void writeLatexConstruct(const QString& source, QString::size_type& curr, QTextStream& out){
    switch(source[curr++].unicode()){
        //Escaped characters
        case MB_USHORT_CONSTRUCT_SYMBOL:
        case MB_USHORT_OPEN:
        case MB_USHORT_CLOSE:
            out << "?";
            break;

        //Unary constructs
        case MB_USHORT_ACCENT_ARROW:
            out << "\\vec{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_ACCENT_BAR:
            out << "\\bar{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_ACCENT_BREVE:
            out << "\\breve{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_ACCENT_DOT:
            out << "\\dot{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_ACCENT_DOUBLE_DOTS:
            out << "\\ddot{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_ACCENT_TRIPLE_DOTS:
            out << "\\dddot{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_ACCENT_HAT:
            out << "\\widehat{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_ACCENT_TILDE:
            out << "\\tilde{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_GROUPING_BARS:
            out << "\\lvert";
            writeLatexSubphrase(source, curr, out);
            out << "\\rvert";
            break;
        case MB_USHORT_GROUPING_DOUBLE_BARS:
            out << "\\lVert";
            writeLatexSubphrase(source, curr, out);
            out << "\\rVert";
            break;
        case MB_USHORT_GROUPING_BRACKETS:
            out << "\\left[";
            writeLatexSubphrase(source, curr, out);
            out << "\\right]";
            break;
        case MB_USHORT_GROUPING_CEIL:
            out << "\\left\\lceil";
            writeLatexSubphrase(source, curr, out);
            out << "\\right\\rceil";
            break;
        case MB_USHORT_GROUPING_FLOOR:
            out << "\\left\\lfloor";
            writeLatexSubphrase(source, curr, out);
            out << "\\right\\rfloor";
            break;
        case MB_USHORT_GROUPING_PARENTHESIS:
            out << "\\left(";
            writeLatexSubphrase(source, curr, out);
            out << "\\right)";
            break;
        case MB_USHORT_SUBSCRIPT:
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_SUPERSCRIPT:
            out << "^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_UNDERSCRIPTED_INF:
            out << "\\inf{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_UNDERSCRIPTED_MAX:
            out << "\\max{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_UNDERSCRIPTED_MIN:
            out << "\\min{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_UNDERSCRIPTED_SUP:
            out << "\\sup{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;

        //Binary constructs
        case MB_USHORT_BINOMIAL_COEFFICIENTS:
            out << "\\binom{";
            writeLatexSubphrase(source, curr, out);
            out << "}{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_DUALSCRIPT:
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << "}^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_EVALSCRIPT:
            out << "\\bigg\\rvert_{";
            writeLatexSubphrase(source, curr, out);
            out << "}^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_FRACTION:
            out << "\\frac{";
            writeLatexSubphrase(source, curr, out);
            out << "}{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_LIMIT:
            out << "\\lim_{";
            writeLatexSubphrase(source, curr, out);
            out << " \\to ";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;

        //Optional unary constructs
        case MB_USHORT_DOUBLE_INTEGRAL:
            out << "\\iint";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_TRIPLE_INTEGRAL:
            out << "\\iiint";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_CLOSED_SURFACE_INTEGRAL:
            out << "\\oiint";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_CLOSED_VOLUME_INTEGRAL:
            out << "\\oiiint";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;

        //Distinctly optional binary constructs
        case MB_USHORT_SUMMATION:
            out << "\\sum";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_PRODUCT:
            out << "\\prod";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_COPRODUCT:
            out << "\\coprod";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_INTERSECTION:
            out << "\\bigcap";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_UNION:
            out << "\\bigcup";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_UNION_PLUS:
            out << "\\bigcupplus";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_INTEGRAL:
            out << "\\int";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;
        case MB_USHORT_CONTOUR_INTEGRAL:
            out << "\\oint";
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "_{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            if(curr == source.size() || source[curr] != MB_OPEN) break;
            out << "^{";
            writeLatexSubphrase(source, curr, out);
            out << '}';
            break;

        //Unique constructs
        case MB_USHORT_ROOT:{
            out << "\\sqrt";
            QString str;
            QTextStream temp(&str);
            writeLatexSubphrase(source, curr, temp);
            if(curr < source.size() && source[curr] == MB_OPEN){
                out << '[';
                writeLatexSubphrase(source, curr, out);
                out << ']';
            }
            out << '{' << str << '}';
            break;
        }

        case MB_USHORT_CASES:
            out << "\\begin{cases}\n";
            do{
                writeLatexSubphrase(source, curr, out);
                out << " & ";
                writeLatexSubphrase(source, curr, out);
                out << " \\\\\n";
            }while(curr < source.size() &&  source[curr] == MB_OPEN);
            out << "\\end{cases}";
            break;
        case MB_USHORT_MATRIX:{
            QString::size_type dim_start = ++curr;
            while(source[++curr] != MB_CLOSE);
            bool success;
            ushort rows = source.midRef(dim_start, curr - dim_start).toUShort(&success);
            curr++;
            dim_start = ++curr;
            while(source[++curr] != MB_CLOSE);
            ushort cols = source.midRef(dim_start, curr - dim_start).toUShort(&success);
            curr++;

            out << "\\begin{bmatrix}\n";
            if(curr < source.size() && source[curr]==MB_OPEN){
                for(ushort i = 0; i < rows; i++){
                    if(i > 0) out << " \\\\\n";
                    for(ushort j = 0; j < cols; j++){
                        if(j > 0) out << " & ";
                        writeLatexSubphrase(source, curr, out);
                    }
                }
            }else{
                for(ushort i = 0; i < rows; i++){
                    if(i > 0) out << " \\\\\n";
                    for(ushort j = 0; j < cols; j++){
                        if(j > 0) out << " & ";
                    }
                }
            }
            out << "\n\\end{bmatrix}";
            break;
        }

        //Unrecognized construct code
        default:
            out << QChar(55349); //55349 is code for '�'
    }
}

static bool isLetter(const QChar& c){
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static bool isPrintableASCII(const QChar& c){
    return c >= 32 && c <= 126;
}

MATHBRAN_LATEX_KEYWORD_LOOKUP_DEFINITION

static void switchLatexChar(const QString& source, QString::size_type& curr, QTextStream& out){
    QChar c = source[curr++];
    switch (c.unicode()) {
        MATHBRAN_LATEX_SYMBOLS
        case 55349:
            switch(source[curr++].unicode()){
                MATHBRAN_LATEX_SYMBOLS_32BIT
                default: out << QChar(55349); //55349 is code for '�'
            }
            break;
        case '\n':
            out << "\\\\\n"; break;
        case MB_USHORT_CONSTRUCT_SYMBOL:
            writeLatexConstruct(source, curr, out); break;
        default:
            if(isLetter(c)) searchKeyword(source, curr, out);
            else if(isPrintableASCII(c)) out << c;
            else out << QChar(55349); //55349 is code for '�'
    }
}

static void writeLatexSubphrase(const QString& source, QString::size_type& curr, QTextStream& out){
    curr++;
    while(source[curr] != MB_CLOSE) switchLatexChar(source, curr, out);
    curr++;
}

inline QString toLatex(const QString& source){
    Q_ASSERT(isWellFormed(source));

    QString latex;
    QTextStream out(&latex);
    QString::size_type curr = 0;
    while(curr < source.size()) switchLatexChar(source, curr, out);

    return latex;
}

};

#endif

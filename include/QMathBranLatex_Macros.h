//CODE-GEN FILE
//This file is generated from subfolder "meta".
//Changes to this file must be made in the meta project.

#define MATHBRAN_LATEX_KEYWORD_LOOKUP_DEFINITION \
static void searchKeyword(const QString& source, QString::size_type& curr, QTextStream& out){ \
    QString::size_type start = curr-1; \
    while(curr < source.size() && isLetter(source[curr])) curr++; \
\
    int range = curr-start; \
    QStringRef str = source.midRef(start, range); \
    if(range >= 2 && range <= 6){ \
        uint8_t hash = 1; \
        for(int i = 0; i < range; i++) hash += source[start+i].unicode()*(i+75); \
\
        switch(hash){ \
            case 102: if(str=="arccos") out << '\\'; break; \
            case 220: if(str=="arcsin") out << '\\'; break; \
            case 178: if(str=="arctan") out << '\\'; break; \
            case 63: if(str=="arg") out << '\\'; break; \
            case 141: if(str=="cos") out << '\\'; break; \
            case 61: if(str=="cosh") out << '\\'; break; \
            case 218: if(str=="cot") out << '\\'; break; \
            case 138: if(str=="coth") out << '\\'; break; \
            case 237: if(str=="csc") out << '\\'; break; \
            case 68: if(str=="deg") out << '\\'; break; \
            case 45: if(str=="det") out << '\\'; break; \
            case 66: if(str=="dim") out << '\\'; break; \
            case 232: if(str=="exp") out << '\\'; break; \
            case 166: if(str=="gcd") out << '\\'; break; \
            case 54: if(str=="hom") out << '\\'; break; \
            case 26: if(str=="inf") out << '\\'; break; \
            case 160: if(str=="ker") out << '\\'; break; \
            case 57: if(str=="lg") out << '\\'; break; \
            case 154: if(str=="lim") out << '\\'; break; \
            case 106: if(str=="liminf") out << '\\'; break; \
            case 191: if(str=="limsup") out << '\\'; break; \
            case 77: if(str=="ln") out << '\\'; break; \
            case 148: if(str=="log") out << '\\'; break; \
            case 212: if(str=="max") out << '\\'; break; \
            case 50: if(str=="min") out << '\\'; break; \
            case 73: if(str=="Pr") out << '\\'; break; \
            case 117: if(str=="sec") out << '\\'; break; \
            case 244: if(str=="sin") out << '\\'; break; \
            case 164: if(str=="sinh") out << '\\'; break; \
            case 30: if(str=="sup") out << '\\'; break; \
            case 223: if(str=="tan") out << '\\'; break; \
            case 143: if(str=="tanh") out << '\\'; break; \
        } \
    } \
\
    out << str; \
}

#define MATHBRAN_LATEX_SYMBOLS \
    case 123:\
        out << "\\{";\
        break;\
    case 125:\
        out << "\\}";\
        break;\
    case 38:\
        out << "\\&";\
        break;\
    case 35:\
        out << "\\#";\
        break;\
    case 37:\
        out << "\\%";\
        break;\
    case 8214:\
        out << "\\|";\
        break;\
    case 36:\
        out << "\\$";\
        break;\
    case 8944:\
        out << "\\adots";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8501:\
        out << "\\aleph";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 945:\
        out << "\\alpha";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 913:\
        out << "\\Alpha";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8736:\
        out << "\\angle";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8776:\
        out << "\\approx";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8727:\
        out << "\\ast";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 92:\
        out << "\\backslash";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8959:\
        out << "\\bagmember";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8757:\
        out << "\\because";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 946:\
        out << "\\beta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 914:\
        out << "\\Beta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8502:\
        out << "\\beth";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9632:\
        out << "\\blacksquare";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8227:\
        out << "\\blacktriangleright";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8869:\
        out << "\\bot";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8904:\
        out << "\\bowtie";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9633:\
        out << "\\box";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8783:\
        out << "\\bumpeq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8745:\
        out << "\\cap";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8901:\
        out << "\\cdot";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8943:\
        out << "\\cdots";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 162:\
        out << "\\cent";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10003:\
        out << "\\checkmark";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 967:\
        out << "\\chi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 935:\
        out << "\\Chi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8728:\
        out << "\\circ";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8759:\
        out << "\\Colon";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8705:\
        out << "\\complement";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8773:\
        out << "\\cong";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8720:\
        out << "\\coprod";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 169:\
        out << "\\copyright";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8746:\
        out << "\\cup";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8845:\
        out << "\\cupdot";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8224:\
        out << "\\dagger";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8504:\
        out << "\\daleth";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8225:\
        out << "\\ddagger";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8945:\
        out << "\\ddots";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 948:\
        out << "\\delta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 916:\
        out << "\\Delta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8960:\
        out << "\\diameter";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9856:\
        out << "\\dicei";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9857:\
        out << "\\diceii";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9858:\
        out << "\\diceiii";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9859:\
        out << "\\diceiv";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9860:\
        out << "\\dicev";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9861:\
        out << "\\dicevi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 989:\
        out << "\\digamma";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 988:\
        out << "\\Digamma";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 247:\
        out << "\\div";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8784:\
        out << "\\doteq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 11034:\
        out << "\\dottedsquare";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8595:\
        out << "\\downarrow";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9834:\
        out << "\\eightnote";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8709:\
        out << "\\emptyset";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 1013:\
        out << "\\epsilon";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 917:\
        out << "\\Epsilon";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8761:\
        out << "\\eqcolon";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8797:\
        out << "\\eqdef";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8801:\
        out << "\\equiv";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8803:\
        out << "\\Equiv";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10734:\
        out << "\\errbarsquare";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 951:\
        out << "\\eta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 919:\
        out << "\\Eta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8455:\
        out << "\\Euler";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8364:\
        out << "\\euro";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8252:\
        out << "\\Exclam";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8707:\
        out << "\\exists";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9837:\
        out << "\\flat";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8704:\
        out << "\\forall";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8279:\
        out << "\\fourth";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9785:\
        out << "\\frownie";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 947:\
        out << "\\gamma";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 915:\
        out << "\\Gamma";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8805:\
        out << "\\geq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8807:\
        out << "\\geqq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8811:\
        out << "\\gg";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8921:\
        out << "\\ggg";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8503:\
        out << "\\gimel";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 94:\
        out << "\\hat{}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8749:\
        out << "\\iiint";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8748:\
        out << "\\iint";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8465:\
        out << "\\Im";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8520:\
        out << "\\imath";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8712:\
        out << "\\in";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8710:\
        out << "\\increment";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8734:\
        out << "\\infty";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8747:\
        out << "\\int";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 953:\
        out << "\\iota";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 921:\
        out << "\\Iota";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8521:\
        out << "\\jmath";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 954:\
        out << "\\kappa";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 922:\
        out << "\\Kappa";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 955:\
        out << "\\lambda";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 923:\
        out << "\\Lambda";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10216:\
        out << "\\langle";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8968:\
        out << "\\lceil";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8592:\
        out << "\\leftarrow";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8656:\
        out << "\\Leftarrow";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8596:\
        out << "\\leftrightarrow";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8660:\
        out << "\\Leftrightarrow";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8804:\
        out << "\\leq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8806:\
        out << "\\leqq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8810:\
        out << "\\ll";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10218:\
        out << "\\llangle";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10214:\
        out << "\\llbracket";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8920:\
        out << "\\lll";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8905:\
        out << "\\ltimes";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10016:\
        out << "\\maltese";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8614:\
        out << "\\mapsto";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8450:\
        out << "\\mathbb{C}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8518:\
        out << "\\mathbb{d}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8495:\
        out << "\\mathbb{e}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8461:\
        out << "\\mathbb{H}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8469:\
        out << "\\mathbb{N}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8473:\
        out << "\\mathbb{P}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8474:\
        out << "\\mathbb{Q}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8477:\
        out << "\\mathbb{R}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8484:\
        out << "\\mathbb{Z}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8459:\
        out << "\\mathscr{H}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8466:\
        out << "\\mathscr{L}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 163:\
        out << "\\mathsterling";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 165:\
        out << "\\mathyen";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8737:\
        out << "\\measuredangle";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8487:\
        out << "\\mho";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8723:\
        out << "\\mp";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 956:\
        out << "\\mu";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 924:\
        out << "\\Mu";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8711:\
        out << "\\nabla";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8777:\
        out << "\\napprox";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9838:\
        out << "\\natural";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8775:\
        out << "\\ncong";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 172:\
        out << "\\neg";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8800:\
        out << "\\neq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8802:\
        out << "\\nequiv";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8708:\
        out << "\\nexists";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8815:\
        out << "\\ngtr";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8715:\
        out << "\\ni";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8814:\
        out << "\\nless";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8740:\
        out << "\\nmid";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8716:\
        out << "\\nni";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8713:\
        out << "\\notin";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8836:\
        out << "\\nsubset";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8840:\
        out << "\\nsubseteq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8837:\
        out << "\\nsupset";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8841:\
        out << "\\nsupseteq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 957:\
        out << "\\nu";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 925:\
        out << "\\Nu";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10752:\
        out << "\\odot";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8752:\
        out << "\\oiiint";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8751:\
        out << "\\oiint";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8750:\
        out << "\\oint";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 969:\
        out << "\\omega";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 937:\
        out << "\\Omega";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 959:\
        out << "\\omicron";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 927:\
        out << "\\Omicron";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8854:\
        out << "\\ominus";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8853:\
        out << "\\oplus";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8856:\
        out << "\\oslash";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8855:\
        out << "\\otimes";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 182:\
        out << "\\P";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8706:\
        out << "\\partial";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 981:\
        out << "\\phi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 934:\
        out << "\\Phi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 960:\
        out << "\\pi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 928:\
        out << "\\Pi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8462:\
        out << "\\Planckconst";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 177:\
        out << "\\pm";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8826:\
        out << "\\prec";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8242:\
        out << "\\prime";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8719:\
        out << "\\prod";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8733:\
        out << "\\propto";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 968:\
        out << "\\psi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 936:\
        out << "\\Psi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 985:\
        out << "\\qoppa";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 984:\
        out << "\\Qoppa";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9833:\
        out << "\\quarternote";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8799:\
        out << "\\questeq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10217:\
        out << "\\rangle";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8969:\
        out << "\\rceil";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8476:\
        out << "\\Re";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8971:\
        out << "\\rfloor";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 961:\
        out << "\\rho";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 929:\
        out << "\\Rho";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8735:\
        out << "\\rightangle";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8594:\
        out << "\\rightarrow";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8658:\
        out << "\\Rightarrow";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8644:\
        out << "\\rightleftarrows";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10219:\
        out << "\\rrangle";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10215:\
        out << "\\rrbracket";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8906:\
        out << "\\rtimes";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8243:\
        out << "\\second";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9839:\
        out << "\\sharp";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10722:\
        out << "\\shuffle";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 963:\
        out << "\\sigma";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 931:\
        out << "\\Sigma";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 126:\
        out << "\\sim";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8767:\
        out << "\\sinewave";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8738:\
        out << "\\sphericalangle";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8851:\
        out << "\\sqcap";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8852:\
        out << "\\sqcup";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8847:\
        out << "\\sqsubset";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8849:\
        out << "\\sqsubseteq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8848:\
        out << "\\sqsupset";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8850:\
        out << "\\sqsupseteq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8834:\
        out << "\\subset";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8912:\
        out << "\\Subset";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10179:\
        out << "\\subsetcirc";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8838:\
        out << "\\subseteq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8827:\
        out << "\\succ";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8721:\
        out << "\\sum";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8835:\
        out << "\\supset";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10180:\
        out << "\\supsetcirc";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8839:\
        out << "\\supseteq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 964:\
        out << "\\tau";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 932:\
        out << "\\Tau";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 174:\
        out << "\\textregistered";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8482:\
        out << "\\textsuperscript{TM}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8756:\
        out << "\\therefore";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 10727:\
        out << "\\thermod";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 952:\
        out << "\\theta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 920:\
        out << "\\Theta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8244:\
        out << "\\third";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 215:\
        out << "\\times";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8868:\
        out << "\\top";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9669:\
        out << "\\triangleleft";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8796:\
        out << "\\triangleq";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9659:\
        out << "\\triangleright";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9835:\
        out << "\\twonotes";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 95:\
        out << "\\underline";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8593:\
        out << "\\uparrow";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8597:\
        out << "\\updownarrow";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8846:\
        out << "\\uplus";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 965:\
        out << "\\upsilon";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 933:\
        out << "\\Upsilon";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 949:\
        out << "\\varepsilon";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 1008:\
        out << "\\varkappa";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 966:\
        out << "\\varphi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 982:\
        out << "\\varpi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 1009:\
        out << "\\varrho";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 962:\
        out << "\\varsigma";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 977:\
        out << "\\vartheta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 1012:\
        out << "\\varTheta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8788:\
        out << "\\vcentcolon=";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8866:\
        out << "\\vdash";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8871:\
        out << "\\vDash";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8942:\
        out << "\\vdots";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8744:\
        out << "\\vee";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8891:\
        out << "\\veebar";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 9888:\
        out << "\\warning";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8743:\
        out << "\\wedge";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8472:\
        out << "\\wp";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 8768:\
        out << "\\wr";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 958:\
        out << "\\xi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 926:\
        out << "\\Xi";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 437:\
        out << "\\Zbar";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 950:\
        out << "\\zeta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 918:\
        out << "\\Zeta";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\

#define MATHBRAN_LATEX_SYMBOLS_32BIT \
    case 57304:\
        out << "\\mathbb{0}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 57305:\
        out << "\\mathbb{1}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 57306:\
        out << "\\mathbb{2}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 57307:\
        out << "\\mathbb{3}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 57308:\
        out << "\\mathbb{4}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 57309:\
        out << "\\mathbb{5}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 57310:\
        out << "\\mathbb{6}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 57311:\
        out << "\\mathbb{7}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 57312:\
        out << "\\mathbb{8}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 57313:\
        out << "\\mathbb{9}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56633:\
        out << "\\mathbb{B}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56635:\
        out << "\\mathbb{D}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56636:\
        out << "\\mathbb{E}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56637:\
        out << "\\mathbb{F}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56641:\
        out << "\\mathbb{J}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56646:\
        out << "\\mathbb{O}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56650:\
        out << "\\mathbb{S}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56651:\
        out << "\\mathbb{T}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56652:\
        out << "\\mathbb{U}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\
    case 56608:\
        out << "\\mathfrak{c}";\
        if(curr < source.size() && isLetter(source[curr])) out << "{}";\
        break;\


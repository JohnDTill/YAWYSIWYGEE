#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "line.h"
#include "text.h"

namespace Typeset{

class Algorithm{
public:
    static qreal cursorOffset(const Text& t, QTextCursor c);
    static void write(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QTextStream& out);
    static Text* textAtSetpoint(const Phrase& p, qreal x);
    static QTextCursor getCursorAtSetpoint(const Text& t, qreal x);
    static Line& lineAtY(Line& start, qreal y);
    static bool contains(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QPointF p);
    static QString QStringRight(QTextCursor c);
    static QString QStringLeft(QTextCursor c);
    static QTextCursor textCursorStart(Text* t);

private:
    static void writeAfterCursor(QTextCursor c, QTextStream& out);
    static void writeUntilCursor(QTextCursor c, QTextStream& out);
    static void writeText(QTextCursor cL, QTextCursor cR, QTextStream& out);
    static void writePhrase(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QTextStream& out);
    static void writePhraseAfterCursor(const Text& t, QTextCursor c, QTextStream& out);
    static void writePhraseBeforeCursor(const Text& t, QTextCursor c, QTextStream& out);
    static void writeMultiLine(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QTextStream& out);
};

}

#endif // ALGORITHM_H

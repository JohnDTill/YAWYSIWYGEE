#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QTextCursor>
class Line;
class Phrase;
class Text;
class QTextStream;

namespace Algorithm{
    qreal cursorOffset(const Text& t, QTextCursor c);
    void write(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QTextStream& out);
    Text* textAtSetpoint(const Phrase& p, qreal x);
    QTextCursor getCursorAtSetpoint(const Text& t, qreal x);
    Line& lineAtY(Line& start, qreal y);
    bool contains(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QPointF p);
    QString QStringRight(QTextCursor c);
    QString QStringLeft(QTextCursor c);
    QTextCursor textCursorStart(Text* t);
};

#endif // ALGORITHM_H

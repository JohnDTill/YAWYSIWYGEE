#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QtGlobal>
#include <vector>
class Line;
class Phrase;
class Text;
class QChar;
class QPointF;
class QString;
class QTextCursor;
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
    std::vector<qreal> findAlignOffsets(const QChar& ch, const Text* tl, QTextCursor cl, const Text* tr, QTextCursor cr);

};

#endif // ALGORITHM_H

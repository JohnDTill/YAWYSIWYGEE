#include "algorithm.h"

#include "construct.h"
#include "line.h"
#include "parser.h"
#include <QTextCursor>

namespace Typeset{

qreal Algorithm::cursorOffset(const Text& t, QTextCursor c){
    QFontMetrics fm(t.font());
    c.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor);

    return fm.width(c.selectedText());
}

void Algorithm::write(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QTextStream& out){
    if(&tL == &tR) writeText(cL, cR, out);
    else if(tL.parent == tR.parent) writePhrase(tL, cL, tR, cR, out);
    else{
        Q_ASSERT(tL.parent->isLine());
        Q_ASSERT(tR.parent->isLine());
        writeMultiLine(tL, cL, tR, cR, out);
    }
}

Text* Algorithm::textAtSetpoint(const Phrase& p, qreal x){
    x -= p.scenePos().x();

    Text* t = p.front;
    while(t->next && x >= t->next->next->x()) t = t->next->next;

    return t;
}

QTextCursor Algorithm::getCursorAtSetpoint(const Text& t, qreal x){
    x -= t.scenePos().x();
    QFontMetrics fm(t.font());
    QTextCursor c = t.textCursor();
    c.setPosition(0);

    while(c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor)){
        if( fm.width(c.selectedText()) > x ){
            c.clearSelection();
            c.movePosition(QTextCursor::Left);
            return c;
        }
    }

    c.clearSelection();
    return c;
}

Line& Algorithm::lineAtY(Line& start, qreal y){
    Line* l = nullptr;
    if(start.scenePos().y() < y){
        for(l = &start; l->next; l = l->next){
            if(l->next->scenePos().y() > y) return *l;
        }
    }else{
        for(l = &start; l->prev; l = l->prev){
            if(l->scenePos().y() < y) return *l;
        }
    }

    return *l;
}

bool Algorithm::contains(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QPointF p){
    if(tL.parent==tR.parent){
        QPointF p_phrase = tL.parent->scenePos();
        qreal yU = p_phrase.y();
        qreal yD = yU + tL.parent->h();
        if(p.y() < yU || p.y() > yD) return false;

        qreal x = p.x() - p_phrase.x();
        return (x >= tL.x() + cursorOffset(tL, cL) && x <= tR.x() + cursorOffset(tR, cR));
    }else{
        Q_ASSERT(tL.parent->isLine());
        Q_ASSERT(tR.parent->isLine());

        QPointF pL = tL.parent->scenePos();
        QPointF pR = tR.parent->scenePos();

        qreal yLU = pL.y();
        qreal yLD = yLU + tL.parent->h();
        qreal yRU = pR.y();
        qreal yRD = yRU + tR.parent->h();
        if(p.y() < yLU || p.y() > yRD) return false;
        else if(p.y() > yLD && p.y() < yRU) return true;
        else if(p.y() < yLD) return p.x() > tL.scenePos().x() + cursorOffset(tL, cL);
        else return p.x() < tR.scenePos().x() + cursorOffset(tR, cR);
    }
}

QString Algorithm::QStringRight(QTextCursor c){
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    return c.selectedText();
}

QString Algorithm::QStringLeft(QTextCursor c){
    c.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor);
    return c.selectedText();
}

QTextCursor Algorithm::textCursorStart(Text* t){
    QTextCursor c = t->textCursor();
    c.setPosition(0);

    return c;
}

void Algorithm::writeAfterCursor(QTextCursor c, QTextStream& out){
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    QString str = c.selectedText();
    out << Parser::applyEscapes(str);
}

void Algorithm::writeUntilCursor(QTextCursor c, QTextStream& out){
    c.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor);
    QString str = c.selectedText();
    out << Parser::applyEscapes(str);
}

void Algorithm::writeText(QTextCursor cL, QTextCursor cR, QTextStream& out){
    cL.setPosition(cR.position(), QTextCursor::KeepAnchor);
    QString str = cL.selectedText();
    out << Parser::applyEscapes(str);
}

void Algorithm::writePhrase(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QTextStream& out){
    Q_ASSERT(tL.parent == tR.parent);
    Q_ASSERT(tL.x() < tR.x());

    writeAfterCursor(cL, out);
    tL.next->write(out);
    for(Text* t = tL.next->next; t != &tR; t = t->next->next){
        t->write(out);
        t->next->write(out);
    }
    writeUntilCursor(cR, out);
}

void Algorithm::writePhraseAfterCursor(const Text& t, QTextCursor c, QTextStream& out){
    writeAfterCursor(c, out);
    for(Construct* c = t.next; c; c = c->next->next){
        c->write(out);
        c->next->write(out);
    }
}

void Algorithm::writePhraseBeforeCursor(const Text& t, QTextCursor c, QTextStream& out){
    for(Text* curr = t.parent->front; curr != &t; curr = curr->next->next){
        curr->write(out);
        curr->next->write(out);
    }
    writeUntilCursor(c, out);
}

void Algorithm::writeMultiLine(const Text& tL, QTextCursor cL, const Text& tR, QTextCursor cR, QTextStream& out){
    Q_ASSERT(tL.parent->isLine());
    Q_ASSERT(tR.parent->isLine());
    Q_ASSERT(tL.parent->getLine().proceeds(tR.parent->getLine()));

    writePhraseAfterCursor(tL, cL, out);
    out << "\n";

    Line* final_line = &tR.parent->getLine();
    for(Line* l = tL.parent->getLine().next; l != final_line; l = l->next){
        l->write(out);
    }

    writePhraseBeforeCursor(tR, cR, out);
}

}

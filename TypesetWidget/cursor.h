#ifndef CURSOR_H
#define CURSOR_H

#include "globals.h"
#include <QClipboard>
#include <QGraphicsItem>
#include <QTextCursor>
#include <QUndoCommand>
#include <stack>

namespace Typeset{

class Construct;
class CursorView;
class Document;
class Line;
class Phrase;
class SubPhrase;
class Text;

class Cursor{
private:
    Document& doc;
    Text* anchor_text;
    QTextCursor anchor_cursor;
    Text* text;
    QTextCursor cursor;

    qreal x_setpoint = 0;

public:
    Cursor(Document& doc);
    bool hasSelection() const;
    void moveToNextChar();
    void moveToPreviousChar();
    void moveToNextWord();
    void moveToPreviousWord();
    void moveToNextLine();
    void moveToPreviousLine();
    void moveToNextPage(const qreal& page_height);
    void moveToPreviousPage(const qreal& page_height);
    void moveToStartOfLine();
    void moveToEndOfLine();
    void moveToStartOfBlock(){DO_THIS( "MoveToStartOfBlock" )}
    void moveToEndOfBlock(){DO_THIS( "MoveToEndOfBlock" )}
    void moveToStartOfDocument();
    void moveToEndOfDocument();
    void selectNextChar();
    void selectPreviousChar();
    void selectNextWord();
    void selectPreviousWord();
    void selectNextLine();
    void selectPreviousLine();
    void selectNextPage(const qreal& page_height);
    void selectPreviousPage(const qreal& page_height);
    void selectStartOfLine();
    void selectEndOfLine();
    void selectStartOfBlock(){DO_THIS( "SelectStartOfBlock" )}
    void selectEndOfBlock(){DO_THIS( "SelectEndOfBlock" )}
    void selectStartOfDocument();
    void selectEndOfDocument();
    void selectAll();
    void deselect(){DO_THIS("deselect")}
    void backspace();
    void del();
    void deleteEndOfWord();
    void deleteStartOfWord();
    void deleteEndOfLine(){DO_THIS( "DeleteEndOfLine" )}
    void deleteCompleteLine(){DO_THIS( "DeleteCompleteLine" )}
    void insertParagraphSeparator();
    void insertLineSeparator(){insertParagraphSeparator();} //This is shift-Enter
    QString selectedCode() const;
    void copy() const;
    void cut();
    void paste();
    void paste(const QString& str);
    void keystroke(const QChar& c);

    void clickText(Text& t, qreal x);
    void clickConstruct(Construct& c);
    void clickPoint(QPointF p);
    void doubleClick();
    void tripleClick();
    void tripleClick(Phrase& p);
    void selectClickPoint(QPointF p);
    void wordSelectPoint(QPointF p);
    void lineSelectPoint(QPointF p);

    bool contains(QPointF p);

    void setPosition(Text& t, QTextCursor c);

private:
    bool forward() const;
    static QTextCursor moveTextCursorToEnd(const Text& t);
    static QTextCursor moveTextCursorToStart(const Text& t);
    void consolidateToActive();
    void consolidateToAnchor();
    void consolidateRight();
    void consolidateLeft();
    void enterTextFromLeft(Text* t);
    void enterTextFromRight(Text* t);
    void selectTextFromLeft(Text* t);
    void selectTextFromRight(Text* t);
    void anchorTextFromLeft(Text* t);
    void anchorTextFromRight(Text* t);
    void enterConstructFromLeft(Construct* c);
    void enterConstructFromRight(Construct* c);
    void clearSetpoint();
    qreal updateSetpoint();
    qreal x();
    QUndoCommand* deleteSelection();
    QUndoCommand* insert(const QString& str);
    QUndoCommand* evaluate(const QString& source);
    void checkForSubstitution(const QChar& c);
    void checkSlashSub();

    friend CursorView;
};

}

#endif // CURSOR_H

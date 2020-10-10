#include "cursor.h"

#include "algorithm.h"
#include "construct.h"
#include "typesetscene.h"
#include "globals.h"
#include "line.h"
#include "parser.h"
#include "subphrase.h"
#include "text.h"
#include "typesetedit.h"
#include "YAWYSIWYGEE_commands.h"
#include "YAWYSIWYGEE_keywords.h"
#include "command/commands.h"
#include "MathBran/include/QMathBran.h"
#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>
#include <QTextDocument>
#include <QTextStream>

Cursor::Cursor(TypesetScene& doc)
    : doc(doc),
      anchor_text(doc.back->back),
      anchor_cursor(moveTextCursorToEnd(*anchor_text)),
      text(anchor_text),
      cursor(anchor_cursor) {}

bool Cursor::hasSelection() const{
    return anchor_text != text || anchor_cursor.position() != cursor.position();
}

void Cursor::moveToNextChar(){
    clearSetpoint();
    if(hasSelection()) consolidateRight();
    else if(cursor.movePosition(QTextCursor::Right)) consolidateToActive();
    else if(text->next) enterConstructFromLeft(text->next);
    else if(Text* t = text->parent->textRight()) enterTextFromLeft(t);
}

void Cursor::moveToPreviousChar(){
    clearSetpoint();
    if(hasSelection()) consolidateLeft();
    else if(cursor.movePosition(QTextCursor::Left)) consolidateToActive();
    else if(text->prev) enterConstructFromRight(text->prev);
    else if(Text* t = text->parent->textLeft()) enterTextFromRight(t);
}

void Cursor::moveToNextWord(){
    if(hasSelection()) consolidateRight();
    else if(cursor.movePosition(QTextCursor::NextWord)) consolidateToActive();
    else if(text->next) enterTextFromLeft(text->next->next);
    else if(Text* t = text->parent->textRight()) enterTextFromLeft(t);
}

void Cursor::moveToPreviousWord(){
    clearSetpoint();
    if(hasSelection()) consolidateLeft();
    else if(cursor.movePosition(QTextCursor::PreviousWord)) consolidateToActive();
    else if(text->prev) enterTextFromRight(text->prev->prev);
    else if(Text* t = text->parent->textLeft()) enterTextFromRight(t);
}

void Cursor::moveToNextLine(){
    if(Text* t = text->parent->textDown(updateSetpoint())){
        text = t;
        cursor = Algorithm::getCursorAtSetpoint(*text, x_setpoint);
    }
    consolidateToActive();
}

void Cursor::moveToPreviousLine(){
    if(Text* t = text->parent->textUp(updateSetpoint())){
        text = t;
        cursor = Algorithm::getCursorAtSetpoint(*text, x_setpoint);
    }
    consolidateToActive();
}

void Cursor::moveToNextPage(const qreal& page_height){
    Line& active_line = text->parent->getLine();
    Line& l = Algorithm::lineAtY(active_line, active_line.scenePos().y() + page_height);

    text = Algorithm::textAtSetpoint(l, updateSetpoint());
    cursor = Algorithm::getCursorAtSetpoint(*text, x_setpoint);
    consolidateToActive();
}

void Cursor::moveToPreviousPage(const qreal& page_height){
    Line& active_line = text->parent->getLine();
    Line& l = Algorithm::lineAtY(active_line, active_line.scenePos().y() - page_height);

    text = Algorithm::textAtSetpoint(l, updateSetpoint());
    cursor = Algorithm::getCursorAtSetpoint(*text, x_setpoint);
    consolidateToActive();
}

void Cursor::moveToStartOfLine(){
    clearSetpoint();
    enterTextFromLeft(text->parent->front);
}

void Cursor::moveToEndOfLine(){
    clearSetpoint();
    enterTextFromRight(text->parent->back);
}

void Cursor::moveToStartOfDocument(){
    clearSetpoint();
    enterTextFromLeft(doc.front->front);
}

void Cursor::moveToEndOfDocument(){
    clearSetpoint();
    enterTextFromRight(doc.back->back);
}

void Cursor::selectNextChar(){
    clearSetpoint();
    if(cursor.movePosition(QTextCursor::Right)) return;
    else if(text->next) selectTextFromLeft(text->next->next);
    else if(Text* t = text->parent->textRightUp()){
        if(!text->parent->isLine()) anchorTextFromRight(t->prev->prev);
        selectTextFromLeft(t);
    }
}

void Cursor::selectPreviousChar(){
    clearSetpoint();
    if(cursor.movePosition(QTextCursor::Left)) return;
    else if(text->prev) selectTextFromRight(text->prev->prev);
    else if(Text* t = text->parent->textLeftUp()){
        if(!text->parent->isLine()) anchorTextFromLeft(t->next->next);
        selectTextFromRight(t);
    }
}

void Cursor::selectNextWord(){
    clearSetpoint();
    if(cursor.movePosition(QTextCursor::NextWord)) return;
    else if(text->next) selectTextFromLeft(text->next->next);
    else if(Text* t = text->parent->textRightUp()){
        if(!text->parent->isLine()) anchorTextFromRight(t->prev->prev);
        selectTextFromLeft(t);
    }
}

void Cursor::selectPreviousWord(){
    clearSetpoint();
    if(cursor.movePosition(QTextCursor::PreviousWord)) return;
    else if(text->prev) selectTextFromRight(text->prev->prev);
    else if(Text* t = text->parent->textLeftUp()){
        if(!text->parent->isLine()) anchorTextFromLeft(t->next->next);
        selectTextFromRight(t);
    }
}

void Cursor::selectNextLine(){
    if(text->parent->isLine()){
        if(Text* t = text->parent->textDown(updateSetpoint())){
            text = t;
            cursor = Algorithm::getCursorAtSetpoint(*text, x_setpoint);
        }
    }else{
        Text* t = text->parent->textRightUp();
        Q_ASSERT(t);
        anchorTextFromRight(t->prev->prev);
        selectTextFromLeft(t);
    }
}

void Cursor::selectPreviousLine(){
    if(text->parent->isLine()){
        if(Text* t = text->parent->textUp(updateSetpoint())){
            text = t;
            cursor = Algorithm::getCursorAtSetpoint(*text, x_setpoint);
        }
    }else{
        Text* t = text->parent->textLeftUp();
        Q_ASSERT(t);
        anchorTextFromLeft(t->next->next);
        selectTextFromRight(t);
    }
}

void Cursor::selectNextPage(const qreal& page_height){
    if(text->parent->isLine()){
        Line& active_line = text->parent->getLine();
        Line& l = Algorithm::lineAtY(active_line, active_line.scenePos().y() + page_height);

        text = Algorithm::textAtSetpoint(l, updateSetpoint());
        cursor = Algorithm::getCursorAtSetpoint(*text, x_setpoint);
    }else{
        selectNextLine();
        selectNextPage(page_height);
    }
}

void Cursor::selectPreviousPage(const qreal& page_height){
    if(text->parent->isLine()){
        Line& active_line = text->parent->getLine();
        Line& l = Algorithm::lineAtY(active_line, active_line.scenePos().y() - page_height);

        text = Algorithm::textAtSetpoint(l, updateSetpoint());
        cursor = Algorithm::getCursorAtSetpoint(*text, x_setpoint);
    }else{
        selectPreviousLine();
        selectPreviousPage(page_height);
    }
}

void Cursor::selectStartOfLine(){
    clearSetpoint();
    selectTextFromLeft(text->parent->front);
}

void Cursor::selectEndOfLine(){
    selectTextFromRight(text->parent->back);
}

void Cursor::selectStartOfDocument(){
    clearSetpoint();
    selectTextFromLeft(doc.front->front);
}

void Cursor::selectEndOfDocument(){
    clearSetpoint();
    selectTextFromRight(doc.back->back);
}

void Cursor::selectAll(){
    clearSetpoint();
    anchor_text = doc.front->front;
    anchor_cursor = moveTextCursorToStart(*anchor_text);
    text = doc.back->back;
    cursor = moveTextCursorToEnd(*text);
}

void Cursor::backspace(){
    if(hasSelection()) doc.undo_stack->push(deleteSelection());
    else if(cursor.atStart() && (text->prev || !text->parent->isLine())) selectPreviousChar();
    else if(cursor.atStart() && text->parent->getLine().prev){
        selectPreviousChar();
        doc.undo_stack->push( deleteSelection() );
    }
    else if(!cursor.atStart()){
        int index = doc.undo_stack->index() - 1;
        if(index >= 0){
            const QUndoCommand* last = doc.undo_stack->command(index);
            if(last->text() == "D"){
                DeleteChar* delete_command = static_cast<DeleteChar*>(const_cast<QUndoCommand*>(last));
                if(text == delete_command->t && cursor.position() == delete_command->pL){
                    delete_command->removeChar(false);
                    emit doc.undo_stack->indexChanged(doc.undo_stack->index());
                    return;
                }
            }
        }

        doc.undo_stack->push( new DeleteChar(*this, false, text, cursor) );
    }
}

void Cursor::del(){
    if(hasSelection()) doc.undo_stack->push(deleteSelection());
    else if(cursor.atEnd() && (text->next || !text->parent->isLine())) selectNextChar();
    else if(cursor.atEnd() && text->parent->getLine().next){
        selectNextChar();
        doc.undo_stack->push( deleteSelection() );
    }
    else if(!cursor.atEnd()){
        int index = doc.undo_stack->index() - 1;
        if(index >= 0){
            const QUndoCommand* last = doc.undo_stack->command(index);
            if(last->text() == "D"){
                DeleteChar* delete_command = static_cast<DeleteChar*>(const_cast<QUndoCommand*>(last));
                if(text == delete_command->t && cursor.position() == delete_command->pL){
                    delete_command->removeChar(true);
                    emit doc.undo_stack->indexChanged(doc.undo_stack->index());
                    return;
                }
            }
        }

        doc.undo_stack->push( new DeleteChar(*this, true, text, cursor) );
    }
}

void Cursor::deleteEndOfWord(){
    if(hasSelection()) doc.undo_stack->push(deleteSelection());
    else if(cursor.atEnd() && (text->next || !text->parent->isLine())) selectNextChar();
    else{
        selectNextWord();
        if(hasSelection()) doc.undo_stack->push(deleteSelection());
    }
}

void Cursor::deleteStartOfWord(){
    if(hasSelection()) doc.undo_stack->push(deleteSelection());
    else if(cursor.atStart() && (text->prev || !text->parent->isLine())) selectPreviousChar();
    else{
        selectPreviousWord();
        if(hasSelection()) doc.undo_stack->push(deleteSelection());
    }
}

void Cursor::insertParagraphSeparator(){
    doc.undo_stack->push( insert("\n") );
}

QString Cursor::selectedCode() const{
    QString str;
    if(!hasSelection()) return str;

    QTextStream out(&str);

    if(forward()) Algorithm::write(*anchor_text, anchor_cursor, *text, cursor, out);
    else Algorithm::write(*text, cursor, *anchor_text, anchor_cursor, out);

    return str;
}

void Cursor::copy() const{
    if(!hasSelection()) return;

    QString str;
    QTextStream out(&str);

    if(forward()) Algorithm::write(*anchor_text, anchor_cursor, *text, cursor, out);
    else Algorithm::write(*text, cursor, *anchor_text, anchor_cursor, out);

    QGuiApplication::clipboard()->setText(str);
}

void Cursor::cut(){
    if(!hasSelection()) return;
    copy();
    del();
}

void Cursor::paste(){
    paste(QGuiApplication::clipboard()->text());
}

void Cursor::paste(const QString& str){
    if(hasSelection()){
        QUndoCommand* deletion = deleteSelection();
        deletion->redo(); //Deletion changes line connections, so consolidateLeft() is not enough
        QUndoCommand* insertion;
        if(MathBran::containsConstruct(str) && MathBran::isWellFormed(str)){
            insertion = evaluate(str);
        }else{
            QString unescape = str;
            insertion = insert(MathBran::removeEscapes(unescape));
        }
        deletion->undo();
        doc.undo_stack->push( new PairCommand(deletion, insertion) );
    }else{
        if(MathBran::containsConstruct(str) && MathBran::isWellFormed(str)){
            doc.undo_stack->push(evaluate(str));
        }else{
            QString unescape = str;
            doc.undo_stack->push(insert(MathBran::removeEscapes(unescape)));
        }
    }
}

void Cursor::keystroke(const QChar& c){
    if(hasSelection()){
        QUndoCommand* deletion = deleteSelection();
        consolidateLeft();
        doc.undo_stack->push( new PairCommand(deletion, new InsertChar(*this, c, text, cursor)) );
        return;
    }

    bool requires_new_command = true;

    int index = doc.undo_stack->index() - 1;
    if(index >= 0){
        const QUndoCommand* last = doc.undo_stack->command(index);
        if(last->text() == "C"){
            InsertChar* insert_command = static_cast<InsertChar*>(const_cast<QUndoCommand*>(last));
            if(text == insert_command->t && cursor.position() == insert_command->pR){
                insert_command->addChar(c);
                emit doc.undo_stack->indexChanged(doc.undo_stack->index());
                requires_new_command = false;
            }
        }else if(last->text() == "L"){
            const PairCommand* pair_command = static_cast<const PairCommand*>(last);
            if(pair_command->second->text() == "C"){
                InsertChar* insert_command = static_cast<InsertChar*>(pair_command->second);
                if(text == insert_command->t && cursor.position() == insert_command->pR){
                    insert_command->addChar(c);
                    emit doc.undo_stack->indexChanged(doc.undo_stack->index());
                    requires_new_command = false;
                }
            }
        }
    }

    if(requires_new_command) doc.undo_stack->push( new InsertChar(*this, c, text, cursor) );

    checkForSubstitution(c);
}

void Cursor::clickText(Text& t, qreal x){
    clearSetpoint();
    text = &t;
    cursor = Algorithm::getCursorAtSetpoint(t, x);
    consolidateToActive();
}

void Cursor::clickConstruct(Construct& c){
    clearSetpoint();
    text = c.next;
    cursor = moveTextCursorToStart(*text);
    anchor_text = c.prev;
    anchor_cursor = moveTextCursorToEnd(*anchor_text);
}

void Cursor::clickPoint(QPointF p){
    clearSetpoint();
    Line& l = Algorithm::lineAtY(text->parent->getLine(), p.y());
    text = Algorithm::textAtSetpoint(l, p.x());
    cursor = Algorithm::getCursorAtSetpoint(*text, p.x());
    consolidateToActive();
}

void Cursor::doubleClick(){
    if(hasSelection()) return;
    cursor.select(QTextCursor::WordUnderCursor);
    anchor_cursor.setPosition(cursor.anchor());
    cursor.clearSelection();
}

void Cursor::tripleClick(){
    tripleClick(*text->parent);
}

void Cursor::tripleClick(Phrase& p){
    if(p.isLine() && p.getLine().next){
        text = p.getLine().next->front;
        cursor = moveTextCursorToStart(*text);
    }else{
        text = p.back;
        cursor = moveTextCursorToEnd(*text);
    }

    anchor_text = p.front;
    anchor_cursor = moveTextCursorToStart(*anchor_text);
}

void Cursor::selectClickPoint(QPointF p){
    Phrase& phr = anchor_text->parent->isLine()
                  ? Algorithm::lineAtY(anchor_text->parent->getLine(), p.y())
                  : *anchor_text->parent;

    text = Algorithm::textAtSetpoint(phr, p.x());
    cursor = Algorithm::getCursorAtSetpoint(*text, p.x());
}

void Cursor::wordSelectPoint(QPointF p){
    bool was_forward = forward();
    selectClickPoint(p);
    if(p.x() < 0) return;
    cursor.select(QTextCursor::WordUnderCursor);
    bool same_word = (text == anchor_text) && anchor_cursor.position() == cursor.position();
    bool is_forward = forward() || same_word;
    cursor.setPosition(is_forward ? cursor.position() : cursor.anchor());
    if(was_forward != is_forward){
        if(is_forward) anchor_cursor.movePosition(QTextCursor::PreviousWord);
        else{
            anchor_cursor.select(QTextCursor::WordUnderCursor);
            anchor_cursor.clearSelection();
        }
    }
}

void Cursor::lineSelectPoint(QPointF p){
    if(!text->parent->isLine()) return;

    bool was_forward = forward();
    Line& l = text->parent->getLine();
    Line& active_line = l.next && was_forward ? *l.next : l;

    qreal bU = active_line.scenePos().y() - line_vspace*3/4;
    qreal bL = bU + active_line.h() + line_vspace*3/2;

    if(p.y() < bU || p.y() > bL){
        //selection moves
        Line& selected_line = Algorithm::lineAtY(active_line, p.y());
        bool is_forward = anchor_text->parent->getLine().preceedsInclusive(selected_line);

        if(is_forward){
            if(selected_line.next){
                text = selected_line.next->front;
                cursor = moveTextCursorToStart(*text);
            }else{
                text = selected_line.back;
                cursor = moveTextCursorToEnd(*text);
            }
            if(is_forward != was_forward){
                anchor_text = anchor_text->parent->front;
                anchor_cursor = moveTextCursorToStart(*anchor_text);
            }
        }else{
            text = selected_line.front;
            cursor = moveTextCursorToStart(*text);
            if(is_forward != was_forward){
                anchor_text = anchor_text->parent->back;
                anchor_cursor = moveTextCursorToEnd(*anchor_text);
            }
        }
    }
}

bool Cursor::contains(QPointF p){
    if(!hasSelection()) return false;
    return forward()
           ? Algorithm::contains(*anchor_text, anchor_cursor, *text, cursor, p)
           : Algorithm::contains(*text, cursor, *anchor_text, anchor_cursor, p);
}

void Cursor::setPosition(Text& t, QTextCursor c){
    text = &t;
    cursor = c;
    consolidateToActive();
}

void Cursor::tab(){
    if(inSubphrase()) return;

    if(!hasSelection()){
        doc.undo_stack->push(insert(QString(spaces_per_tab, ' ')));
        return;
    }

    const bool forw = forward();
    Text* tl = forw ? anchor_text : text;
    Text* tr = forw ? text : anchor_text;
    QTextCursor cl = forw ? anchor_cursor : cursor;
    const bool reset_cl = cl.position()==0;
    QTextCursor cr = forw ? cursor : anchor_cursor;
    Line* ll = &tl->parent->getLine();
    Line* lr = &tr->parent->getLine();

    if(ll == lr && !(tl == ll->front && tr == ll->back && cl.atStart() && cr.atEnd())){
        doc.undo_stack->push(deleteSelection());
        return;
    }

    Line* end = (cr.atStart() && tr == lr->front) ? lr : lr->next;
    doc.undo_stack->push(new Indent(*this, ll, end));
    if(reset_cl) cl.setPosition(0);
    anchor_text = tl;
    text = tr;
    anchor_cursor = cl;
    cursor = cr;
}

void Cursor::shiftTab(){
    if(inSubphrase()) return;

    if(!hasSelection()){
        QTextDocument* d = text->document();
        int i = cursor.position()-1;
        while(i >= 0 && i >= cursor.position()-spaces_per_tab && d->characterAt(i) == ' ') i--;

        if(i+1 != cursor.position()){
            cursor.setPosition(i+1);
            doc.undo_stack->push(deleteSelection());
        }

        return;
    }

    const bool forw = forward();
    Text* tl = forw ? anchor_text : text;
    Text* tr = forw ? text : anchor_text;
    QTextCursor cl = forw ? anchor_cursor : cursor;
    QTextCursor cr = forw ? cursor : anchor_cursor;
    Line* ll = &tl->parent->getLine();
    Line* lr = &tr->parent->getLine();

    if(ll == lr && !(tl == ll->front && tr == ll->back && cl.atStart() && cr.atEnd())){
        doc.undo_stack->push(deleteSelection());
        return;
    }

    Line* end = (cr.atStart() && tr == lr->front) ? lr : lr->next;
    Deindent* d = new Deindent(*this, ll, end);
    if(d->found_spaces){
        doc.undo_stack->push(d);
        anchor_text = tl;
        text = tr;
        anchor_cursor = cl;
        cursor = cr;
    }else{
        delete d;
    }
}

void Cursor::populateContextMenu(QMenu& menu){
    std::vector<qreal> offsets = forward() ?
                                 Algorithm::findAlignOffsets('=', anchor_text, anchor_cursor, text, cursor) :
                                 Algorithm::findAlignOffsets('=', text, cursor, anchor_text, anchor_cursor);
    if(!offsets.empty()){
        QAction* alignAction = menu.addAction("Align at '='");
        QObject::connect(alignAction,SIGNAL(triggered()),&doc,SLOT(alignAtEquals()));
    }
}

void Cursor::alignAtEquals(){
    const bool frwd = forward();
    Text* tl = frwd ? anchor_text : text;
    Text* tr = frwd ? text : anchor_text;
    QTextCursor cl = frwd ? anchor_cursor : cursor;
    QTextCursor cr = frwd ? cursor : anchor_cursor;

    std::vector<qreal> offsets = Algorithm::findAlignOffsets('=', tl, cl, tr, cr);
    if(offsets.empty()) return;

    if(cr.atStart() && tr == tr->parent->front){
        tr = tr->parent->getLine().prev->back;
        cr = tr->textCursor();
        cr.movePosition(QTextCursor::End);
    }

    doc.undo_stack->push(new AlignAtSymbol(*this, &tl->parent->getLine(), &tr->parent->getLine(), offsets));
}

bool Cursor::forward() const{
    if(text == anchor_text) return cursor.position() > anchor_cursor.position();
    else if(text->parent == anchor_text->parent) return text->x() > anchor_text->x();
    else{
        Q_ASSERT(text->parent->isLine());
        Q_ASSERT(anchor_text->parent->isLine());
        return anchor_text->parent->getLine().preceeds( text->parent->getLine() );
    }
}

bool Cursor::inSubphrase() const{
    return !text->parent->isLine();
}

QTextCursor Cursor::moveTextCursorToEnd(const Text& t){
    QTextCursor c = t.textCursor();
    c.movePosition(QTextCursor::End);

    return c;
}

QTextCursor Cursor::moveTextCursorToStart(const Text& t){
    QTextCursor c = t.textCursor();
    c.setPosition(0);

    return c;
}

void Cursor::consolidateToActive(){
    anchor_text = text;
    anchor_cursor = cursor;
}

void Cursor::consolidateToAnchor(){
    text = anchor_text;
    cursor = anchor_cursor;
}

void Cursor::consolidateRight(){
    if(forward()) consolidateToActive();
    else consolidateToAnchor();
}

void Cursor::consolidateLeft(){
    if(forward()) consolidateToAnchor();
    else consolidateToActive();
}

void Cursor::enterTextFromLeft(Text* t){
    text = t;
    cursor = moveTextCursorToStart(*text);
    consolidateToActive();
}

void Cursor::enterTextFromRight(Text* t){
    text = t;
    cursor = moveTextCursorToEnd(*text);
    consolidateToActive();
}

void Cursor::selectTextFromLeft(Text* t){
    text = t;
    cursor = moveTextCursorToStart(*text);
}

void Cursor::selectTextFromRight(Text* t){
    text = t;
    cursor = moveTextCursorToEnd(*text);
}

void Cursor::anchorTextFromLeft(Text* t){
    anchor_text = t;
    anchor_cursor = moveTextCursorToStart(*anchor_text);
}

void Cursor::anchorTextFromRight(Text* t){
    anchor_text = t;
    anchor_cursor = moveTextCursorToEnd(*anchor_text);
}

void Cursor::enterConstructFromLeft(Construct* c){
    if(SubPhrase* front = c->front()) enterTextFromLeft(front->front);
    else enterTextFromLeft(c->next);
}

void Cursor::enterConstructFromRight(Construct* c){
    if(SubPhrase* back = c->back()) enterTextFromRight(back->back);
    else enterTextFromRight(c->prev);
}

void Cursor::clearSetpoint(){
    x_setpoint = 0;
}

qreal Cursor::updateSetpoint(){
    x_setpoint = qMax(x_setpoint, x());
    return x_setpoint;
}

qreal Cursor::x(){
    return text->scenePos().x() + Algorithm::cursorOffset(*text, cursor);
}

QUndoCommand* Cursor::deleteSelection(){
    if(text==anchor_text){
        return TextCommand::remove(*this, text, cursor, anchor_cursor);
    }else if(text->parent == anchor_text->parent){
        return PhraseCommand::remove(*this, text, cursor, anchor_text, anchor_cursor);
    }else{
        return MultilineCommand::remove(*this, doc, text, cursor, anchor_text, anchor_cursor);
    }
}

QUndoCommand* Cursor::insert(const QString& str){
    if(str.contains('\n')) return MultilineCommand::insert(*this, doc, str, text, cursor);
    else return TextCommand::insert(*this, str, text, cursor);
}

QUndoCommand* Cursor::evaluate(const QString& source){
    if(source.contains('\n')) return MultilineCommand::eval(*this, doc, source, text, cursor);
    else return PhraseCommand::eval(*this, source, text, cursor);
}

void Cursor::checkSlashSub(){
    if(cursor.position() > 1 && text->toPlainText().at(cursor.position()-2) == '}'){
        checkComplexSlashSub();
        return;
    }

    QTextCursor temp_cursor = cursor;
    temp_cursor.movePosition(QTextCursor::Left);
    int word_end = temp_cursor.position();
    while(!temp_cursor.atStart()){
        temp_cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        switch(temp_cursor.selectedText().front().unicode()){
            case ' ': return;
            case '\\':
                temp_cursor.movePosition(QTextCursor::Right);
                temp_cursor.setPosition(word_end, QTextCursor::KeepAnchor);
                QString key = temp_cursor.selectedText();
                QString symbol = Keywords::lookup(key);
                if(!symbol.isEmpty()){
                    anchor_cursor.setPosition(temp_cursor.anchor() - 1);
                    paste(symbol);
                }else{
                    QString construct = Commands::lookup(key);
                    if(!construct.isEmpty()){
                        anchor_cursor.setPosition(temp_cursor.anchor() - 1);
                        paste(MB_CONSTRUCT_SYMBOL + construct);
                    }
                }

                return;
        }
        temp_cursor.clearSelection();
    }
}

void Cursor::checkComplexSlashSub(){
    Text* t = text;
    QTextCursor c = cursor;
    c.movePosition(QTextCursor::Left);

    int nesting_level = 0;
    QString converted = "";

    for(;;){
        int word_end = c.position();

        while(!c.atStart()){
            c.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
            switch(c.selectedText().front().unicode()){
                case ' ': {
                    if(nesting_level == 0) return;
                    int p = c.position();
                    c.setPosition(word_end, QTextCursor::KeepAnchor);
                    converted.prepend(c.selectedText());
                    c.setPosition(p);
                    word_end = p;
                    break;
                }
                case '}': {
                    nesting_level++;
                    int p = c.position();
                    c.movePosition(QTextCursor::Right);
                    c.setPosition(word_end, QTextCursor::KeepAnchor);
                    converted.prepend(MB_CLOSE + c.selectedText());
                    c.setPosition(p);
                    word_end = p;
                    break;
                }
                case '{': {
                    if(nesting_level == 0) return;
                    nesting_level--;
                    int p = c.position();
                    c.movePosition(QTextCursor::Right);
                    c.setPosition(word_end, QTextCursor::KeepAnchor);
                    converted.prepend(MB_OPEN + c.selectedText());
                    c.setPosition(p);
                    word_end = p;
                    break;
                }
                case '\\':
                    int p = c.position();
                    c.movePosition(QTextCursor::Right);
                    c.setPosition(word_end, QTextCursor::KeepAnchor);
                    QString key = c.selectedText();
                    QString symbol = Keywords::lookup(key);
                    if(!symbol.isEmpty()){
                        converted.prepend(symbol);
                    }else{
                        QString construct = Commands::lookup(key);
                        if(!construct.isEmpty()){
                            converted.prepend(MB_CONSTRUCT_SYMBOL + construct.front());
                        }else{
                            converted.prepend(key);
                        }
                    }

                    c.setPosition(p);
                    word_end = p;

                    if(nesting_level==0){
                        if(MathBran::isWellFormed(converted)){
                            anchor_text = t;
                            anchor_cursor = c;
                            paste(converted);
                        }

                        return;
                    }
            }
            c.clearSelection();
        }

        if(t->prev){
            c.setPosition(word_end, QTextCursor::KeepAnchor);
            converted.prepend(c.selectedText());

            QString c_str;
            QTextStream out(&c_str);
            t->prev->write(out);
            converted.prepend(c_str);

            t = t->prev->prev;
            c = t->textCursor();
            c.movePosition(QTextCursor::End);
        }else{
            return;
        }
    }
}

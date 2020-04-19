#include "text.h"

#include "document.h"
#include "globals.h"
#include "parser.h"
#include "phrase.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QTextDocument>

namespace Typeset{

Text::Text(uint8_t script_level, QString str){
    setFont(Globals::fonts[script_level]);
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable);
    document()->setUndoRedoEnabled(false);
    document()->setDocumentMargin(margin);
    setPlainText(str);

    calculateSize();
}

bool Text::isEmpty() const{
    return document()->isEmpty();
}

void Text::setParentPhrase(Phrase* parent){
    this->parent = parent;
    setParentItem(parent);
}

bool Text::isDeepestScriptLevel(uint8_t script_level){
    return script_level == Globals::deepest_script_level;
}

void Text::updateToTop(){
    calculateSize();
    parent->updateToTop();
}

void Text::calculateSize(){
    QRectF b = boundingRect();
    w = b.width() - subtract_from_width;
    u = (b.height() - subtract_from_height) / 2;
}

void Text::populateMenu(QMenu& menu){
    parent->populateMenu(menu);
}

void Text::write(QTextStream& out) const{
    QString str = toPlainText();
    Parser::applyEscapes(str);
    out << str;
}

uint8_t Text::getScriptLevel() const{
    int current_font_size = font().pixelSize();
    for(uint8_t i = 0; i <= Globals::deepest_script_level; i++)
        if( current_font_size == Globals::font_sizes[i] ) return i;

    FATAL_ERROR( "getScriptLevel() lookup failed" )
}

void Text::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    //This avoids a border which QGraphicsTextItem shows when focused
    QStyleOptionGraphicsItem my_option(*option);
    my_option.state &= ~QStyle::State_HasFocus;
    if(option->state.testFlag(QStyle::StateFlag::State_Selected)){
        my_option.state &= ~QStyle::State_Selected;
        setDefaultTextColor(scene()->palette().highlightedText().color());
    }else{
        setDefaultTextColor(scene()->palette().text().color());
    }
    QGraphicsTextItem::paint(painter, &my_option, widget);
}

void Text::focusInEvent(QFocusEvent* event){
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::focusInEvent(event);
}

void Text::focusOutEvent(QFocusEvent* event){
    setTextInteractionFlags(Qt::TextInteractionFlag::TextBrowserInteraction); //Remove blinking cursor
    QGraphicsItem::focusOutEvent(event); //Bypass QGraphicsTextItem::focusOutEvent()
}

Document* Text::typesetDocument() const{
    return static_cast<Document*>(scene());
}

}

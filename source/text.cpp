#include "text.h"

#include "construct.h"
#include "typesetscene.h"
#include "globals.h"
#include "parser.h"
#include "phrase.h"
#include "MathBran/include/QMathBran.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextStream>

#ifdef YAWYSIWYGEE_TEST
#include <list>
static std::list<Text*> all_texts;
#endif

Text::Text(uint8_t script_level, QString str){
    setFont(Globals::fonts[script_level]);
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable);
    document()->setUndoRedoEnabled(false);
    document()->setDocumentMargin(margin);
    setPlainText(str);

    calculateSize();

    #ifdef YAWYSIWYGEE_TEST
    all_texts.push_back(this);
    #endif
}

#ifdef YAWYSIWYGEE_TEST
Text::~Text(){
    all_texts.remove(this);
}

bool Text::verify(){
    for(Text* t : all_texts){
        if(t->next && t->next->prev != t) return false;
        if(t->prev && t->prev->next != t) return false;
    }

    return true;
}
#endif

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
    startSignalToNext();
    parent->updateToTop();
}

void Text::notifyPrevUpdate(){
    if(isEmpty() && next) next->notifyPrevPrevUpdate(prev);
}

void Text::startSignalToNext(){
    if(next){
        if(isEmpty() && prev) next->notifyPrevPrevUpdate(prev);
        else next->notifyPrevUpdate();
    }
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
    MathBran::applyEscapes(str);
    out << str;
}

uint8_t Text::getScriptLevel() const{
    int current_font_size = font().pixelSize();
    for(uint8_t i = 0; i <= Globals::deepest_script_level; i++)
        if( current_font_size == Globals::font_sizes[i] ) return i;

    Q_ASSERT(false);
    return 0;
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

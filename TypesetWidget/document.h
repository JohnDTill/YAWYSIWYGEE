#ifndef TYPESETSCENE_H
#define TYPESETSCENE_H

#include "cursor.h"
#include "cursorview.h"
#include <QApplication>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QUndoStack>
#include <QTime>

namespace Typeset{

class Construct;
class Line;
class Text;

class Document : public QGraphicsScene{
    Q_OBJECT

public:
    Line* front;
    Line* back;
    Cursor* cursor;
    CursorView* cv;
    const bool allow_write;
    QUndoStack* undo_stack;
    bool show_line_nums = true;

private:
    bool double_clicked = false;
    bool triple_clicked = false;
    QTime double_click_time;
    QPointF click_location;
    static constexpr qreal triple_click_period = 1 * 1000;
    static constexpr qreal allowed_triple_click_travel = 1;
    static constexpr qreal first_line_vspace = 5;

    //Shading appears on left of editor area behind line numbers

    qreal h;
    qreal w;

public:
    Document(bool allow_write = true, bool show_line_numbers = true, Line* f = nullptr, Line* b = nullptr);
    void deletePostorder();
    void setLineNumbersVisible(bool show);
    void updateTheme();
    void write(QTextStream& out) const;
    void updateCursorView();
    void updateSize();

protected:
    virtual void drawBackground(QPainter* painter, const QRectF& rect) override final;
    virtual void keyPressEvent(QKeyEvent* e) override final;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e) override final;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) override final;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* e) override final;

private:
    bool testTripleClick(QPointF click_location) const;
    void processTextInput(const QString& text);
    void processLeftClick(QGraphicsSceneMouseEvent* e);
    void processLeftShiftClick(QGraphicsSceneMouseEvent* e);
    void contextClick(QGraphicsSceneMouseEvent* e);
    void processRightClick(QGraphicsSceneMouseEvent* e, QMenu& menu);
    void processDrag(QGraphicsSceneMouseEvent* e);
    void processWordDrag(QGraphicsSceneMouseEvent* e);

    void processLineSelection(qreal y);
    void processClickMiss(QPointF scene_pos);
    void processShiftWordMiss(QPointF scene_pos);

    void setCursor(const Cursor& c);

private slots:
    void cutSelection();
    void copySelection();
    void paste();
    void selectAll();
};

}

#endif // TYPESETSCENE_H

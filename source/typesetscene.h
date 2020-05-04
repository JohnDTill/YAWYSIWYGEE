#ifndef TYPESETSCENE_H
#define TYPESETSCENE_H

#include <QApplication>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTime>

class Construct;
class Cursor;
class CursorView;
class Line;
class Text;
class QUndoStack;

class TypesetScene : public QGraphicsScene{
    Q_OBJECT

public:
    Line* front;
    Line* back;
    Cursor* cursor;
    CursorView* cv;
    bool allow_write;
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
    static constexpr qreal horizontal_scroll_padding = 8;
    static constexpr qreal linebox_width = 50;
    static constexpr qreal margin_top = 3;
    static constexpr qreal margin_bot = 8;

    //Shading appears on left of editor area behind line numbers

    qreal h;
    qreal w;

public:
    TypesetScene(bool allow_write = true, bool show_line_numbers = true, Line* f = nullptr, Line* b = nullptr);
    ~TypesetScene() override;
    void setLineNumbersVisible(bool show);
    void write(QTextStream& out) const;
    void updateCursorView();
    void updateSize();
    void copyAsPng(qreal upscale = 3);

public slots:
    void copySelectionAsPng(qreal upscale = 3);

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

#endif // TYPESETSCENE_H

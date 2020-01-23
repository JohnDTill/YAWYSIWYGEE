#ifndef TYPESETWIDGET_H
#define TYPESETWIDGET_H

#include <QGraphicsView>

namespace Typeset{

class Document;

class Edit : public QGraphicsView{
    Q_OBJECT

private:
    static constexpr qreal scale_in_factor = 1.1;
    static constexpr qreal scale_out_factor = 0.9;
    static constexpr qreal min_scale = 0.2;
    static constexpr qreal max_scale = 5;
    bool dragging = false;

public:
    Edit();
    void setScene(QGraphicsScene* scene);
    void zoomIn();
    void zoomOut();
    void resetZoom();

private:
    Document* doc();
    qreal heightInSceneCoordinates();

protected:
    virtual void mouseMoveEvent(QMouseEvent* e) override final;
    virtual void wheelEvent(QWheelEvent* event) override final;
    virtual void keyPressEvent(QKeyEvent* e) override final;

private slots:
    void ensureFocusedItemVisible(QGraphicsItem* newFocusItem);
};

}

#endif // TYPESETWIDGET_H

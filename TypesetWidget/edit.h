#ifndef TYPESETWIDGET_H
#define TYPESETWIDGET_H

#include <QGraphicsView>

#include "document.h"

namespace Typeset{

class Edit : public QGraphicsView{
    Q_OBJECT

private:
    Document* doc;
    static constexpr qreal scale_in_factor = 1.1;
    static constexpr qreal scale_out_factor = 0.9;
    static constexpr qreal min_scale = 0.2;
    static constexpr qreal max_scale = 5;

public:
    Edit(bool allow_write = true, bool show_line_numbers = true);
    QString toCode() const;
    void setCode(QString& code);
    QString selectedCode() const;

public slots:
    void newDocument(bool allow_write = true, bool show_line_numbers = true);
    void load(QString filename, bool allow_write = true, bool show_line_numbers = true);
    void loadPrompt(bool allow_write = true, bool show_line_numbers = true);
    void save();
    void saveAs(QString save_path);
    void savePrompt();
    void printSvgPrompt();
    void zoomIn(qreal scale_factor = scale_in_factor);
    void zoomOut(qreal scale_factor = scale_out_factor);
    void resetZoom();
    void setLineNumbersVisible(bool show);
    void copySelectionAsPng(qreal upscale = 3);
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void paste(const QString& str);

signals:
    void undoAvailable(bool available);
    void redoAvailable(bool available);

private:
    void setDocument(Document* scene);
    qreal heightInSceneCoordinates() const;

protected:
    virtual void mouseMoveEvent(QMouseEvent* e) override final;
    virtual void wheelEvent(QWheelEvent* event) override final;
    virtual void keyPressEvent(QKeyEvent* e) override final;

private slots:
    void ensureFocusedItemVisible(QGraphicsItem* newFocusItem);
    void passUndo(bool available);
    void passRedo(bool available);
};

}

#endif // TYPESETWIDGET_H

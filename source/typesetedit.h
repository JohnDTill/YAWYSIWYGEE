#ifndef TYPESETEDIT_H
#define TYPESETEDIT_H

#include <QGraphicsView>
#include <QSvgGenerator>

class TypesetScene;

class TypesetEdit : public QWidget{
    Q_OBJECT

private:
    QString doc_title;
    bool read_only = false;
    bool show_line_numbers = true;

public:
    TypesetEdit(QWidget* parent = nullptr);
    ~TypesetEdit() override;
    QString documentTitle() const;
    QFont font() const;
    bool isReadOnly() const;
    void printSvg(QSvgGenerator* svg_generator, bool draw_background = true) const;
    QString toMathBran() const;
    QString selectedMathBran() const;
    void setDocumentTitle(const QString& title);
    void setPalette(const QPalette& palette);
    void setReadOnly(bool ro);
    void showLineNumbers(bool show = true);

public slots:
    void clear();
    void copy();
    void copyPng(qreal upscale = 3);
    void cut();
    void insertMathBran(const QString& text);
    void insertPlainText(const QString& text);
    void paste();
    void redo();
    void setMathBran(const QString& text);
    void undo();
    void zoomIn(qreal scale_factor = 1.1);
    void zoomOut(qreal scale_factor = 0.9);
    void zoomReset();

signals:
    void redoAvailable(bool available);
    void undoAvailable(bool available);

protected:
    virtual void keyPressEvent(QKeyEvent* e) override final;
    virtual void wheelEvent(QWheelEvent* event) override final;

private:
    TypesetScene* scene;
    QGraphicsView* view;
    void setScene(TypesetScene* scene);
    qreal heightInSceneCoordinates() const;

private slots:
    void ensureFocusedItemVisible(QGraphicsItem* newFocusItem);
    void passUndo(bool available);
    void passRedo(bool available);
};

#endif // TYPESETEDIT_H

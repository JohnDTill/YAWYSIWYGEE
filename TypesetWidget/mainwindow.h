#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "edit.h"
#include "document.h"
#include <QComboBox>

#ifdef _MSC_VER
#define NO_RETURN(method) \
    __declspec(noreturn) method;
#else
#define NO_RETURN(method) \
    method __attribute__((noreturn));
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    Typeset::Edit typeset_edit;
    QComboBox symbol_box;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionPrint_triggered();
    NO_RETURN(void on_actionExit_triggered())

    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

    void on_actionLoad_Test_txt_triggered();
    void on_actionSave_As_Test_txt_triggered();
    NO_RETURN(void on_actionTest_for_Memory_Leaks_triggered())

    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionReset_Zoom_triggered();
    void on_actionShow_Line_Numbers_toggled(bool show);
    void on_actionVanilla_triggered();
    void on_actionChalkboard_triggered();
    void on_actionCopy_as_PNG_triggered();

    void on_actionFraction_triggered();
    void on_actionMatrix_triggered();
    void on_actionCases_triggered();
    void on_actionBinom_triggered();
    void on_actionLim_triggered();
    void on_actionRoot_triggered();
    void on_actionBigint_triggered();
    void on_actionBigsum_triggered();

    void insertChar(const QString& text);

private:
    NO_RETURN(void testForMemoryLeaks())
    bool lineNumbersShown() const;
};

#endif // MAINWINDOW_H

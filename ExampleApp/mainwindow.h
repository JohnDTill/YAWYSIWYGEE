#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <edit.h>
#include <document.h>
#include <QTableWidgetItem>

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

    void insertChar(QTableWidgetItem* item);

    void on_actionSubscript_triggered();
    void on_actionSuperscript_triggered();
    void on_actionDualscript_triggered();
    void on_actionAccentarrow_triggered();
    void on_actionAccentbar_triggered();
    void on_actionAccentbreve_triggered();
    void on_actionAccentdot_triggered();
    void on_actionAccentddot_triggered();
    void on_actionAccentdddot_triggered();
    void on_actionAccenthat_triggered();
    void on_actionAccenttilde_triggered();

    void on_toolButton_triggered(QAction *arg1);
    void on_actionGroupnorm_triggered();
    void on_actionGroupingabs_triggered();
    void on_actionGroupingangle_triggered();
    void on_actionGroupingdangle_triggered();
    void on_actionGroupingceil_triggered();
    void on_actionGroupingfloor_triggered();
    void on_actionEval_triggered();
    void on_groupButton_triggered(QAction *arg1);

private:
    bool lineNumbersShown() const;
    void setupSymbolTable();
};

#endif // MAINWINDOW_H

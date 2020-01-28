#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "globals.h"
#include "parser.h"
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QtMath>
#include <QtSvg/QSvgGenerator>

#include <QMessageBox>
#define FATAL_ERROR(message) {\
    QMessageBox messageBox; \
    messageBox.critical(nullptr, "Error", QString("Fatal Error: ") + message); \
    messageBox.setFixedSize(500,200); \
    exit(0);}

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    setCentralWidget(&typeset_edit);

    connect(&typeset_edit, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(&typeset_edit, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
}

#include <QTextEdit>
MainWindow::~MainWindow(){
    delete ui;
    QTextEdit e;
}

void MainWindow::on_actionNew_triggered(){
    typeset_edit.newDocument();
}

void MainWindow::on_actionLoad_triggered(){
    typeset_edit.loadPrompt();
}

void MainWindow::on_actionSave_triggered(){
    typeset_edit.save();
}

void MainWindow::on_actionSave_As_triggered(){
    typeset_edit.savePrompt();
}

void MainWindow::on_actionPrint_triggered(){
    typeset_edit.printSvgPrompt();
}

void MainWindow::on_actionExit_triggered(){
    exit(0);
}

void MainWindow::on_actionUndo_triggered(){
    typeset_edit.undo();
}

void MainWindow::on_actionRedo_triggered(){
    typeset_edit.redo();
}

void MainWindow::on_actionSave_As_Test_txt_triggered(){
    DO_THIS( "SaveAsTest" )
}

void MainWindow::on_actionLoad_Test_txt_triggered(){
    load(":/test.txt");
}

void MainWindow::on_actionTest_for_Memory_Leaks_triggered(){
    testForMemoryLeaks();
}

void MainWindow::on_actionZoom_In_triggered(){
    typeset_edit.zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered(){
    typeset_edit.zoomOut();
}

void MainWindow::on_actionReset_Zoom_triggered(){
    typeset_edit.resetZoom();
}

void MainWindow::on_actionShow_Line_Numbers_toggled(bool show){
    typeset_edit.setLineNumbersVisible(show);
}

void MainWindow::on_actionVanilla_triggered(){
    ui->actionVanilla->setChecked(true);
    ui->actionChalkboard->setChecked(false);

    Typeset::Globals::background_brush = QBrush(Qt::GlobalColor::white);
    Typeset::Globals::construct_color = Qt::GlobalColor::black;
    Typeset::Globals::construct_brush = QBrush(Typeset::Globals::construct_color);
    Typeset::Globals::construct_pen = QPen(Typeset::Globals::construct_color);
    Typeset::Globals::left_pane_color = QColor::fromRgbF(0.9,0.9,0.9);
    Typeset::Globals::left_pane_pen = QPen(Typeset::Globals::left_pane_color);
    Typeset::Globals::left_pane_brush = QBrush(Typeset::Globals::left_pane_color, Qt::SolidPattern);
    Typeset::Globals::text_color = Qt::GlobalColor::black;
    Typeset::Globals::line_num_active_brush = QBrush(Qt::black);
    Typeset::Globals::line_num_passive_brush = QBrush(Qt::darkGray);
    Typeset::Globals::active_linenum_font.setBold(true);
    Typeset::Globals::empty_box_pen.setColor(Typeset::Globals::construct_color);
    Typeset::Globals::invert_selection_textcolor = true;

    typeset_edit.updateTheme();
}

void MainWindow::on_actionChalkboard_triggered(){
    ui->actionVanilla->setChecked(false);
    ui->actionChalkboard->setChecked(true);

    QColor background_color = QColor::fromRgb(70,133,87);
    Typeset::Globals::background_brush = QBrush(background_color);
    Typeset::Globals::construct_color = Qt::GlobalColor::white;
    Typeset::Globals::construct_brush = QBrush(Typeset::Globals::construct_color);
    Typeset::Globals::construct_pen = QPen(Typeset::Globals::construct_color);
    Typeset::Globals::left_pane_pen = QPen(Typeset::Globals::construct_color);
    Typeset::Globals::left_pane_brush = Typeset::Globals::background_brush;
    Typeset::Globals::text_color = Qt::GlobalColor::white;
    Typeset::Globals::line_num_active_brush = QBrush(Qt::white);
    Typeset::Globals::line_num_passive_brush = QBrush(Qt::white);
    Typeset::Globals::active_linenum_font.setBold(false);
    Typeset::Globals::empty_box_pen.setColor(Typeset::Globals::construct_color);
    Typeset::Globals::invert_selection_textcolor = false;

    typeset_edit.updateTheme();
}

void MainWindow::load(const QString& filename){
    typeset_edit.load(filename);
}

void MainWindow::testForMemoryLeaks(){
    for(;;) on_actionLoad_Test_txt_triggered();
}

bool MainWindow::lineNumbersShown() const{
    return ui->actionShow_Line_Numbers->isChecked();
}

void MainWindow::on_actionCopy_as_PNG_triggered(){
    typeset_edit.copySelectionAsPng();
}

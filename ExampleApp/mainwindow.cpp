#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QtMath>
#include <QtSvg/QSvgGenerator>
#include <QMathBranLatex.h>
#include <QMathBranUnicode.h>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    setCentralWidget(&typeset_edit);
    TypesetToolbar* toolbar = new TypesetToolbar(this);
    connect(toolbar, SIGNAL(clickSymbol(QString)), &typeset_edit, SLOT(insertPlainText(const QString&)));
    connect(toolbar, SIGNAL(clickMathBran(QString)), &typeset_edit, SLOT(insertMathBran(const QString&)));
    connect(toolbar, SIGNAL(clickEnclosingMathBran(QString, QString)),
            this, SLOT(enclosedMathBranButton(QString, QString)));
    addToolBar(toolbar);

    #ifdef __EMSCRIPTEN__
    //WASM cannot access clipboard without ctrl-c/v/x press; GUI buttons don't work
    ui->actionCopy->setVisible(false);
    ui->actionCut->setVisible(false);
    ui->actionCut->setEnabled(false);
    ui->actionPaste->setVisible(false);
    ui->actionCopy_as_PNG->setVisible(false);
    ui->actionCopy_as_TeX->setVisible(false);
    ui->actionCopy_as_Unicode->setVisible(false);

    //Extra setup for WASM
    QLabel* link = new QLabel(this);
    link->setText("<a href=\"https://github.com/JohnDTill/YAWYSIWYGEE\">https://github.com/JohnDTill/YAWYSIWYGEE</a>");
    link->setOpenExternalLinks(true);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(link);
    layout->addWidget(&typeset_edit);
    QWidget* container = new QWidget(this);
    container->setLayout(layout);
    setCentralWidget(container);
    #else
    connect(ui->actionCut, SIGNAL(triggered()), &typeset_edit, SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered()), &typeset_edit, SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered()), &typeset_edit, SLOT(paste()));
    #endif

    connect(&typeset_edit, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(&typeset_edit, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_actionNew_triggered(){
    typeset_edit.clear();
    save_path.clear();
    setWindowTitle("YAWYSIWYGEE");
}

void MainWindow::on_actionLoad_triggered(){
    loadPrompt();
}

void MainWindow::on_actionSave_triggered(){
    save();
}

void MainWindow::on_actionSave_As_triggered(){
    savePrompt();
}

void MainWindow::on_actionPrint_triggered(){
    printSvgPrompt();
}

void MainWindow::on_actionExit_triggered(){
    exit(0);
}

void MainWindow::on_actionUndo_triggered(){
    typeset_edit.undo();
}

void MainWindow::on_actionRedo_triggered(){
    typeset_edit.redo();
    typeset_edit.clearErrors();
}

void MainWindow::on_actionLoad_Test_txt_triggered(){
    load(":/test.txt");
    save_path.clear();
}

void MainWindow::on_actionZoom_In_triggered(){
    typeset_edit.zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered(){
    typeset_edit.zoomOut();
}

void MainWindow::on_actionReset_Zoom_triggered(){
    typeset_edit.zoomReset();
}

void MainWindow::on_actionShow_Line_Numbers_toggled(bool show){
    typeset_edit.showLineNumbers(show);
}

void MainWindow::on_actionVanilla_triggered(){
    ui->actionVanilla->setChecked(true);
    ui->actionChalkboard->setChecked(false);

    typeset_edit.setPalette(palette());
}

void MainWindow::on_actionChalkboard_triggered(){
    ui->actionVanilla->setChecked(false);
    ui->actionChalkboard->setChecked(true);

    QPalette chalkboard = palette();
    chalkboard.setColor(QPalette::All, QPalette::Base, QColor::fromRgb(70,133,87));
    chalkboard.setColor(QPalette::All, QPalette::Text, Qt::white);
    chalkboard.setColor(QPalette::All, QPalette::Mid, Qt::white);
    chalkboard.setColor(QPalette::All, QPalette::Window, QColor::fromRgb(70,133,87));

    typeset_edit.setPalette(chalkboard);
}

void MainWindow::on_actionCopy_as_PNG_triggered(){
    typeset_edit.copyPng();
}

void MainWindow::load(QString filename){
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "Could not open \"" + filename + "\" to read.");
        messageBox.setFixedSize(500,200);
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    QString math_bran = in.readAll();
    if(!MathBran::isWellFormed(math_bran)){
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "\"" + filename + "\" contains invalid MathBran code.");
        messageBox.setFixedSize(500,200);
        return;
    }
    typeset_edit.setMathBran(math_bran);

    QFileInfo file_info(file);
    typeset_edit.setDocumentTitle(file_info.fileName());
    setWindowTitle("YAWYSIWYGEE - \"" + file_info.fileName() + '"');
    save_path = filename;
}

void MainWindow::loadPrompt(){
    QString path = QFileDialog::getOpenFileName(nullptr, tr("Load File"), "./", tr("Text (*.txt)"));
    if(path.isEmpty()) return;
    else load(path);
}

void MainWindow::save(){
    if(save_path.isEmpty()) savePrompt();
    else saveAs(save_path);
}

void MainWindow::saveAs(QString save_path){
    QFile file(save_path);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error", "Could not open \"" + save_path + "\" to write.");
        messageBox.setFixedSize(500,200);
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << typeset_edit.toMathBran();

    QFileInfo file_info(file);
    typeset_edit.setDocumentTitle(file_info.fileName());
    setWindowTitle("YAWYSIWYGEE - \"" + file_info.fileName() + '"');
    this->save_path = save_path;
}

void MainWindow::savePrompt(){
    QString title = typeset_edit.documentTitle();
    QString prompt_name = title.isEmpty() ? "untitled.txt" : typeset_edit.documentTitle();
    QString file_name = QFileDialog::getSaveFileName(nullptr, tr("Save File"),
                                prompt_name,
                                tr("Text (*.txt)"));

    if(!file_name.isEmpty()) saveAs(file_name);
}

void MainWindow::printSvgPrompt(){
    QString title = typeset_edit.documentTitle();
    QString prompt_name = title.isEmpty() ?
                          "untitled.svg" :
                          typeset_edit.documentTitle().split('.').front() + ".svg";
    QString file_name = QFileDialog::getSaveFileName(nullptr, tr("Export PDF"),
                                prompt_name,
                                tr("SVG (*.svg)"));

    if(file_name.isEmpty()) return;

    QSvgGenerator svgGen;
    svgGen.setFileName(file_name);

    typeset_edit.printSvg(&svgGen);
}

void MainWindow::on_actionCopy_as_TeX_triggered(){
    QString math_bran = typeset_edit.selectedMathBran();
    if(!math_bran.isEmpty()) QGuiApplication::clipboard()->setText(MathBran::toLatex(math_bran));
}

void MainWindow::on_actionCopy_as_Unicode_triggered(){
    QString math_bran = typeset_edit.selectedMathBran();
    if(math_bran.isEmpty()) return;

    if(!MathBran::canConvertToUnicode(math_bran)){
        QMessageBox messageBox;
        messageBox.warning(nullptr, "Warning", "Selected text cannot be converted to unicode.");
        messageBox.setFixedSize(500, 200);
        return;
    }

    QGuiApplication::clipboard()->setText(MathBran::toUnicode(math_bran));
}

void MainWindow::enclosedMathBranButton(QString l, QString r){
    QString selected = typeset_edit.selectedMathBran();
    if(selected.contains('\n')) typeset_edit.insertMathBran(l + r);
    else typeset_edit.insertMathBran(l + selected + r);
}

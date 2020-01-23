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
    Typeset::Globals::initGlobals();
    setCentralWidget(&typeset_edit);

    typeset_document = new Typeset::Document(true, lineNumbersShown());
    doc_title = "untitled";
    typeset_edit.setScene(typeset_document);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_actionNew_triggered(){
    typeset_document->deletePostorder();
    typeset_document = new Typeset::Document(true, lineNumbersShown());
    doc_title = "untitled";
    typeset_edit.setScene(typeset_document);

    connect(typeset_document->undo_stack, SIGNAL(canUndoChanged(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(typeset_document->undo_stack, SIGNAL(canRedoChanged(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
}

void MainWindow::on_actionLoad_triggered(){
    QString file_name = QFileDialog::getOpenFileName(nullptr, tr("Load File"),
                                "./",
                                tr("Text (*.txt)"));

    if(!file_name.isEmpty()) load(file_name);
}

void MainWindow::on_actionSave_triggered(){
    if(doc_title == "untitled") on_actionSave_As_triggered();
    else save(doc_title);
}

void MainWindow::on_actionSave_As_triggered(){
    QString file_name = QFileDialog::getSaveFileName(nullptr, tr("Save File"),
                                "./" + doc_title,
                                tr("Text (*.txt)"));

    if(!file_name.isEmpty()) save(file_name);
}

void MainWindow::on_actionPrint_triggered(){
    QString prompt_name = "./" + doc_title + ".svg";
    QString file_name = QFileDialog::getSaveFileName(nullptr, tr("Export PDF"),
                                prompt_name,
                                tr("SVG (*.svg)"));

    if(file_name.isEmpty()) return;

    QRectF bounds = typeset_document->sceneRect();
    int x = qCeil(bounds.x());
    int y = qCeil(bounds.y());
    int w = qCeil(bounds.width());
    int h = qCeil(bounds.height());

    QSvgGenerator svgGen;
    svgGen.setFileName(file_name);
    svgGen.setSize(QSize(w, h));
    svgGen.setViewBox(QRect(x, y, w, h));

    QPainter painter(&svgGen);
    typeset_document->render(&painter, bounds);
}

void MainWindow::on_actionExit_triggered(){
    exit(0);
}

void MainWindow::on_actionUndo_triggered(){
    typeset_document->undo_stack->undo();
}

void MainWindow::on_actionRedo_triggered(){
    typeset_document->undo_stack->redo();
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
    typeset_document->setLineNumbersVisible(show);
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

    typeset_document->updateTheme();
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

    typeset_document->updateTheme();
}

void MainWindow::load(const QString& filename){
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox messageBox;
        messageBox.critical(this, "Error", "Could not open \"" + filename + "\" to read.");
        messageBox.setFixedSize(500,200);
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    if(!Typeset::Parser::isValidCode(in)){
        QMessageBox messageBox;
        messageBox.critical(this, "Error", "Invalid code in \"" + filename + "\".");
        messageBox.setFixedSize(500,200);
        return;
    }

    typeset_document->deletePostorder();
    typeset_document = Typeset::Parser::parseDocument(in, true, lineNumbersShown());
    doc_title = filename.split(".").front();
    typeset_edit.setScene(typeset_document);

    //Make sure the view gets the focus item signal
    QGraphicsItem* focus_item = typeset_document->focusItem();
    typeset_document->clearFocus();
    if(focus_item) focus_item->setFocus();

    connect(typeset_document->undo_stack, SIGNAL(canUndoChanged(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(typeset_document->undo_stack, SIGNAL(canRedoChanged(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
}

void MainWindow::save(const QString& filename){
    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox messageBox;
        messageBox.critical(this, "Error", "Could not open \"" + filename + "\" to write.");
        messageBox.setFixedSize(500,200);
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    typeset_document->write(out);
}

void MainWindow::testForMemoryLeaks(){
    for(;;) on_actionLoad_Test_txt_triggered();
}

bool MainWindow::lineNumbersShown() const{
    return ui->actionShow_Line_Numbers->isChecked();
}

void MainWindow::on_actionCopy_as_PNG_triggered(){
    constexpr qreal upscale = 3;
    QImage image(upscale*typeset_document->sceneRect().size().toSize(), QImage::Format_RGB16);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    typeset_document->render(&painter);
    QApplication::clipboard()->setImage(image, QClipboard::Clipboard);
}

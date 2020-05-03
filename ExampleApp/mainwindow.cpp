#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QTableWidgetItem>
#include <QTextStream>
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

    connect(ui->actionCut, SIGNAL(triggered()), &typeset_edit, SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered()), &typeset_edit, SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered()), &typeset_edit, SLOT(paste()));

    ui->mainToolBar->insertWidget(ui->actionFraction, ui->toolButton);
    ui->toolButton->addAction(ui->actionAccentarrow);
    ui->toolButton->addAction(ui->actionAccentbar);
    ui->toolButton->addAction(ui->actionAccentbreve);
    ui->toolButton->addAction(ui->actionAccentdot);
    ui->toolButton->addAction(ui->actionAccentddot);
    ui->toolButton->addAction(ui->actionAccentdddot);
    ui->toolButton->addAction(ui->actionAccenthat);
    ui->toolButton->addAction(ui->actionAccenttilde);
    ui->toolButton->setDefaultAction(ui->actionAccentarrow);

    ui->mainToolBar->addWidget(ui->groupButton);
    ui->groupButton->addAction(ui->actionGroupingabs);
    ui->groupButton->addAction(ui->actionGroupnorm);
    ui->groupButton->addAction(ui->actionGroupingangle);
    ui->groupButton->addAction(ui->actionGroupingdangle);
    ui->groupButton->addAction(ui->actionGroupingceil);
    ui->groupButton->addAction(ui->actionGroupingfloor);
    ui->groupButton->addAction(ui->actionEval);
    ui->groupButton->setDefaultAction(ui->actionGroupnorm);

    setupSymbolTable();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_actionNew_triggered(){
    typeset_edit.clear();
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
}

void MainWindow::on_actionLoad_Test_txt_triggered(){
    load(":/test.txt");
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

void MainWindow::on_actionFraction_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜f⏴⏵⏴⏵");
    else typeset_edit.insertMathBran("⁜f⏴" + selection + "⏵⏴⏵");
}

void MainWindow::on_actionMatrix_triggered(){
    typeset_edit.insertMathBran("⁜⊞⏴2⏵⏴2⏵");
}

void MainWindow::on_actionCases_triggered(){
    typeset_edit.insertMathBran("⁜c");
}

void MainWindow::on_actionBinom_triggered(){
    typeset_edit.insertMathBran("⁜b");
}

void MainWindow::on_actionLim_triggered(){
    typeset_edit.insertMathBran("⁜l");
}

void MainWindow::on_actionRoot_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜√⏴⏵");
    else typeset_edit.insertMathBran("⁜√⏴" + selection + "⏵");
}

void MainWindow::on_actionBigint_triggered(){
    typeset_edit.insertMathBran("⁜∫");
}

void MainWindow::on_actionBigsum_triggered(){
    typeset_edit.insertMathBran("⁜∑");
}

void MainWindow::insertChar(QTableWidgetItem* item){
    if(item->text().isEmpty()) return;
    typeset_edit.insertPlainText(item->text());

    typeset_edit.setFocus();
    //DO THIS: setFocus() does not have intended effect
}

void MainWindow::on_actionSubscript_triggered(){
    typeset_edit.insertMathBran("⁜_⏴⏵");
}

void MainWindow::on_actionSuperscript_triggered(){
    typeset_edit.insertMathBran("⁜^⏴⏵");
}

void MainWindow::on_actionDualscript_triggered(){
    typeset_edit.insertMathBran("⁜Δ⏴⏵⏴⏵");
}

void MainWindow::on_actionAccentarrow_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜→⏴⏵");
    else typeset_edit.insertMathBran("⁜→⏴" + selection + "⏵");
}

void MainWindow::on_actionAccentbar_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜ā⏴⏵");
    else typeset_edit.insertMathBran("⁜ā⏴" + selection + "⏵");
}

void MainWindow::on_actionAccentbreve_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜ă⏴⏵");
    else typeset_edit.insertMathBran("⁜ă⏴" + selection + "⏵");
}

void MainWindow::on_actionAccentdot_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜ȧ⏴⏵");
    else typeset_edit.insertMathBran("⁜ȧ⏴" + selection + "⏵");
}

void MainWindow::on_actionAccentddot_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜ä⏴⏵");
    else typeset_edit.insertMathBran("⁜ä⏴" + selection + "⏵");
}

void MainWindow::on_actionAccentdddot_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜⋯⏴⏵");
    else typeset_edit.insertMathBran("⁜⋯⏴" + selection + "⏵");
}

void MainWindow::on_actionAccenthat_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜â⏴⏵");
    else typeset_edit.insertMathBran("⁜â⏴" + selection + "⏵");
}

void MainWindow::on_actionAccenttilde_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜ã⏴⏵");
    else typeset_edit.insertMathBran("⁜ã⏴" + selection + "⏵");
}

void MainWindow::on_toolButton_triggered(QAction* action){
    ui->toolButton->setDefaultAction(action);
}

void MainWindow::on_actionGroupnorm_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜‖⏴⏵");
    else typeset_edit.insertMathBran("⁜‖⏴" + selection + "⏵");
}

void MainWindow::on_actionGroupingabs_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜|⏴⏵");
    else typeset_edit.insertMathBran("⁜|⏴" + selection + "⏵");
}

void MainWindow::on_actionGroupingangle_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜⟨⏴⏵");
    else typeset_edit.insertMathBran("⁜⟨⏴" + selection + "⏵");
}

void MainWindow::on_actionGroupingdangle_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜⟪⏴⏵");
    else typeset_edit.insertMathBran("⁜⟪⏴" + selection + "⏵");
}

void MainWindow::on_actionGroupingceil_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜⌈⏴⏵");
    else typeset_edit.insertMathBran("⁜⌈⏴" + selection + "⏵");
}

void MainWindow::on_actionGroupingfloor_triggered(){
    QString selection = typeset_edit.selectedMathBran();
    if(selection.contains('\n')) typeset_edit.insertMathBran("⁜⌊⏴⏵");
    else typeset_edit.insertMathBran("⁜⌊⏴" + selection + "⏵");
}

void MainWindow::on_actionEval_triggered(){
    typeset_edit.insertMathBran("⁜┊⏴⏵⏴⏵");
}

void MainWindow::on_groupButton_triggered(QAction* action){
    ui->groupButton->setDefaultAction(action);
}

#include <YAWYSIWYGEE_keywords.h>
void MainWindow::setupSymbolTable(){
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->insertWidget(ui->actionSubscript, ui->tableWidget);
    ui->mainToolBar->insertWidget(ui->actionSubscript, spacer);

    QFont table_font = typeset_edit.font();
    table_font.setPixelSize(24);
    ui->tableWidget->setFont(table_font);

    int row = 0;
    int col = 0;
    int cols = ui->tableWidget->columnCount();

    std::pair<QString, QString> keywords[] = YAWYSIWYGEE_KEYWORDS;

    for(auto pair : keywords){
        if(col >= cols){
            row++;
            col = 0;
            ui->tableWidget->insertRow(row);
        }

        QTableWidgetItem* item = new QTableWidgetItem(pair.second);
        item->setFlags(item->flags() & ~Qt::ItemFlag::ItemIsEditable);
        item->setToolTip('\\' + pair.first);
        ui->tableWidget->setItem(row,col,item);

        col++;
    }

    //Make empty entries uneditable
    for(int i = col; i < cols; i++){
        QTableWidgetItem* item = new QTableWidgetItem("");
        item->setFlags(item->flags() & ~Qt::ItemFlag::ItemIsEditable);
        ui->tableWidget->setItem(row,i,item);
    }

    connect(ui->tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(insertChar(QTableWidgetItem*)));
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

    typeset_edit.setMathBran(in.readAll());
}

void MainWindow::loadPrompt(){
    QString file_name = QFileDialog::getOpenFileName(nullptr, tr("Load File"),
                                "./",
                                tr("Text (*.txt)"));

    if(file_name.isEmpty()) return;
    else load(file_name);
}

void MainWindow::save(){
    if(typeset_edit.documentTitle().isEmpty()) savePrompt();
    else saveAs(typeset_edit.documentTitle());
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
}

void MainWindow::savePrompt(){
    QString title = typeset_edit.documentTitle();
    QString prompt_name = title.isEmpty() ? "untitled" : typeset_edit.documentTitle() + ".svg";
    QString file_name = QFileDialog::getSaveFileName(nullptr, tr("Save File"),
                                prompt_name,
                                tr("Text (*.txt)"));

    if(!file_name.isEmpty()) saveAs(file_name);
}

void MainWindow::printSvgPrompt(){
    QString title = typeset_edit.documentTitle();
    QString prompt_name = title.isEmpty() ? "untitled" : typeset_edit.documentTitle() + ".svg";
    QString file_name = QFileDialog::getSaveFileName(nullptr, tr("Export PDF"),
                                prompt_name,
                                tr("SVG (*.svg)"));

    if(file_name.isEmpty()) return;

    QSvgGenerator svgGen;
    svgGen.setFileName(file_name);

    typeset_edit.printSvg(&svgGen);
}

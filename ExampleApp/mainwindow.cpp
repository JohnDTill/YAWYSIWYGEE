#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <globals.h>
#include <parser.h>
#include <substitutions.h>
#include <QClipboard>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QtMath>
#include <QtSvg/QSvgGenerator>

#include <QLabel>
#include <QTableWidgetItem>

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
    typeset_edit.newDocument(true, lineNumbersShown());
}

void MainWindow::on_actionLoad_triggered(){
    typeset_edit.loadPrompt(true, lineNumbersShown());
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

void MainWindow::on_actionLoad_Test_txt_triggered(){
    typeset_edit.load(":/test.txt", true, lineNumbersShown());
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

    typeset_edit.setPalette(palette());
    typeset_edit.scene()->setPalette(palette());
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
    typeset_edit.scene()->setPalette(chalkboard);
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

void MainWindow::on_actionFraction_triggered(){
    typeset_edit.paste("⁜f⏴" + typeset_edit.selectedCode() + "⏵⏴⏵");
}

void MainWindow::on_actionMatrix_triggered(){
    typeset_edit.paste("⁜⊞⏴2⏵⏴2⏵");
}

void MainWindow::on_actionCases_triggered(){
    typeset_edit.paste("⁜c");
}

void MainWindow::on_actionBinom_triggered(){
    typeset_edit.paste("⁜b");
}

void MainWindow::on_actionLim_triggered(){
    typeset_edit.paste("⁜w⏴lim⏵");
}

void MainWindow::on_actionRoot_triggered(){
    typeset_edit.paste("⁜√⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionBigint_triggered(){
    typeset_edit.paste("⁜∫");
}

void MainWindow::on_actionBigsum_triggered(){
    typeset_edit.paste("⁜∑");
}

void MainWindow::insertChar(QTableWidgetItem* item){
    if(item->text().isEmpty()) return;
    typeset_edit.paste(item->text());
    typeset_edit.setFocus();
}

void MainWindow::on_actionSubscript_triggered(){
    typeset_edit.paste("⁜_⏴" + typeset_edit.selectedCode() + "⏵⏴⏵");
}

void MainWindow::on_actionSuperscript_triggered(){
    typeset_edit.paste("⁜^⏴" + typeset_edit.selectedCode() + "⏵⏴⏵");
}

void MainWindow::on_actionDualscript_triggered(){
    typeset_edit.paste("⁜Δ⏴" + typeset_edit.selectedCode() + "⏵⏴⏵⏴⏵");
}

void MainWindow::on_actionAccentarrow_triggered(){
    typeset_edit.paste("⁜→⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionAccentbar_triggered(){
    typeset_edit.paste("⁜ā⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionAccentbreve_triggered(){
    typeset_edit.paste("⁜ă⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionAccentdot_triggered(){
    typeset_edit.paste("⁜ȧ⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionAccentddot_triggered(){
    typeset_edit.paste("⁜ä⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionAccentdddot_triggered(){
    typeset_edit.paste("⁜⋯⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionAccenthat_triggered(){
    typeset_edit.paste("⁜â⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionAccenttilde_triggered(){
    typeset_edit.paste("⁜ã⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_toolButton_triggered(QAction* action){
    ui->toolButton->setDefaultAction(action);
}

void MainWindow::on_actionGroupnorm_triggered(){
    typeset_edit.paste("⁜‖‖⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionGroupingabs_triggered(){
    typeset_edit.paste("⁜||⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionGroupingangle_triggered(){
    typeset_edit.paste("⁜⟨⟩⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionGroupingdangle_triggered(){
    typeset_edit.paste("⁜⟪⟫⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionGroupingceil_triggered(){
    typeset_edit.paste("⁜⌈⌉⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionGroupingfloor_triggered(){
    typeset_edit.paste("⁜⌊⌋⏴" + typeset_edit.selectedCode() + "⏵");
}

void MainWindow::on_actionEval_triggered(){
    typeset_edit.paste("⁜Δ⏴⁜┊|⏴" + typeset_edit.selectedCode() + "⏵⏵⏴⏵⏴⏵");
}

void MainWindow::on_groupButton_triggered(QAction* action){
    ui->groupButton->setDefaultAction(action);
}

void MainWindow::setupSymbolTable(){
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->insertWidget(ui->actionSubscript, ui->tableWidget);
    ui->mainToolBar->insertWidget(ui->actionSubscript, spacer);

    QFont table_font = Typeset::Globals::fonts[0];
    table_font.setPixelSize(24);
    ui->tableWidget->setFont(table_font);

    QMap<QString, QString>::const_iterator i;
    int row = 0;
    int col = 0;
    int cols = ui->tableWidget->columnCount();
    for(i = Typeset::keyword_map.begin(); i != Typeset::keyword_map.end(); i++){
        if(col >= cols){
            row++;
            col = 0;
            ui->tableWidget->insertRow(row);
        }

        QTableWidgetItem* item = new QTableWidgetItem(i.value());
        item->setFlags(item->flags() & ~Qt::ItemFlag::ItemIsEditable);
        item->setToolTip('\\' + i.key());
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

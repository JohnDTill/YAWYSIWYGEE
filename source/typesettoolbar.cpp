#include "typesettoolbar.h"

#include "globals.h"
#include "YAWYSIWYGEE_keywords.h"
#include <QHeaderView>
#include <QTableWidget>
#include <QToolButton>

TypesetToolbar::TypesetToolbar(QWidget* parent) : QToolBar(parent) {
    Qt::ToolBarAreas a;
    a.setFlag(Qt::ToolBarArea::TopToolBarArea);
    a.setFlag(Qt::ToolBarArea::BottomToolBarArea);
    setAllowedAreas(a);
    Globals::initGlobals();
    setupSymbolTable();

    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    addWidget(spacer);

    setupScripts();
    setupAccents();
    setupMisc();
    setupBigSymbols();
    setupIntegrals();
    setupGroupings();
}

void TypesetToolbar::setupSymbolTable(){
    //Set up dimensions
    static constexpr int symbol_table_cols = 12;
    int row = 0;
    int col = 0;

    //Set properties
    symbol_table = new QTableWidget((YAWYSIWYGEE_NUM_KEYWORDS-1)/symbol_table_cols+1, symbol_table_cols, this);
    QFont table_font = Globals::fonts[0];
    table_font.setPointSize(12);
    symbol_table->setFont(table_font);
    symbol_table->setFocusPolicy(Qt::NoFocus);
    symbol_table->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    symbol_table->setMaximumHeight(60);
    symbol_table->setMinimumWidth(30*symbol_table_cols + 28);
    symbol_table->verticalHeader()->setVisible(false);
    symbol_table->horizontalHeader()->setVisible(false);
    symbol_table->verticalHeader()->setMinimumSectionSize(5);
    symbol_table->horizontalHeader()->setMinimumSectionSize(5);
    symbol_table->verticalHeader()->setDefaultSectionSize(30);
    symbol_table->horizontalHeader()->setDefaultSectionSize(30);
    connect( symbol_table, SIGNAL(itemClicked(QTableWidgetItem*)),
             this, SLOT(symbolTableTriggered(QTableWidgetItem*)) );

    //Add keywords to table
    std::pair<QString, QString> keywords[] = YAWYSIWYGEE_KEYWORDS;
    for(auto pair : keywords){
        if(col >= symbol_table_cols){
            row++;
            col = 0;
        }

        QTableWidgetItem* item = new QTableWidgetItem(pair.second);
        item->setFlags(item->flags() & ~Qt::ItemFlag::ItemIsEditable);
        item->setToolTip('\\' + pair.first);
        symbol_table->setItem(row,col,item);
        col++;
    }

    //Make empty entries uneditable
    for(int i = col; i < symbol_table_cols; i++){
        QTableWidgetItem* item = new QTableWidgetItem("");
        item->setFlags(item->flags() & ~Qt::ItemFlag::ItemIsEditable);
        symbol_table->setItem(row,i,item);
    }

    addWidget(symbol_table);
}

void TypesetToolbar::setupScripts(){
    addAction(new TypesetAction("A", "\\_{subscript}", "⁜_⏴⏵", this));
    addAction(new TypesetAction("B", "\\^{superscript}", "⁜^⏴⏵", this));
    addAction(new TypesetAction("C", "\\_^{subscript}{superscript}", "⁜Δ⏴⏵⏴⏵", this));
}

void TypesetToolbar::setupAccents(){
    QToolButton* accents = new QToolButton(this);
    accents->setPopupMode(QToolButton::MenuButtonPopup);
    connect(accents, SIGNAL(triggered(QAction*)), accents, SLOT(setDefaultAction(QAction*)));
    addWidget(accents);

    accents->addAction(new EnclosedTypesetAction("J", "\\vec", "⁜→⏴", "⏵", this));
    accents->addAction(new EnclosedTypesetAction("K", "\\bar", "⁜ā⏴", "⏵", this));
    accents->addAction(new EnclosedTypesetAction("L", "\\breve", "⁜ă⏴", "⏵", this));
    accents->addAction(new EnclosedTypesetAction("M", "\\dot", "⁜ȧ⏴", "⏵", this));
    accents->addAction(new EnclosedTypesetAction("N", "\\ddot", "⁜ä⏴", "⏵", this));
    accents->addAction(new EnclosedTypesetAction("O", "\\dddot", "⁜⋯⏴", "⏵", this));
    accents->addAction(new EnclosedTypesetAction("P", "\\hat", "⁜â⏴", "⏵", this));
    accents->addAction(new EnclosedTypesetAction("Q", "\\tilde", "⁜ã⏴", "⏵", this));

    accents->setDefaultAction(accents->actions().front());
}

void TypesetToolbar::setupMisc(){
    addAction(new EnclosedTypesetAction("D", "\\frac", "⁜f⏴", "⏵⏴⏵", this));
    addAction(new TypesetAction("E", "\\mat{rows}{cols}", "⁜⊞⏴2⏵⏴2⏵", this));
    addAction(new TypesetAction("F", "\\cases", "⁜c⏴⏵⏴⏵⏴⏵⏴⏵", this));
    addAction(new TypesetAction("G", "\\binom{n}{k}", "⁜b⏴n⏵⏴k⏵", this));
    addAction(new TypesetAction("H", "\\lim{var}{to}", "⁜l⏴⏵⏴⏵", this));
    addAction(new EnclosedTypesetAction("I", "\\root", "⁜√⏴", "⏵", this));
}

void TypesetToolbar::setupBigSymbols(){
    QToolButton* bigs = new QToolButton(this);
    bigs->setPopupMode(QToolButton::MenuButtonPopup);
    connect(bigs, SIGNAL(triggered(QAction*)), bigs, SLOT(setDefaultAction(QAction*)));
    addWidget(bigs);

    bigs->addAction(new TypesetAction("∑", "\\sum", "⁜∑", this));
    bigs->addAction(new TypesetAction("∏", "\\prod", "⁜∏", this));
    bigs->addAction(new TypesetAction("∐", "\\coprod", "⁜∐", this));
    bigs->addAction(new TypesetAction("⋂", "\\bigcap", "⁜⋂", this));
    bigs->addAction(new TypesetAction("⋃", "\\bigcup", "⁜⋃", this));
    bigs->addAction(new TypesetAction("⊎", "\\biguplus", "⁜⨄", this));

    bigs->setDefaultAction(bigs->actions().front());
}

void TypesetToolbar::setupIntegrals(){
    QToolButton* ints = new QToolButton(this);
    ints->setPopupMode(QToolButton::MenuButtonPopup);
    connect(ints, SIGNAL(triggered(QAction*)), ints, SLOT(setDefaultAction(QAction*)));
    addWidget(ints);

    ints->addAction(new TypesetAction("∫", "\\int", "⁜∫", this));
    ints->addAction(new TypesetAction("∬", "\\iint", "⁜∬", this));
    ints->addAction(new TypesetAction("∭", "\\iiint", "⁜∭", this));
    ints->addAction(new TypesetAction("∮", "\\oint", "⁜∮", this));
    ints->addAction(new TypesetAction("∯", "\\oiint", "⁜∯", this));
    ints->addAction(new TypesetAction("∰", "\\oiiint", "⁜∰", this));

    ints->setDefaultAction(ints->actions().front());
}

void TypesetToolbar::setupGroupings(){
    QToolButton* groups = new QToolButton(this);
    groups->setPopupMode(QToolButton::MenuButtonPopup);
    connect(groups, SIGNAL(triggered(QAction*)), groups, SLOT(setDefaultAction(QAction*)));
    addWidget(groups);

    groups->addAction(new EnclosedTypesetAction("‖⬚‖", "\\norm", "⁜‖⏴", "⏵", this));
    groups->addAction(new EnclosedTypesetAction("|⬚|", "\\abs", "⁜|⏴", "⏵", this));
    groups->addAction(new EnclosedTypesetAction("⌈⬚⌉", "\\ceil", "⁜⌈⏴", "⏵", this));
    groups->addAction(new EnclosedTypesetAction("⌊⬚⌋", "\\floor", "⁜⌊⏴", "⏵", this));
    groups->addAction(new EnclosedTypesetAction("(⬚)", "\\()", "⁜(⏴", "⏵", this));
    groups->addAction(new EnclosedTypesetAction("[⬚]", "\\[]", "⁜[⏴", "⏵", this));
    groups->addAction(new TypesetAction("R", "\\eval", "⁜┊⏴a⏵⏴b⏵", this));

    groups->setDefaultAction(groups->actions().front());
}

void TypesetToolbar::emitMathBran(){
    emit clickMathBran(static_cast<TypesetAction*>(sender())->command);
}

void TypesetToolbar::emitEnclosedMathBran(){
    EnclosedTypesetAction* a = static_cast<EnclosedTypesetAction*>(sender());
    emit clickEnclosingMathBran(a->command_start, a->command_end);
}

void TypesetToolbar::symbolTableTriggered(QTableWidgetItem* item){
    if(item->text().isEmpty()) return;
    emit clickSymbol(item->text());
}

TypesetToolbar::TypesetAction::TypesetAction(const QString& text,
                                             const QString& tooltip,
                                             const QString& command,
                                             TypesetToolbar* parent)
    : QAction(text, parent), command(command) {
    setToolTip(tooltip);
    setFont(Globals::glyph_font);
    connect(this, SIGNAL(triggered(bool)), parent, SLOT(emitMathBran()));
}

TypesetToolbar::EnclosedTypesetAction::EnclosedTypesetAction(const QString& text,
                                                             const QString& tooltip,
                                                             const QString& command_start,
                                                             const QString& command_end,
                                                             TypesetToolbar* parent)
    : QAction(text, parent), command_start(command_start), command_end(command_end) {
    setToolTip(tooltip);
    setFont(Globals::glyph_font);
    connect(this, SIGNAL(triggered(bool)), parent, SLOT(emitEnclosedMathBran()));
}

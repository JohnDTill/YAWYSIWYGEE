#ifndef TYPESETTOOLBAR_H
#define TYPESETTOOLBAR_H

#include <QToolBar>
class QTableWidget;
class QTableWidgetItem;

class TypesetToolbar : public QToolBar{
    Q_OBJECT

private:
    QTableWidget* symbol_table;

public:
    TypesetToolbar(QWidget* parent = nullptr);

signals:
    void clickMathBran(QString);
    void clickEnclosingMathBran(QString, QString);
    void clickSymbol(QString);

private:
    void setupSymbolTable();
    void setupScripts();
    void setupAccents();
    void setupMisc();
    void setupBigSymbols();
    void setupIntegrals();
    void setupGroupings();

private slots:
    void symbolTableTriggered(QTableWidgetItem* item);
    void emitMathBran();
    void emitEnclosedMathBran();

private:
    class TypesetAction : public QAction{
    public:
        const QString command;
        TypesetAction(const QString& text,
                      const QString& tooltip,
                      const QString& command,
                      TypesetToolbar* parent);
    };

    class EnclosedTypesetAction : public QAction{
    public:
        const QString command_start;
        const QString command_end;
        EnclosedTypesetAction(const QString& text,
                              const QString& tooltip,
                              const QString& command_start,
                              const QString& command_end,
                              TypesetToolbar* parent);
    };
};

#endif // TYPESETTOOLBAR_H

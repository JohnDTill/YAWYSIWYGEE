#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    //Should look into the QPalette paradigm for customization
    QPalette p = a.palette();
    //p.setBrush(QPalette::Active, QPalette::Highlight, QBrush(QColor("pink")));
    p.setBrush(QPalette::Inactive, QPalette::Highlight, QBrush(QColor(0, 120, 215)));
    //p.setBrush(QPalette::Active, QPalette::HighlightedText, QBrush(QColor("green")));
    p.setBrush(QPalette::Inactive, QPalette::HighlightedText, QBrush(QColor("white")));
    QApplication::setPalette(p, "QTextControl");
    a.setPalette(p);

    MainWindow w;
    w.show();

    return a.exec();
}

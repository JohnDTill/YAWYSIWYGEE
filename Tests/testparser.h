#ifndef TESTPARSER_H
#define TESTPARSER_H

#include <QtTest/QtTest>

class TestParser : public QObject{
    Q_OBJECT

private slots:
    void validateTestFile();
    void loadSaveTestFile();
};

#endif // TESTPARSER_H

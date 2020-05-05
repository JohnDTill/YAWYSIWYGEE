#ifndef TESTPARSER_H
#define TESTPARSER_H

#include <QtTest/QtTest>

class Test : public QObject{
    Q_OBJECT

private slots:
    void validateTestFile();
    void loadSaveTestFile();
    void typeHereIGoAgain();
    void typeRotationAndDelta();
    void BENCHMARK_validateTestFile();
    void BENCHMARK_loadTestFile();
    void BENCHMARK_saveTestFile();
    void BENCHMARK_typeHereIGoAgain();
    void BENCHMARK_eraseHereIGoAgain();
    void BENCHMARK_undoHereIGoAgain();
    void BENCHMARK_redoHereIGoAgain();
    void BENCHMARK_typeRotationAndDelta();
};

#endif // TESTPARSER_H

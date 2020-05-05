#include "test.h"

#include <YAWYSIWYGEE>
#include <MathBran/include/QMathBran.h>
#include <typesetscene.h>
#include <construct.h>
#include <phrase.h>
#include <text.h>

void Test::validateTestFile(){
    QFile test_file(":test.txt");
    QVERIFY(test_file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&test_file);
    in.setCodec("UTF-8");
    QString test_code = in.readAll();

    QVERIFY(MathBran::isWellFormed(test_code));
}

void Test::loadSaveTestFile(){
    QFile test_file(":test.txt");
    QVERIFY(test_file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&test_file);
    in.setCodec("UTF-8");
    QString test_code = in.readAll();

    TypesetEdit edit;
    edit.setMathBran(test_code);
    QCOMPARE(edit.toMathBran(), test_code);
}

void Test::typeHereIGoAgain(){
    TypesetEdit edit;
    TypesetScene* scene = edit.getTypesetScene();
    scene->macroHereIGoAgainTyping();

    QVERIFY(Phrase::verify());
    QVERIFY(Construct::verify());
    QVERIFY(Text::verify());

    scene->macroHereIGoAgainDeletion();

    QVERIFY(Phrase::verify());
    QVERIFY(Construct::verify());
    QVERIFY(Text::verify());

    scene->macroHereIGoAgainUndo();

    QVERIFY(Phrase::verify());
    QVERIFY(Construct::verify());
    QVERIFY(Text::verify());

    scene->macroHereIGoAgainRedo();

    QVERIFY(Phrase::verify());
    QVERIFY(Construct::verify());
    QVERIFY(Text::verify());
}

void Test::typeRotationAndDelta(){
    TypesetEdit edit;
    TypesetScene* scene = edit.getTypesetScene();
    scene->macroMath();

    QVERIFY(Phrase::verify());
    QVERIFY(Construct::verify());
    QVERIFY(Text::verify());
}

void Test::BENCHMARK_validateTestFile(){
    QFile test_file(":test.txt");
    QVERIFY(test_file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&test_file);
    in.setCodec("UTF-8");
    QString test_code = in.readAll();

    QBENCHMARK{
        MathBran::isWellFormed(test_code);
    }
}

void Test::BENCHMARK_loadTestFile(){
    QFile test_file(":test.txt");
    QVERIFY(test_file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&test_file);
    in.setCodec("UTF-8");
    QString test_code = in.readAll();

    QBENCHMARK{
        TypesetEdit edit;
        edit.setMathBran(test_code);
    }
}

void Test::BENCHMARK_saveTestFile(){
    QFile test_file(":test.txt");
    QVERIFY(test_file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&test_file);
    in.setCodec("UTF-8");
    QString test_code = in.readAll();

    TypesetEdit edit;
    edit.setMathBran(test_code);

    QBENCHMARK{
        edit.toMathBran();
    }
}

void Test::BENCHMARK_typeHereIGoAgain(){
    TypesetEdit edit;
    TypesetScene* scene = edit.getTypesetScene();

    QBENCHMARK{
        scene->macroHereIGoAgainTyping();
    }
}

void Test::BENCHMARK_eraseHereIGoAgain(){
    TypesetEdit edit;
    TypesetScene* scene = edit.getTypesetScene();

    scene->macroHereIGoAgainTyping();

    QBENCHMARK{
        scene->macroHereIGoAgainDeletion();
    }
}

void Test::BENCHMARK_undoHereIGoAgain(){
    TypesetEdit edit;
    TypesetScene* scene = edit.getTypesetScene();

    scene->macroHereIGoAgainTyping();
    scene->macroHereIGoAgainDeletion();

    QBENCHMARK{
        scene->macroHereIGoAgainUndo();
    }
}

void Test::BENCHMARK_redoHereIGoAgain(){
    TypesetEdit edit;
    TypesetScene* scene = edit.getTypesetScene();

    scene->macroHereIGoAgainTyping();
    scene->macroHereIGoAgainDeletion();
    scene->macroHereIGoAgainUndo();

    QBENCHMARK{
        scene->macroHereIGoAgainRedo();
    }
}

void Test::BENCHMARK_typeRotationAndDelta(){
    TypesetEdit edit;
    TypesetScene* scene = edit.getTypesetScene();

    QBENCHMARK{
        scene->macroMath();
    }
}

QTEST_MAIN(Test)

#include "testparser.h"

#include <edit.h>
#include <parser.h>

void TestParser::validateTestFile(){
    QFile test_file(":test.txt");
    QVERIFY(test_file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&test_file);
    in.setCodec("UTF-8");
    QString test_code = in.readAll();

    QVERIFY(Typeset::Parser::isValidCode(test_code));
}

void TestParser::loadSaveTestFile(){
    QFile test_file(":test.txt");
    QVERIFY(test_file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&test_file);
    in.setCodec("UTF-8");
    QString test_code = in.readAll();

    Typeset::Edit edit;
    edit.setMathBran(test_code);
    QCOMPARE(edit.toMathBran(), test_code);
}

QTEST_MAIN(TestParser)

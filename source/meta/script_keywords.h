#ifndef SCRIPT_KEYWORDS_H
#define SCRIPT_KEYWORDS_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>

struct KeywordEntry{
    QString keyword;
    QString symbol;

    KeywordEntry(QString keyword, QString symbol)
        : keyword(keyword), symbol(symbol) {}
};

int processKeywords(){
    //Open table file
    QFile table(":/TableKeywords.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

    //Process table file
    std::vector<KeywordEntry> rows;
    table.readLine(); //Discard headers
    unsigned line = 1;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        QList<QString> entries = line_str.split(",");

        QString keyword = entries.at(0);
        QString symbol = entries.at(1);

        rows.push_back(KeywordEntry(keyword, symbol));
    }

    //Open gen file for writing
    QFile file("../YAWYSIWYGEE_keywords.h");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << file.errorString();
        return 0;
    }

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForMib(106)); //UTF8

    //Front matter
    out << "//CODE-GEN FILE\n"
           "//This file is generated from subfolder \"meta\".\n"
           "//Changes to this file must be made in the meta project.\n"
           "\n";

    //Macro definition
    out << "#define YAWYSIWYGEE_KEYWORDS {\\\n";

    for(KeywordEntry e : rows)
        out << "    { \"" << e.keyword << "\" , \"" << e.symbol << "\" },\\\n";
    out << "}\n"
           "\n"
           "#define YAWYSIWYGEE_NUM_KEYWORDS " << rows.size() << "\n";

    //Cleanup
    file.close();

    return 0;
}

#endif // SCRIPT_KEYWORDS_H

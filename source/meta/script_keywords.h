#ifndef SCRIPT_KEYWORDS_H
#define SCRIPT_KEYWORDS_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QtMath>

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
           "#define YAWYSIWYGEE_NUM_KEYWORDS " << rows.size() << "\n\n";

    //Lookup function signature
    out << "class QString;\n"
           "namespace Keywords {\n"
           "QString lookup(const QString& key);\n"
           "}\n";

    //Cleanup
    file.close();


    //Open gen file for writing
    QFile cpp_file("../YAWYSIWYGEE_keywords.cpp");
    if(!cpp_file.open(QIODevice::WriteOnly)){
        qDebug() << cpp_file.errorString();
        return 0;
    }

    QTextStream cpp(&cpp_file);
    cpp.setCodec(QTextCodec::codecForMib(106)); //UTF8

    //Front matter
    cpp << "//CODE-GEN FILE\n"
           "//This file is generated from subfolder \"meta\".\n"
           "//Changes to this file must be made in the meta project.\n"
           "\n";

    //Find longest keyword
    uint16_t max_length = 0;
    for(KeywordEntry e : rows)
        max_length = qMax(max_length, static_cast<uint16_t>(e.keyword.length()));

    //Lookup function definition
    cpp << "#include \"YAWYSIWYGEE_keywords.h\"\n"
           "#include <QString>\n"
           "\n"
           "namespace Keywords {\n"
           "\n"
           "QString lookup(const QString& key){\n"
           "    if(key.isEmpty() || key.size() > " << max_length << ") return QString();\n"
           "\n"
           "    uint32_t hash = key[0].unicode();\n"
           "    for(uint8_t i = static_cast<uint8_t>(key.size())-1; i > 0; i--)\n"
           "        hash += (static_cast<uint32_t>(key[i].unicode()) << 3*i);\n"
           "\n"
           "    switch(hash){\n";

    for(KeywordEntry e : rows){
        QString key = e.keyword;
        uint32_t hash = key[0].unicode();
        for(uint8_t i = static_cast<uint8_t>(key.size())-1; i > 0; i--)
            hash += (static_cast<uint32_t>(key[i].unicode()) << 3*i);

        cpp << "        case " << hash << ":";
        for(int i = 0; i < 12 - QString::number(hash).size(); i++) cpp << ' ';
        cpp << " return key==\"" << key << "\"";
        for(int i = 0; i < 16 - key.size(); i++) cpp << ' ';
        cpp << "?   \"" << e.symbol << "\" : QString();\n";
    }

    cpp << "        default: return QString();\n"
           "    }\n"
           "}\n"
           "\n"
           "};\n";

    //Cleanup
    cpp_file.close();

    return 0;
}

#endif // SCRIPT_KEYWORDS_H

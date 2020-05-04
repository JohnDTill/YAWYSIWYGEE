#ifndef SCRIPT_ADHOC_H
#define SCRIPT_ADHOC_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>

struct AdhocEntry{
    std::vector<std::pair<QChar, QChar> > data;
    QChar second;

    AdhocEntry(QChar second) : second(second) {}
    QChar first(uint i){return data.at(i).first;}
    QChar sub(uint i){return data.at(i).second;}
};

int processAdhocSubstitutions(){
    //Open table file
    QFile table(":/TableAdhocSymbols.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

    //Process table file
    std::vector<AdhocEntry> rows;
    table.readLine(); //Discard headers
    unsigned line = 1;
    QChar examined;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        QList<QString> entries = line_str.split(",");

        if(entries.size() < 3){
            qDebug() << "Line " << line << " does not have 3 entries";
            continue;
        }else if(entries[0].size() != 1){
            qDebug() << "First entry of line " << line << " is not size 1";
            continue;
        }else if(entries[1].size() != 1){
            qDebug() << "Second entry of line " << line << " is not size 1";
            continue;
        }else if(entries[2].size() != 1){
            qDebug() << "Third entry of line " << line << " is not size 1";
            continue;
        }

        QChar first = entries[0].front();
        QChar second = entries[1].front();
        QChar sub = entries[2].front();

        if(second != examined){
            examined = second;
            rows.push_back(AdhocEntry(examined));
        }

        rows.back().data.push_back(std::pair<QChar,QChar>(first, sub));
    }

    //Open gen file for writing
    QFile file("../cursor_adhoc_sub.cpp");
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
           "\n"
           "#include \"cursor.h\"\n"
           "\n";

    out << "//Could check for space ' ' in IF statement, then have single switch off both\n"
           "//32-bit QChars as a 64-bit switch value.\n\n";

    //Method definition
    out << "void Cursor::checkForSubstitution(const QChar& c){\n"
           "    if(cursor.position() < 2) return;\n"
           "\n"
           "    switch(c.unicode()){\n"
           "        case ' ': checkSlashSub(); break;\n";

    for(AdhocEntry e : rows){

        out << "        case " << e.second.unicode() << ":\n"
               "            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);\n"
               "            switch(cursor.selectedText().front().unicode()){\n";

        for(auto datum : e.data){
            out << "                case " << datum.first.unicode() << ": "
                   "cursor.clearSelection(); keystroke(QChar(" << datum.second.unicode() << ")); break;\n";
        }
        out << "                default: cursor.movePosition(QTextCursor::Right);\n"
               "            }\n"
               "            break;\n";
    }
    out << "    }\n"
           "}\n";

    //Cleanup
    file.close();

    return 0;
}

#endif // SCRIPT_ADHOC_H

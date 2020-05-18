#ifndef SCRIPT_COMMANDS_H
#define SCRIPT_COMMANDS_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QtMath>

struct CommandEntry{
    QString command;
    QString code;
    QString default_args;

    CommandEntry(QString command, QString code, QString default_args)
        : command(command), code(code), default_args(default_args) {}
};

int processCommands(){
    //Open table file
    QFile table(":/TableCommands.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

    //Process table file
    std::vector<CommandEntry> rows;
    table.readLine(); //Discard headers
    unsigned line = 1;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        QList<QString> entries = line_str.split(",");

        QString command = entries.at(0);
        QString code = entries.at(1);
        QString default_args = entries.at(2);

        rows.push_back(CommandEntry(command, code, default_args));
    }

    //Open gen file for writing
    QFile file("../YAWYSIWYGEE_commands.h");
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

    //Lookup function signature
    out << "class QString;\n"
           "namespace Commands {\n"
           "QString lookup(const QString& key);\n"
           "}\n";

    //Cleanup
    file.close();


    //Open gen file for writing
    QFile cpp_file("../YAWYSIWYGEE_commands.cpp");
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


    //Try to find a suitable hash function
    uint8_t n = 0;
    for(;;){
        QList<uint8_t> hashes;
        for(CommandEntry e : rows){
            QString key = e.command;
            uint8_t hash = static_cast<uint8_t>(key[0].unicode());
            for(uint8_t i = static_cast<uint8_t>(key.size())-1; i > 0; i--)
                hash += static_cast<uint8_t>(key[i].unicode())*(i+n);

            if(hashes.contains(hash)){
                n++;
                if(n==0){
                    cpp << "//FAILED TO BUILD HASH FUNCTION";
                    qDebug() << "Failed with to build hash function";
                    return 0;
                }
                break;
            }

            hashes.push_back(hash);
        }

        if(hashes.size() == static_cast<int>(rows.size())) break;
    }

    uint16_t max_length = 0;
    for(CommandEntry e : rows)
        max_length = qMax(max_length, static_cast<uint16_t>(e.command.length()));

    //Lookup function definition
    cpp << "#include \"YAWYSIWYGEE_commands.h\"\n"
           "#include \"MathBran/include/QMathBran_Macros.h\"\n"
           "#include <QString>\n"
           "\n"
           "namespace Commands {\n"
           "\n"
           "QString lookup(const QString& key){\n"
           "    if(key.isEmpty() || key.size() > " << max_length << ") return QString();\n"
           "\n"
           "    uint8_t hash = static_cast<uint8_t>(key[0].unicode());\n"
           "    for(uint8_t i = static_cast<uint8_t>(key.size())-1; i > 0; i--)\n"
           "        hash += static_cast<uint8_t>(key[i].unicode())*(i+" << n << ");\n"
           "\n"
           "    switch(hash){\n";

    for(CommandEntry e : rows){
        QString key = e.command;
        uint8_t hash = static_cast<uint8_t>(key[0].unicode());
        for(uint8_t i = static_cast<uint8_t>(key.size())-1; i > 0; i--)
            hash += static_cast<uint8_t>(key[i].unicode())*(i+n);

        cpp << "        case " << hash << ":";
        for(int i = 0; i < 3 - QString::number(hash).size(); i++) cpp << ' ';
        cpp << " return key==\"" << key << "\"";
        for(int i = 0; i < 9 - key.size(); i++) cpp << ' ';
        cpp << "?  QString(" << e.code << ")";
        if(!e.default_args.isEmpty()) cpp << "+\"" << e.default_args << "\"";
        cpp << " : QString();\n";
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

#endif // SCRIPT_COMMANDS_H

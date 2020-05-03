#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextCodec>

void skipHeader(QFile& table_file){
    table_file.readLine(); //Discard headers
}

void createIncludes(QTextStream& out){
    out << "//CODE-GEN FILE\n"
           "//This file is generated from subfolder \"meta\".\n"
           "//Changes to this file must be made in the meta project.\n"
           "\n"
           "#include <QString>\n"
           "#include <QHash>\n"
           "#include <QMap>\n"
           "\n";
}

void createSubstitutionMap(QFile& table_file, QTextStream& out){
    skipHeader(table_file);

    out << "//Note: value type is QString because QChar uses UTF-16, but some values are UTF-32 characters\n";
    out << "static const QHash<QString, QString> keyword_map = {\n";

    int max_keyword_length = 0;
    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        QList<QString> entries = line_str.split(',');

        QString shortcut = entries.at(8);
        QString keyword = entries.at(0);
        QString value = entries.at(1);

        keyword = shortcut.isEmpty() ? keyword : shortcut;

        bool no_typesetting = entries.at(4).isEmpty();
        bool not_latex_only = entries.at(6).isEmpty();

        if(no_typesetting && not_latex_only){
            if(keyword.length() > max_keyword_length)
                max_keyword_length = keyword.length();

            out << "    { \"" << keyword << "\" , \"" << value << "\" },\n";
        }
    }
    out << "};\n\n";
    out << "static constexpr int max_keyword_length = " << max_keyword_length << ";\n\n";

    table_file.reset();
}

void createAdhocSwitch(QFile& table_file, QTextStream& out){
    skipHeader(table_file);
    out << "//CODE-GEN FILE\n"
           "//This file is generated from subfolder \"meta\".\n"
           "//Changes to this file must be made in the meta project.\n"
           "\n"
           "#include \"cursor.h\"\n"
           "\n"
           "template<ushort indicator, ushort sub>\n"
           "static void checkTwoCharSub(Cursor& c, QTextCursor cursor){\n"
           "    cursor.movePosition(QTextCursor::Left);\n"
           "    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);\n"
           "    if(cursor.selectedText() == indicator){\n"
           "        c.selectPreviousChar();\n"
           "        c.selectPreviousChar();\n"
           "        c.keystroke(QChar(sub));\n"
           "    }\n"
           "}\n"
           "\n"
           "void Cursor::checkForSubstitution(const QChar& c){\n"
           "    if(cursor.position() < 2) return;\n"
           "\n"
           "    switch(c.unicode()){\n"
           "        case ' ':\n"
           "            checkSlashSub();\n"
           "            break;\n";

    ushort curr = 0;
    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        QList<QString> entries = line_str.split(',');

        if(entries.at(1).front().unicode() != curr){
            if(curr != 0) out << "            break;\n";

            curr = entries.at(1).front().unicode();
            QString code = QChar(curr);
            if(code == '\\') code = "\\\\";
            out << "        case '" << code << "':\n";
        }

        out << "            checkTwoCharSub<" << entries.at(0).front().unicode() << ',' <<
               entries.at(2).front().unicode() << ">(*this, cursor);\n";
    }

    out << "    }\n}\n";

    table_file.reset();
}

void createKeywordMacro(QFile& table_file, QTextStream& out){
    skipHeader(table_file);

    out << "#define YAWYSIWYGEE_KEYWORDS {\\\n";

    int max_keyword_length = 0;
    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        QList<QString> entries = line_str.split(',');

        QString shortcut = entries.at(8);
        QString keyword = entries.at(0);
        QString value = entries.at(1);

        keyword = shortcut.isEmpty() ? keyword : shortcut;

        bool no_typesetting = entries.at(4).isEmpty();
        bool not_latex_only = entries.at(6).isEmpty();

        if(no_typesetting && not_latex_only){
            if(keyword.length() > max_keyword_length)
                max_keyword_length = keyword.length();

            out << "    { \"" << keyword << "\" , \"" << value << "\" },\\\n";
        }
    }
    out << "}\n";

    table_file.reset();
}

int main(int, char**){
    QFile keyword_table(":/TableKeywords.csv");
    if(!keyword_table.open(QIODevice::ReadOnly)){
        qDebug() << keyword_table.errorString();
        return 0;
    }

    if(keyword_table.atEnd()){
        qDebug() << "File is empty.";
        return 0;
    }

    QFile adhoc_table(":/TableAdhocSymbols.csv");
    if(!adhoc_table.open(QIODevice::ReadOnly)){
        qDebug() << adhoc_table.errorString();
        return 0;
    }

    QFile keyword_gen_file("../substitutions.h");
    if(!keyword_gen_file.open(QIODevice::WriteOnly)){
        qDebug() << keyword_gen_file.errorString();
        return 0;
    }

    QFile adhoc_gen_file("../cursor_adhoc_sub.cpp");
    if(!adhoc_gen_file.open(QIODevice::WriteOnly)){
        qDebug() << adhoc_gen_file.errorString();
        return 0;
    }

    QFile include_keyword_gen_file("../../include/YAWYSIWYGEE_keywords.h");
    if(!include_keyword_gen_file.open(QIODevice::WriteOnly)){
        qDebug() << include_keyword_gen_file.errorString();
        return 0;
    }

    QTextStream out_keyword(&keyword_gen_file);
    out_keyword.setCodec(QTextCodec::codecForMib(106)); //UTF8
    createIncludes(out_keyword);
    createSubstitutionMap(keyword_table, out_keyword);

    QTextStream out_include_keyword(&include_keyword_gen_file);
    out_include_keyword.setCodec(QTextCodec::codecForMib(106)); //UTF8
    createKeywordMacro(keyword_table, out_include_keyword);
    keyword_table.close();

    QTextStream out_adhoc(&adhoc_gen_file);
    createAdhocSwitch(adhoc_table, out_adhoc);
    adhoc_gen_file.close();

    QDir dir("./");
    dir.removeRecursively();

    return 0;
}

#include <QDebug>
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
           "#include <string>\n"
           "#include <QString>\n"
           "#include <QHash>\n"
           "\n"
           "namespace Typeset{\n\n";
}

void createSubstitutionMap(QFile& table_file, QTextStream& out){
    skipHeader(table_file);

    out << "static const QMap<QString, QChar> keyword_to_qchar = {\n";

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

            if(value.size()==1)
                out << "    { \"" << keyword << "\" , " << value.front().unicode() << " },\n";
        }
    }
    out << "};\n\n";
    out << "static constexpr int max_keyword_length = " << max_keyword_length << ";\n\n";

    table_file.reset();
}

void createLatexMap(QFile& table_file, QTextStream& out){
    skipHeader(table_file);
    out << "static const QHash<QChar, std::string> qchar_to_latex = {\n";

    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        QList<QString> entries = line_str.split(',');

        QString shortcut = entries.at(8);
        QString keyword = entries.at(0);
        QString value = entries.at(1);

        keyword = shortcut.isEmpty() ? keyword : shortcut;

        bool no_typesetting = entries.at(4).isEmpty();

        if(no_typesetting && value.size()==1)
            out << "    { " << value.front().unicode() << " , \"" << keyword << "\" },\n";
    }
    out << "};\n\n";

    table_file.reset();
}

int main(int, char**){
    QFile table_file(":/Table.csv");
    if(!table_file.open(QIODevice::ReadOnly)){
        qDebug() << table_file.errorString();
        return 0;
    }

    if(table_file.atEnd()){
        qDebug() << "File is empty.";
        return 0;
    }

    QFile gen_file("../substitutions.h");
    if(!gen_file.open(QIODevice::WriteOnly)){
        qDebug() << gen_file.errorString();
        return 0;
    }

    QTextStream out(&gen_file);
    out.setCodec(QTextCodec::codecForMib(106)); //UTF8
    createIncludes(out);
    createSubstitutionMap(table_file, out);
    createLatexMap(table_file, out);
    out << "}\n";
    gen_file.close();

    return 0;
}

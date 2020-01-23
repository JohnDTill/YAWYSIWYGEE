#include <QDebug>
#include <QFile>
#include <QTextCodec>

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
    table_file.readLine(); //Discard headers

    QFile keyword_to_qstring_file("../LatexSymbols/keywordtoqstring.h");
    if(!keyword_to_qstring_file.open(QIODevice::WriteOnly)){
        qDebug() << keyword_to_qstring_file.errorString();
        return 0;
    }
    keyword_to_qstring_file.write("#include <string>\n"
                                  "#include <QString>\n"
                                  "#include <QHash>\n"
                                  "\n"
                                  "namespace LatexSymbols{\n"
                                  "const QHash<const std::string, QString> keyword_to_qstring = {\n");

    QFile keyword_to_qchar_file("../LatexSymbols/keywordtoqchar.h");
    if(!keyword_to_qchar_file.open(QIODevice::WriteOnly)){
        qDebug() << keyword_to_qchar_file.errorString();
        return 0;
    }
    keyword_to_qchar_file.write("#include <string>\n"
                                "#include <QChar>\n"
                                "#include <QHash>\n"
                                "\n"
                                "namespace LatexSymbols{\n"
                                "const QHash<QString, QChar> keyword_to_qchar = {\n");

    QFile qchar_to_keyword_file("../LatexSymbols/qchartokeyword.h");
    if(!qchar_to_keyword_file.open(QIODevice::WriteOnly)){
        qDebug() << qchar_to_keyword_file.errorString();
        return 0;
    }
    qchar_to_keyword_file.write("#include <QChar>\n"
                                "#include <QString>\n"
                                "#include <QHash>\n"
                                "\n"
                                "namespace LatexSymbols{\n"
                                "const QHash<QChar, QString> qchar_to_keyword = {\n");

    int typeset_column = 4;
    int latex_only_column = 6;
    int shortcut_column = 8;

    const int utf8_code = 106;
    int code = utf8_code;

    bool first_qstring = true;
    bool first_qchar = true;
    bool first_keyword = true;

    int max_keyword_length = 0;

    while(!table_file.atEnd()){
        QByteArray line = table_file.readLine();
        QString line_str = QTextCodec::codecForMib(code)->toUnicode(line);
        QList<QString> entries = line_str.split(',');
        qDebug() << line;

        QString shortcut = entries.at(8);
        QString keyword = entries.at(0);
        QString value = entries.at(1);

        keyword = shortcut.isEmpty() ? keyword : shortcut;

        bool no_typesetting = entries.at(typeset_column).isEmpty();
        bool not_latex_only = entries.at(latex_only_column).isEmpty();

        if(no_typesetting && not_latex_only){
            if(keyword.length() > max_keyword_length){
                max_keyword_length = keyword.length();
            }

            if(!first_qstring) keyword_to_qstring_file.write(",\n");
            keyword_to_qstring_file.write(("    { \"" + keyword + "\" , \"" + value + "\" }").toUtf8());
            first_qstring = false;

            if(value.size()==1){
                if(!first_qchar) keyword_to_qchar_file.write(",\n");
                keyword_to_qchar_file.write(("    { \"" + keyword + "\" , " + QString::number(value.at(0).unicode()) + " }").toUtf8());
                first_qchar = false;
            }
        }

        if(no_typesetting && value.size()==1){
            if(!first_keyword) qchar_to_keyword_file.write(",\n");
            qchar_to_keyword_file.write(("    { " + QString::number(value.at(0).unicode()) + " , \"" + keyword + "\" }").toUtf8());
            first_keyword = false;
        }
    }

    keyword_to_qstring_file.write("\n};\n}\n");
    keyword_to_qstring_file.close();

    keyword_to_qchar_file.write("\n};\n\n");
    keyword_to_qchar_file.write(("const uint_fast8_t max_keyword_length = " + QString::number(max_keyword_length) + ";\n}\n").toUtf8());
    keyword_to_qchar_file.close();

    qchar_to_keyword_file.write("\n};\n}\n");
    qchar_to_keyword_file.close();

    return 0;
}

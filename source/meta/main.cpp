#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextCodec>

#include "script_adhoc.h"
#include "script_keywords.h"

int main(int, char**){
    processKeywords();
    processAdhocSubstitutions();

    QDir dir("./");
    dir.removeRecursively();

    return 0;
}

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextCodec>

#include "script_adhoc.h"
#include "script_keywords.h"
#include "script_commands.h"

int main(int, char**){
    processKeywords();
    processAdhocSubstitutions();
    processCommands();

    QDir dir("./");
    dir.removeRecursively();

    return 0;
}

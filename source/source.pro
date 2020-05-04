QT += core gui svg widgets

TEMPLATE = lib
DEFINES += QT_DEPRECATED_WARNINGS  #YAWYSIWYGEE_TEST
CONFIG += c++11
CONFIG += staticlib
DESTDIR = $$shell_path($$_PRO_FILE_PWD_)/../lib
TARGET = YAWYSIWYGEE

SOURCES += \
    algorithm.cpp \
    construct.cpp \
    construct/group.cpp \
    cursor.cpp \
    cursor_adhoc_sub.cpp \
    cursorview.cpp \
    globals.cpp \
    line.cpp \
    parser.cpp \
    phrase.cpp \
    subphrase.cpp \
    text.cpp \
    typesetedit.cpp \
    command/commanddeletechar.cpp \
    command/commanddeletemultiline.cpp \
    command/commanddeletephrase.cpp \
    command/commanddeletetext.cpp \
    command/commandevalmultiline.cpp \
    command/commandevalphrase.cpp \
    command/commandinsertchar.cpp \
    command/commandinsertmultiline.cpp \
    command/commandinserttext.cpp \
    command/commandlist.cpp \
    construct/accent.cpp \
    construct/bigqchar.cpp \
    construct/binomial.cpp \
    construct/cases.cpp \
    construct/fraction.cpp \
    construct/integral.cpp \
    construct/limit.cpp \
    construct/matrix.cpp \
    construct/root.cpp \
    construct/script.cpp \
    construct/underscriptedword.cpp \
    typesetscene.cpp

HEADERS += \
    YAWYSIWYGEE \
    algorithm.h \
    construct.h \
    construct/constructs.h \
    construct/group.h \
    cursor.h \
    cursorview.h \
    globals.h \
    line.h \
    parser.h \
    phrase.h \
    subphrase.h \
    substitutions.h \
    text.h \
    typesetedit.h \
    command/commanddeletechar.h \
    command/commanddeletemultiline.h \
    command/commanddeletephrase.h \
    command/commanddeletetext.h \
    command/commandevalmultiline.h \
    command/commandevalphrase.h \
    command/commandinsertchar.h \
    command/commandinsertmultiline.h \
    command/commandinserttext.h \
    command/commandlist.h \
    construct/accent.h \
    construct/bigqchar.h \
    construct/binomial.h \
    construct/cases.h \
    construct/fraction.h \
    construct/integral.h \
    construct/limit.h \
    construct/matrix.h \
    construct/root.h \
    construct/script.h \
    construct/underscriptedword.h \
    typesetscene.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += qtypesetobjectresource.qrc


# copies the given files to the destination directory
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g

        QMAKE_PRE_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_PRE_LINK)
}

copyToDestDir($$PWD/"YAWYSIWYGEE", $$PWD/../include)
copyToDestDir($$PWD/"TypesetEdit.h", $$PWD/../include)

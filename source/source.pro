QT += core gui svg widgets

TEMPLATE = lib
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += YAWYSIWYGEE_TEST
CONFIG += c++11
CONFIG += staticlib
TARGET = YAWYSIWYGEE

CONFIG(debug, debug|release) {
    DESTDIR = $$shell_path($$_PRO_FILE_PWD_)/../lib/debug
} else {
    DESTDIR = $$shell_path($$_PRO_FILE_PWD_)/../lib/release
}

SOURCES += \
    algorithm.cpp \
    command/deletechar.cpp \
    command/insertchar.cpp \
    command/multilinecommand.cpp \
    command/paircommand.cpp \
    command/phrasecommand.cpp \
    command/textcommand.cpp \
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
    typesetscene.cpp \
    YAWYSIWYGEE_keywords.cpp \
    YAWYSIWYGEE_commands.cpp \
    typesettoolbar.cpp

HEADERS += \
    YAWYSIWYGEE \
    algorithm.h \
    command/commands.h \
    command/deletechar.h \
    command/insertchar.h \
    command/multilinecommand.h \
    command/paircommand.h \
    command/phrasecommand.h \
    command/textcommand.h \
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
    text.h \
    typesetedit.h \
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
    typesetscene.h \
    YAWYSIWYGEE_keywords.h \
    YAWYSIWYGEE_commands.h \
    typesettoolbar.h


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
copyToDestDir($$PWD/"typesetedit.h", $$PWD/../include)
copyToDestDir($$PWD/"typesettoolbar.h", $$PWD/../include)

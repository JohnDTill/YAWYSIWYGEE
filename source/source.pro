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
    cursor.cpp \
    cursor_adhoc_sub.cpp \
    cursorview.cpp \
    globals.cpp \
    line.cpp \
    parser.cpp \
    phrase.cpp \
    scene.cpp \
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
    construct/grouping.cpp \
    construct/integral.cpp \
    construct/limit.cpp \
    construct/matrix.cpp \
    construct/root.cpp \
    construct/script.cpp \
    construct/underscriptedword.cpp

HEADERS += \
    YAWYSIWYGEE \
    algorithm.h \
    construct.h \
    cursor.h \
    cursorview.h \
    globals.h \
    line.h \
    parser.h \
    phrase.h \
    scene.h \
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
    construct/grouping.h \
    construct/integral.h \
    construct/limit.h \
    construct/matrix.h \
    construct/root.h \
    construct/script.h \
    construct/underscriptedword.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += qtypesetobjectresource.qrc

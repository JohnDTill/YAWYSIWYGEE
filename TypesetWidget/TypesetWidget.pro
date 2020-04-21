QT += core gui svg widgets

TARGET = TypesetWidget
TEMPLATE = lib
DEFINES += QT_DEPRECATED_WARNINGS  #YAWYSIWYGEE_TEST
CONFIG += c++11
CONFIG += staticlib
DESTDIR = $$shell_path($$_PRO_FILE_PWD_)/../lib

SOURCES += \
    accent.cpp \
    algorithm.cpp \
    bigqchar.cpp \
    binomial.cpp \
    cases.cpp \
    commanddeletechar.cpp \
    commanddeletemultiline.cpp \
    commanddeletephrase.cpp \
    commanddeletetext.cpp \
    commandevalmultiline.cpp \
    commandevalphrase.cpp \
    commandinsertchar.cpp \
    commandinsertmultiline.cpp \
    commandinserttext.cpp \
    commandlist.cpp \
    construct.cpp \
    cursor.cpp \
    cursor_adhoc_sub.cpp \
    cursorview.cpp \
    document.cpp \
    edit.cpp \
    fraction.cpp \
    globals.cpp \
    grouping.cpp \
    integral.cpp \
    limit.cpp \
    line.cpp \
    matrix.cpp \
    parser.cpp \
    phrase.cpp \
    root.cpp \
    script.cpp \
    subphrase.cpp \
    text.cpp \
    underscriptedword.cpp

HEADERS += \
    accent.h \
    algorithm.h \
    bigqchar.h \
    binomial.h \
    cases.h \
    commanddeletechar.h \
    commanddeletemultiline.h \
    commanddeletephrase.h \
    commanddeletetext.h \
    commandevalmultiline.h \
    commandevalphrase.h \
    commandinsertchar.h \
    commandinsertmultiline.h \
    commandinserttext.h \
    commandlist.h \
    construct.h \
    cursor.h \
    cursorview.h \
    document.h \
    edit.h \
    fraction.h \
    globals.h \
    grouping.h \
    integral.h \
    limit.h \
    line.h \
    matrix.h \
    parser.h \
    phrase.h \
    root.h \
    script.h \
    subphrase.h \
    substitutions.h \
    text.h \
    underscriptedword.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += qtypesetobjectresource.qrc

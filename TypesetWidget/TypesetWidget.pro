QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TypesetWidget
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SUBDIRS += ../LatexSymbols

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
    cursorview.cpp \
    document.cpp \
    edit.cpp \
    fraction.cpp \
    globals.cpp \
    grouping.cpp \
    integral.cpp \
    line.cpp \
    main.cpp \
    mainwindow.cpp \
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
    line.h \
    mainwindow.h \
    matrix.h \
    parser.h \
    phrase.h \
    root.h \
    script.h \
    subphrase.h \
    substitutions.h \
    text.h \
    underscriptedword.h

FORMS += mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += qtypesetobjectresource.qrc

RC_ICONS += lambda.ico

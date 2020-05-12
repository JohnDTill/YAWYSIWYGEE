QT += core gui svg widgets

TARGET = TypesetWidget
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11
INCLUDEPATH += ../include
INCLUDEPATH += ../ExampleApp
INCLUDEPATH += ../source

SOURCES += ../ExampleApp/main.cpp \
           ../ExampleApp/mainwindow.cpp

HEADERS += ../ExampleApp/mainwindow.h

SOURCES += \
    ../source/algorithm.cpp \
    ../source/command/deletechar.cpp \
    ../source/command/deletemultiline.cpp \
    ../source/command/deletephrase.cpp \
    ../source/command/deletetext.cpp \
    ../source/command/evalmultiline.cpp \
    ../source/command/evalphrase.cpp \
    ../source/command/insertchar.cpp \
    ../source/command/insertmultiline.cpp \
    ../source/command/inserttext.cpp \
    ../source/construct.cpp \
    ../source/construct/group.cpp \
    ../source/cursor.cpp \
    ../source/cursor_adhoc_sub.cpp \
    ../source/cursorview.cpp \
    ../source/globals.cpp \
    ../source/line.cpp \
    ../source/parser.cpp \
    ../source/phrase.cpp \
    ../source/subphrase.cpp \
    ../source/text.cpp \
    ../source/typesetedit.cpp \
    ../source/command/commandlist.cpp \
    ../source/construct/accent.cpp \
    ../source/construct/bigqchar.cpp \
    ../source/construct/binomial.cpp \
    ../source/construct/cases.cpp \
    ../source/construct/fraction.cpp \
    ../source/construct/integral.cpp \
    ../source/construct/limit.cpp \
    ../source/construct/matrix.cpp \
    ../source/construct/root.cpp \
    ../source/construct/script.cpp \
    ../source/construct/underscriptedword.cpp \
    ../source/typesetscene.cpp \
    ../source/typesettoolbar.cpp

HEADERS += \
    ../source/YAWYSIWYGEE \
    ../source/algorithm.h \
    ../source/command/commands.h \
    ../source/command/deletechar.h \
    ../source/command/deletemultiline.h \
    ../source/command/deletephrase.h \
    ../source/command/deletetext.h \
    ../source/command/evalmultiline.h \
    ../source/command/evalphrase.h \
    ../source/command/insertchar.h \
    ../source/command/insertmultiline.h \
    ../source/command/inserttext.h \
    ../source/construct.h \
    ../source/construct/constructs.h \
    ../source/construct/group.h \
    ../source/cursor.h \
    ../source/cursorview.h \
    ../source/globals.h \
    ../source/line.h \
    ../source/parser.h \
    ../source/phrase.h \
    ../source/subphrase.h \
    ../source/text.h \
    ../source/typesetedit.h \
    ../source/command/commandlist.h \
    ../source/construct/accent.h \
    ../source/construct/bigqchar.h \
    ../source/construct/binomial.h \
    ../source/construct/cases.h \
    ../source/construct/fraction.h \
    ../source/construct/integral.h \
    ../source/construct/limit.h \
    ../source/construct/matrix.h \
    ../source/construct/root.h \
    ../source/construct/script.h \
    ../source/construct/underscriptedword.h \
    ../source/typesetscene.h \
    ../source/typesettoolbar.h \
    ../source/YAWYSIWYGEE_keywords.h

RESOURCES += ../source/qtypesetobjectresource.qrc

FORMS += ../ExampleApp/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += ../ExampleApp/example_resource.qrc
RC_ICONS += ../ExampleApp/lambda.ico

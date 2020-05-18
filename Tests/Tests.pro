QT += core gui svg testlib widgets

CONFIG += c++11
INCLUDEPATH += ../source
DEFINES += YAWYSIWYGEE_TEST

HEADERS += \
    test.h

SOURCES += \
    test.cpp \
    typesetscene_tests.cpp

SOURCES += \
    ../source/algorithm.cpp \
    ../source/command/deletechar.cpp \
    ../source/command/multilinecommand.cpp \
    ../source/command/phrasecommand.cpp \
    ../source/command/insertchar.cpp \
    ../source/command/textcommand.cpp \
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
    ../source/command/paircommand.cpp \
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
    ../source/YAWYSIWYGEE_keywords.cpp \
    ../source/YAWYSIWYGEE_commands.cpp

HEADERS += \
    ../source/YAWYSIWYGEE \
    ../source/algorithm.h \
    ../source/command/commands.h \
    ../source/command/deletechar.h \
    ../source/command/multilinecommand.h \
    ../source/command/phrasecommand.h \
    ../source/command/insertchar.h \
    ../source/command/textcommand.h \
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
    ../source/command/paircommand.h \
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
    ../source/YAWYSIWYGEE_keywords.h \
    ../source/YAWYSIWYGEE_commands.h

RESOURCES += ../source/qtypesetobjectresource.qrc

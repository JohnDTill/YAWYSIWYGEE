QT += core

TARGET = meta
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += main.cpp

HEADERS += script_keywords.h \
           ../YAWYSIWYGEE_keywords.h \
           ../YAWYSIWYGEE_commands.h \
           script_adhoc.h \
           script_commands.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += TableKeywords.csv \
             TableCommands.csv \
             TableAdhocSymbols.csv \
             ../cursor_adhoc_sub.cpp \
             ../YAWYSIWYGEE_keywords.cpp \
             ../YAWYSIWYGEE_commands.cpp

RESOURCES += TableKeywords.csv \
             TableCommands.csv \
             TableAdhocSymbols.csv

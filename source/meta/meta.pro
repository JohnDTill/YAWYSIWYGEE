QT += core

TARGET = meta
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += main.cpp

HEADERS += script_keywords.h \
           ../YAWYSIWYGEE_keywords.h \
           script_adhoc.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += TableKeywords.csv \
             TableAdhocSymbols.csv \
             ../cursor_adhoc_sub.cpp

RESOURCES += TableKeywords.csv \
             TableAdhocSymbols.csv

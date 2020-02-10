QT += core gui svg testlib widgets

CONFIG += c++11
INCLUDEPATH += ../TypesetWidget

HEADERS += \
    testparser.h

SOURCES += \
    testparser.cpp

win32: LIBS += -L$$PWD/../lib/ -lTypesetWidget

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/TypesetWidget.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/libTypesetWidget.a

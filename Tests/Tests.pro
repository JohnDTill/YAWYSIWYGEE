QT += core gui svg testlib widgets

CONFIG += c++11
INCLUDEPATH += ../include
DEFINES += YAWYSIWYGEE_TEST

HEADERS += testparser.h

SOURCES += testparser.cpp

LIBS += -L$$PWD/../lib/ -lYAWYSIWYGEE

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/YAWYSIWYGEE.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/libYAWYSIWYGEE.a

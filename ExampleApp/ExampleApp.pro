QT += core gui svg widgets

TARGET = TypesetWidget
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS  #YAWYSIWYGEE_TEST
CONFIG += c++11
INCLUDEPATH += ../include

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += example_resource.qrc
RC_ICONS += lambda.ico

LIBS += -L$$PWD/../lib/ -lYAWYSIWYGEE

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/YAWYSIWYGEE.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/libYAWYSIWYGEE.a

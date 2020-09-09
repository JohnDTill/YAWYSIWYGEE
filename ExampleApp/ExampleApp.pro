QT += core gui svg widgets

TARGET = TypesetWidget
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
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

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../lib/debug/ -lYAWYSIWYGEE
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/debug/YAWYSIWYGEE.lib
    else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/debug/libYAWYSIWYGEE.a
}else{
    LIBS += -L$$PWD/../lib/release/ -lYAWYSIWYGEE
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/release/YAWYSIWYGEE.lib
    else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/release/libYAWYSIWYGEE.a
}

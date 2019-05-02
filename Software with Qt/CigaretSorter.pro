#-------------------------------------------------
#
# Project created by QtCreator 2019-04-29T10:36:03
#
#-------------------------------------------------

QT       += core gui
QT += widgets printsupport

TARGET = CigaretSorter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        arduinohelper.cpp \
        avgimgplot.cpp \
        main.cpp \
        mainwindow.cpp \
        mpvwidget.cpp \
        qcustomplot.cpp

HEADERS += \
        arduinohelper.h \
        avgimgplot.h \
        mainwindow.h \
        mpvwidget.h \
        qcustomplot.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += /usr/local/include
LIBS += "/usr/local/Cellar/mpv/0.29.1_3/lib/libmpv.1.101.0.dylib"
LIBS += "/usr/local/Cellar/libserialport/0.1.1/lib/libserialport.0.dylib"
LIBS += -dead_strip
CONFIG += app_bundle

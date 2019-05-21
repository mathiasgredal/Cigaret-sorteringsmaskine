#-------------------------------------------------
#
# Project created by QtCreator 2019-04-29T10:36:03
#
#-------------------------------------------------

QT       += core gui
QT += widgets printsupport network

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
        cpr/auth.cpp \
        cpr/cookies.cpp \
        cpr/cprtypes.cpp \
        cpr/digest.cpp \
        cpr/error.cpp \
        cpr/multipart.cpp \
        cpr/parameters.cpp \
        cpr/payload.cpp \
        cpr/proxies.cpp \
        cpr/session.cpp \
        cpr/ssl_options.cpp \
        cpr/timeout.cpp \
        cpr/util.cpp \
        main.cpp \
        mainwindow.cpp \
        mpvwidget.cpp \
        neural_network_preview.cpp \
        nn_interface.cpp
HEADERS += \
        arduinohelper.h \
        avgimgplot.h \
        cpr/api.h \
        cpr/auth.h \
        cpr/body.h \
        cpr/cookies.h \
        cpr/cpr.h \
        cpr/cprtypes.h \
        cpr/curlholder.h \
        cpr/defines.h \
        cpr/digest.h \
        cpr/error.h \
        cpr/low_speed.h \
        cpr/max_redirects.h \
        cpr/multipart.h \
        cpr/parameters.h \
        cpr/payload.h \
        cpr/proxies.h \
        cpr/response.h \
        cpr/session.h \
        cpr/ssl_options.h \
        cpr/timeout.h \
        cpr/util.h \
        mainwindow.h \
        mpvwidget.h \
        neural_network_preview.h \
        nn_interface.h
FORMS += \
        mainwindow.ui


INCLUDEPATH += /usr/local/include
LIBS += "/usr/local/Cellar/mpv/0.29.1_3/lib/libmpv.1.101.0.dylib"
LIBS += "/usr/local/Cellar/libserialport/0.1.1/lib/libserialport.0.dylib"
LIBS += "/usr/local/Cellar/curl/7.64.1/lib/libcurl.4.dylib"
CONFIG += app_bundle

RESOURCES +=



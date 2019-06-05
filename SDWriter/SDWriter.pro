#-------------------------------------------------
#
# Project created by QtCreator 2018-06-14T20:37:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SDWriter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sddirectaccess.cpp \
    dhfs.cpp \
    selectdevice.cpp \
    filemidplane.cpp

HEADERS  += mainwindow.h \
    sddirectaccess.h \
    dhfs.h \
    selectdevice.h \
    filemidplane.h

FORMS    += mainwindow.ui \
    selectdevice.ui

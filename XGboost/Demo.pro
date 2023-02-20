#-------------------------------------------------
#
# Project created by QtCreator 2023-02-18T14:40:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app

DESTDIR     =  $$PWD/bin
INCLUDEPATH += $$PWD/inc
INCLUDEPATH += $$PWD/inc/python
LIBS += -L$$PWD/libs             \
              -lpython3          \
              -lpython38

DEFINES += QT_DEPRECATED_WARNINGS



CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resource.qrc

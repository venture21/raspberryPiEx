#-------------------------------------------------
#
# Project created by QtCreator 2017-05-30T13:44:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ledControl
TEMPLATE = app


SOURCES  += main.cpp\
            mainwindow.cpp \
            gpioled.cpp

LIBS     += -lwiringPi

HEADERS  += mainwindow.h \
            gpioled.h \
    gpioled.h

FORMS    += mainwindow.ui

#-------------------------------------------------
#
# Project created by QtCreator 2019-04-25T16:47:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtApp
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp
SOURCES += PiWidget.cpp

HEADERS  += withQt.h
LIBS += -lpthread -lwiringPi

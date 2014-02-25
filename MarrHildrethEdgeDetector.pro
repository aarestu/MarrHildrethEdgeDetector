#-------------------------------------------------
#
# Project created by QtCreator 2014-02-25T03:46:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = MarrHildrethEdgeDetector
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += D://opencv//sources//opencv_bin//install//include

LIBS += D://opencv//sources//opencv_bin//bin//*.dll

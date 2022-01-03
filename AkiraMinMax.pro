#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T13:41:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = AkiraMinMax
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

CONFIG += c++11

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui

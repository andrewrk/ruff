#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T19:01:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ruff
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    csv.cpp \
    commandparser.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    csv.h \
    commandparser.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    resources.qrc

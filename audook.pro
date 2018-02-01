#-------------------------------------------------
#
# Project created by QtCreator 2018-02-01T15:51:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = audook
TEMPLATE = app
LIBS+=-lpodofo
LIBS+=-lespeak

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    TextExtractor.cpp \
    mainwindow.cpp \
    main.cpp \
    credits_window.cpp \
    about_window.cpp

HEADERS += \
    TextExtractor.h \
    mainwindow.h \
    json_control.h \
    extension.h \
    credits_window.h \
    about_window.h

RESOURCES += \
    resources.qrc \
    images.qrc

FORMS += \
    mainwindow.ui \
    credits_window.ui \
    about_window.ui

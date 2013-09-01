#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T23:10:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImmaLaunchinMahMinecraft
TEMPLATE = app


SOURCES += main.cpp\
        ilmm.cpp \
    gamelauncher.cpp \
    auth.cpp \
    filedownloader.cpp \
    versions.cpp \
    downloader.cpp \
    options.cpp

HEADERS  += ilmm.h \
    gamelauncher.h \
    auth.h \
    filedownloader.h \
    versions.h \
    downloader.h \
    options.h

FORMS    += ilmm.ui \
    options.ui

RESOURCES += \
    Res.qrc

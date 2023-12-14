QT += core
QT -= gui

TARGET = st
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    stlist.cpp \
    stable.cpp

HEADERS += \
    stentry.h \
    stlist.h \
    stable.h


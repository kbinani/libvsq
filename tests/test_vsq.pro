#-------------------------------------------------
#
# Project created by QtCreator 2012-02-13T03:20:30
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = test_vsq
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    TimesigTest.cpp \
    main.cpp \
    TimesigListTest.cpp \
    MeasureLineIteratorTest.cpp \
    LyricTest.cpp \
    StringUtilTest.cpp \
    PhoneticSymbolTest.cpp \
    VibratoBPTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../vsq/
LIBS += -lvsq -lcppunit -L"../build/macosx/release"

HEADERS += \
    Util.h

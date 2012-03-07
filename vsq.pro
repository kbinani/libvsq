#-------------------------------------------------
#
# Project created by QtCreator 2012-02-13T03:14:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = vsq
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    Timesig.cpp \
    TimesigList.cpp \
    MeasureLineIterator.cpp \
    MeasureLine.cpp \
    Lyric.cpp \
    PhoneticSymbol.cpp \
    VibratoBP.cpp \
    VibratoBPList.cpp \
    Tempo.cpp \
    TextStream.cpp \
    Handle.cpp \
    Event.cpp \
    EventList.cpp \
    BPList.cpp

HEADERS += \
    Timesig.h \
    TimesigList.h \
    MeasureLineIterator.h \
    MeasureLine.h \
    vsqglobal.h \
    HandleType.h \
    Lyric.h \
    StringUtil.h \
    PhoneticSymbol.h \
    VibratoBP.h \
    VibratoBPList.h \
    Tempo.h \
    TextStream.h \
    ArticulationType.h \
    Handle.h \
    EventType.h \
    Event.h \
    EventWriteOption.h \
    Event.h \
    EventList.h \
    VoiceLanguageEnum.h \
    EventListIterator.h \
    BP.h \
    BPListSearchResult.h \
    BPList.h \
    BPListKeyClockIterator.h \
    MidiParameterType.h \
    ByteArrayOutputStream.h \
    MidiEvent.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

INCLUDEPATH += ../ ./

OTHER_FILES += \
    README.markdown \
    README.ja_JP.markdown

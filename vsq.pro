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
    Timesig.hpp \
    TimesigList.hpp \
    MeasureLineIterator.hpp \
    MeasureLine.hpp \
    vsqglobal.hpp \
    HandleType.hpp \
    Lyric.hpp \
    StringUtil.hpp \
    PhoneticSymbol.hpp \
    VibratoBP.hpp \
    VibratoBPList.hpp \
    Tempo.hpp \
    TextStream.hpp \
    ArticulationType.hpp \
    Handle.hpp \
    EventType.hpp \
    Event.hpp \
    EventWriteOption.hpp \
    Event.hpp \
    EventList.hpp \
    VoiceLanguageEnum.hpp \
    EventListIterator.hpp \
    BP.hpp \
    BPListSearchResult.hpp \
    BPList.hpp \
    BPListKeyClockIterator.hpp \
    MidiParameterType.hpp \
    ByteArrayOutputStream.hpp \
    MidiEvent.hpp \
    OutputStream.hpp \
    Track.hpp \
    DynamicsMode.hpp

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

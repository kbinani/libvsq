#-------------------------------------------------
#
# Project created by QtCreator 2012-02-13T03:14:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = vsq
TEMPLATE = lib
CONFIG += staticlib

SOURCES +=

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
    VoiceLanguageEnum.hpp \
    BP.hpp \
    BPListSearchResult.hpp \
    BPList.hpp \
    MidiParameterType.hpp \
    ByteArrayOutputStream.hpp \
    MidiEvent.hpp \
    OutputStream.hpp \
    DynamicsMode.hpp \
    PlayMode.hpp \
    Common.hpp \
    Master.hpp \
    MixerItem.hpp \
    Mixer.hpp \
    EventListIndexIteratorKind.hpp \
    EventListIndexIterator.hpp \
    TempoList.hpp \
    Track.hpp \
    NrpnEvent.hpp \
    CP932Converter.hpp \
    CP932ConverterData.hpp \
    BitConverter.hpp \
    Sequence.hpp

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

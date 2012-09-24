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
    VibratoBPTest.cpp \
    VibratoBPListTest.cpp \
    TempoTest.cpp \
    TextStreamTest.cpp \
    ArticulationTypeTest.cpp \
    HandleTest.cpp \
    ../*.cpp \
    EventTypeTest.cpp \
    EventTest.cpp \
    EventWriteOptionTest.cpp \
    EventTest.cpp \
    HandleTypeTest.cpp \
    EventListTest.cpp \
    VoiceLanguageEnumTest.cpp \
    BPTest.cpp \
    BPListSearchResultTest.cpp \
    BPListTest.cpp \
    MidiParameterTypeTest.cpp \
    ByteArrayOutputStreamTest.cpp \
    MidiEventTest.cpp \
    PlayModeTest.cpp \
    CommonTest.cpp \
    MasterTest.cpp \
    MixerItemTest.cpp \
    MixerTest.cpp \
    EventListIndexIteratorKindTest.cpp \
    EventListIndexIteratorTest.cpp \
    Event.ListTest.cpp \
    Event.ListIteratorTest.cpp \
    TrackTest.cpp \
    TempoListTest.cpp \
    NrpnEventTest.cpp \
    CP932ConverterTest.cpp \
    BitConverterTest.cpp \
    SequenceTest.cpp \
    VocaloidMidiEventListFactoryTest.cpp \
    VSQFileWriterTest.cpp \
    FileInputStreamTest.cpp \
    SMFReaderTest.cpp \
    VSQFileReaderTest.cpp \
    NoteNumberUtilTest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../vsq/
LIBS += -lcppunit

HEADERS += \
    Util.hpp \
    ../*.hpp

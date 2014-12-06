CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(libvsqtest)

SET(CMAKE_CXX_FLAGS "-std=c++11")

LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib/$(CONFIGURATION)"
                 "${PROJECT_SOURCE_DIR}/../lib"
                 /usr/local/lib)
INCLUDE_DIRECTORIES(ext/gtest/include
                    ext/gtest
                    /usr/local/include)
IF((NOT APPLE) AND (NOT WIN32))
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
ENDIF()

SET(COVERAGE false CACHE BOOL "Enable gcov")

IF(COVERAGE)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -coverage")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
ENDIF()

ADD_EXECUTABLE(libvsqtest
    ArticulationTypeTest.cpp
    BPListSearchResultTest.cpp
    BPListTest.cpp
    BPTest.cpp
    BitConverterTest.cpp
    ByteArrayOutputStreamTest.cpp
    CP932ConverterTest.cpp
    CommonTest.cpp
    Event.ListIteratorTest.cpp
    Event.ListTest.cpp
    EventListIndexIteratorKindTest.cpp
    EventListIndexIteratorTest.cpp
    EventTest.cpp
    EventTypeTest.cpp
    FileInputStreamTest.cpp
    FileOutputStreamTest.cpp
    HandleTest.cpp
    HandleTypeTest.cpp
    LyricTest.cpp
    MasterTest.cpp
    MeasureLineIteratorTest.cpp
    MidiEventTest.cpp
    MidiParameterTypeTest.cpp
    MixerItemTest.cpp
    MixerTest.cpp
    MusicXmlWriterTest.cpp
    NoteNumberUtilTest.cpp
    NrpnEventTest.cpp
    PhoneticSymbolDictionaryTest.cpp
    PhoneticSymbolTest.cpp
    PlayModeTest.cpp
    SMFReaderTest.cpp
    SequenceTest.cpp
    StreamWriterTest.cpp
    StringUtilTest.cpp
    TempoListTest.cpp
    TempoTest.cpp
    TextStreamTest.cpp
    TimesigListTest.cpp
    TimesigTest.cpp
    TrackTest.cpp
    VSQFileReaderTest.cpp
    VSQFileWriterTest.cpp
    VibratoBPListTest.cpp
    VibratoBPTest.cpp
    VocaloidMidiEventListFactoryTest.cpp
    VoiceLanguageTest.cpp
    Util.hpp
    ext/gtest/src/gtest-all.cc
    ext/gtest/src/gtest_main.cc)

TARGET_LINK_LIBRARIES(libvsqtest vsq)

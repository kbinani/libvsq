CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(libvsq)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
SET(CMAKE_CXX_FLAGS "-std=c++11")

ADD_LIBRARY(vsq STATIC
    ArticulationType.hpp
    BP.hpp
    BPList.hpp
    BPListSearchResult.hpp
    BitConverter.hpp
    ByteArrayOutputStream.hpp
    CP932Converter.hpp
    CP932ConverterData.hpp
    Common.hpp
    DynamicsMode.hpp
    Event.hpp
    EventListIndexIterator.hpp
    EventListIndexIteratorKind.hpp
    EventType.hpp
    EventWriteOption.hpp
    FileInputStream.hpp
    FileOutputStream.hpp
    Handle.hpp
    HandleType.hpp
    InputStream.hpp
    Lyric.hpp
    Master.hpp
    MeasureLine.hpp
    MeasureLineIterator.hpp
    MidiEvent.hpp
    MidiParameterType.hpp
    Mixer.hpp
    MixerItem.hpp
    MusicXmlWriter.hpp
    NoteNumberUtil.hpp
    NrpnEvent.hpp
    OutputStream.hpp
    PhoneticSymbol.hpp
    PhoneticSymbolDictionary.hpp
    PlayMode.hpp
    SMFReader.hpp
    Sequence.hpp
    StreamWriter.hpp
    StringUtil.hpp
    Tempo.hpp
    TempoList.hpp
    TextOutputStream.hpp
    TextStream.hpp
    Timesig.hpp
    TimesigList.hpp
    Track.hpp
    VSQFileReader.hpp
    VSQFileWriter.hpp
    VibratoBP.hpp
    VibratoBPList.hpp
    VocaloidMidiEventListFactory.hpp
    VoiceLanguageEnum.hpp
    vsqglobal.hpp
    BP.cpp)

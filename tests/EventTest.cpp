#include "Util.h"
#include "../Event.h"

using namespace std;
using namespace VSQ_NS;

class EventTest : public CppUnit::TestCase
{
public:
    Event getNoteEvent()
    {
        Event noteEvent( 0, EventType::NOTE );
        noteEvent.setLength( 2 );
        noteEvent.note = 6;
        noteEvent.dynamics = 21;
        noteEvent.pmBendDepth = 4;
        noteEvent.pmBendLength = 5;
        noteEvent.pmbPortamentoUse = 3;
        noteEvent.demDecGainRate = 7;
        noteEvent.demAccent = 8;
//        noteEvent.preUtterance = 9;
//        noteEvent.voiceOverlap = 10;
        noteEvent.vibratoDelay = 13;
        noteEvent.lyricHandle.index = 1;
        return noteEvent;
    }
    
    Event getSingerEvent()
    {
        Event singerEvent( 0, EventType::SINGER );
        singerEvent.singerHandle = Handle( HandleType::SINGER );
        singerEvent.singerHandle.index = 16;
        singerEvent.index = 16;
        return singerEvent;
    }
    
    Event getIconEvent()
    {
        Event iconEvent( 0, EventType::ICON );
        iconEvent.note = 19;
        iconEvent.index = 17;
        return iconEvent;
    }
    
    void testConstruct()
    {
        Event event;
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, event.clock );
        CPPUNIT_ASSERT_EQUAL( 0, event.id );
    }
    
    void testConstructWithLine()
    {
        Event event( "123=ID#0001" );
        CPPUNIT_ASSERT_EQUAL( (tick_t)123, event.clock );
    
        event = Event( "1230=EOS" );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1230, event.clock );
        CPPUNIT_ASSERT( event.isEOS() );
    }
    
    void testConstructWithClockAndId()
    {
        Event event( 1, EventType::NOTE );
        event.note = 60;
        event.index = 12;
    
        CPPUNIT_ASSERT_EQUAL( (tick_t)1, event.clock );
        CPPUNIT_ASSERT_EQUAL( 12, event.index );
        CPPUNIT_ASSERT_EQUAL( 60, event.note );
    }
    
    void testEquals()
    {
        //    fail();
    }
    
    void testWriteNoteWithOption()
    {
        Event event = getNoteEvent();
        event.clock = 20;
        event.index = 1;
        EventWriteOption optionAll;
        optionAll.length = true;
        optionAll.note = true;
        optionAll.dynamics = true;
        optionAll.pmBendDepth = true;
        optionAll.pmBendLength = true;
        optionAll.pmbPortamentoUse = true;
        optionAll.demDecGainRate = true;
        optionAll.demAccent = true;
        //TODO. optionAll.preUtterance = true;
        //TODO. optionAll.voiceOverlap = true;
    
        TextStream stream;
    
        // handleがどれもnilな音符イベント
        event.write( stream, optionAll );
        string expected =
                "[ID#0001]\n"
                "Type=Anote\n"
                "Length=2\n"
                "Note#=6\n"
                "Dynamics=21\n"
                "PMBendDepth=4\n"
                "PMBendLength=5\n"
                "PMbPortamentoUse=3\n"
                "DEMdecGainRate=7\n"
                "DEMaccent=8\n"
                "LyricHandle=h#0001\n";
            //TODO. "PreUtterance=9\n" ..
            //TODO. "VoiceOverlap=10\n" ..
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    
        // handleに全部値が入っている音符イベント
        // 現在、PreUtteranceとVoiceOverlapは扱わないようにしているので、
        // オプション全指定と、オプションが無い場合の動作が全くおなじになってしまっている。
        // ustEventをちゃんと処理するようになったら、TODOコメントのところを外すこと
        event.lyricHandle = Handle( HandleType::LYRIC );
        event.lyricHandle.setLyricAt( 0, Lyric( "わ", "w a" ) );
        event.lyricHandle.index = 11;
        event.vibratoHandle = Handle( HandleType::VIBRATO );
        event.vibratoHandle.index = 12;
        event.noteHeadHandle = Handle( HandleType::NOTE_HEAD );
        event.noteHeadHandle.index = 14;
        stream = TextStream();
        event.write( stream, optionAll );
        expected =
            "[ID#0001]\n"
            "Type=Anote\n"
            "Length=2\n"
            "Note#=6\n"
            "Dynamics=21\n"
            "PMBendDepth=4\n"
            "PMBendLength=5\n"
            "PMbPortamentoUse=3\n"
            "DEMdecGainRate=7\n"
            "DEMaccent=8\n"
            //TODO. "PreUtterance=9\n" ..
            //TODO. "VoiceOverlap=10\n" ..
            "LyricHandle=h#0011\n"
            "VibratoHandle=h#0012\n"
            "VibratoDelay=13\n"
            "NoteHeadHandle=h#0014\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    
        // オプションが無い場合
        stream = TextStream();
        event.write( stream );
        expected =
            "[ID#0001]\n"
            "Type=Anote\n"
            "Length=2\n"
            "Note#=6\n"
            "Dynamics=21\n"
            "PMBendDepth=4\n"
            "PMBendLength=5\n"
            "PMbPortamentoUse=3\n"
            "DEMdecGainRate=7\n"
            "DEMaccent=8\n"
            "LyricHandle=h#0011\n"
            "VibratoHandle=h#0012\n"
            "VibratoDelay=13\n"
            "NoteHeadHandle=h#0014\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    
        // オプションが空の場合
        stream = TextStream();
        EventWriteOption emptyOption;
        emptyOption.demAccent = false;
        emptyOption.demDecGainRate = false;
        emptyOption.dynamics = false;
        emptyOption.length = false;
        emptyOption.note = false;
        emptyOption.pmBendDepth = false;
        emptyOption.pmBendLength = false;
        emptyOption.pmbPortamentoUse = false;
        emptyOption.preUtterance = false;
        emptyOption.voiceOverlap = false;
        event.write( stream, emptyOption );
        expected =
            "[ID#0001]\n"
            "Type=Anote\n"
            "LyricHandle=h#0011\n"
            "VibratoHandle=h#0012\n"
            "VibratoDelay=13\n"
            "NoteHeadHandle=h#0014\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }
    
    void testWriteSinger()
    {
        Event event = getSingerEvent();
        event.clock = 1;
        event.index = 15;
        TextStream stream;
        event.write( stream );
        string expected =
            "[ID#0015]\n"
            "Type=Singer\n"
            "IconHandle=h#0016\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }
    
    void testWriteIcon()
    {
        Event event = getIconEvent();
        event.iconDynamicsHandle = Handle( HandleType::DYNAMICS );
        event.iconDynamicsHandle.index = 18;
        event.clock = 2;
        event.index = 17;
        TextStream stream;
        event.write( stream );
        string expected =
            "[ID#0017]\n"
            "Type=Aicon\n"
            "IconHandle=h#0018\n"
            "Note#=19\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }
    
    void testCompareTo()
    {
        Event singerEvent( 1920, EventType::SINGER );
        Event noteEvent( 1920, EventType::NOTE );
        CPPUNIT_ASSERT_EQUAL( 0, singerEvent.compareTo( singerEvent ) );
        CPPUNIT_ASSERT( 0 > singerEvent.compareTo( noteEvent ) );
        CPPUNIT_ASSERT( 0 < noteEvent.compareTo( singerEvent ) );
    
        singerEvent.clock = 2000;
        noteEvent.clock = 1920;
        CPPUNIT_ASSERT( 0 < singerEvent.compareTo( noteEvent ) );
    
        singerEvent.clock = 2000;
        noteEvent.clock = 2001;
        CPPUNIT_ASSERT_EQUAL( 1, noteEvent.compareTo( singerEvent ) );
    }
    
    void testCompare()
    {
        Event singerEvent = getSingerEvent();
        singerEvent.clock = 1920;
        Event noteEvent = getNoteEvent();
        noteEvent.clock = 1920;
        CPPUNIT_ASSERT( false == Event::compare( singerEvent, singerEvent ) );
        CPPUNIT_ASSERT( Event::compare( singerEvent, noteEvent ) );
        CPPUNIT_ASSERT( false == Event::compare( noteEvent, singerEvent ) );
    
        singerEvent.clock = 2000;
        noteEvent.clock = 1920;
        CPPUNIT_ASSERT( false == Event::compare( singerEvent, noteEvent ) );
    
        singerEvent.clock = 2000;
        noteEvent.clock = 2001;
        CPPUNIT_ASSERT( false == Event::compare( noteEvent, singerEvent ) );
    }

    CPPUNIT_TEST_SUITE( EventTest );
    CPPUNIT_TEST( testConstruct );
    CPPUNIT_TEST( testConstructWithLine );
    CPPUNIT_TEST( testConstructWithClockAndId );
    CPPUNIT_TEST( testEquals );
    CPPUNIT_TEST( testWriteNoteWithOption );
    CPPUNIT_TEST( testWriteSinger );
    CPPUNIT_TEST( testWriteIcon );
    CPPUNIT_TEST( testCompareTo );
    CPPUNIT_TEST( testCompare );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( EventTest );

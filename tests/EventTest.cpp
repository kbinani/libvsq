#include "Util.hpp"
#include "../Event.hpp"

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
    
    Event getSingerEvent(){
        Event singerEvent( 0, EventType::SINGER );
        singerEvent.singerHandle = Handle( HandleType::SINGER );
        singerEvent.singerHandle.index = 16;
        return singerEvent;
    }
    
    Event getIconEvent(){
        Event iconEvent( 0, EventType::ICON );
        iconEvent.note = 19;
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
    
    void testConstructWithClockAndId(){
        Event event( 1, EventType::NOTE );
    
        CPPUNIT_ASSERT_EQUAL( (tick_t)1, event.clock );
        CPPUNIT_ASSERT_EQUAL( EventType::NOTE, event.type );
    }
    
    void testEquals()
    {
        //    fail();
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

    void testClone(){
        Event event = getSingerEvent();
        event.clock = 40;
        event.id = 4;
        event.singerHandle = Handle( HandleType::SINGER );
        event.singerHandle.index = 12;
        Event copy = event.clone();
        CPPUNIT_ASSERT_EQUAL( (tick_t)40, copy.clock );
        CPPUNIT_ASSERT_EQUAL( 4, copy.id );
        CPPUNIT_ASSERT_EQUAL( 12, copy.singerHandle.index );

        Event id( 0, EventType::NOTE );
        id.note = 6;
        id.dynamics = 7;
        id.pmBendDepth = 8;
        id.pmBendLength = 9;
        id.pmbPortamentoUse = 10;
        id.demDecGainRate = 11;
        id.demAccent = 12;
        id.vibratoDelay = 13;
        id.pMeanOnsetFirstNote = 14;
        id.vMeanNoteTransition = 15;
        id.d4mean = 16;
        id.pMeanEndingNote = 17;
        //assert_nil( id.singerHandle );
        //assert_not_nil( id.lyricHandle );
        //assert_nil( id.vibratoHandle );
        //assert_nil( id.noteHeadHandle );
        //assert_nil( id.iconDynamicsHandle );

        copy = id.clone();
        CPPUNIT_ASSERT_EQUAL( EventType::NOTE, copy.type );
        CPPUNIT_ASSERT_EQUAL( 6, copy.note );
        CPPUNIT_ASSERT_EQUAL( 7, copy.dynamics );
        CPPUNIT_ASSERT_EQUAL( 8, copy.pmBendDepth );
        CPPUNIT_ASSERT_EQUAL( 9, copy.pmBendLength );
        CPPUNIT_ASSERT_EQUAL( 10, copy.pmbPortamentoUse );
        CPPUNIT_ASSERT_EQUAL( 11, copy.demDecGainRate );
        CPPUNIT_ASSERT_EQUAL( 12, copy.demAccent );
        CPPUNIT_ASSERT_EQUAL( 13, copy.vibratoDelay );
        CPPUNIT_ASSERT_EQUAL( 14, copy.pMeanOnsetFirstNote );
        CPPUNIT_ASSERT_EQUAL( 15, copy.vMeanNoteTransition );
        CPPUNIT_ASSERT_EQUAL( 16, copy.d4mean );
        CPPUNIT_ASSERT_EQUAL( 17, copy.pMeanEndingNote );

        Handle iconHandle( HandleType::SINGER );
        iconHandle.caption = "foo";
        id.singerHandle = iconHandle;
        Handle lyricHandle( HandleType::LYRIC );
        lyricHandle.index = 102;
        id.lyricHandle = lyricHandle;
        Handle vibratoHandle( HandleType::VIBRATO );
        vibratoHandle.iconId = "aho";
        id.vibratoHandle = vibratoHandle;
        Handle noteHeadHandle( HandleType::NOTE_HEAD );
        noteHeadHandle.ids = "baka";
        id.noteHeadHandle = noteHeadHandle;
        Handle iconDynamicsHandle( HandleType::DYNAMICS );
        iconDynamicsHandle.startDyn = 183635;
        id.iconDynamicsHandle = iconDynamicsHandle;

        copy = id.clone();
        CPPUNIT_ASSERT_EQUAL( string( "foo" ), copy.singerHandle.caption );
        CPPUNIT_ASSERT_EQUAL( 102, copy.lyricHandle.index );
        CPPUNIT_ASSERT_EQUAL( string( "aho" ), copy.vibratoHandle.iconId );
        CPPUNIT_ASSERT_EQUAL( string( "baka" ), copy.noteHeadHandle.ids );
        CPPUNIT_ASSERT_EQUAL( 183635, copy.iconDynamicsHandle.startDyn );
    }

    CPPUNIT_TEST_SUITE( EventTest );
    CPPUNIT_TEST( testConstruct );
    CPPUNIT_TEST( testConstructWithLine );
    CPPUNIT_TEST( testConstructWithClockAndId );
    CPPUNIT_TEST( testEquals );
    CPPUNIT_TEST( testCompareTo );
    CPPUNIT_TEST( testCompare );
    CPPUNIT_TEST( testClone );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( EventTest );

#include "Util.hpp"
#include "../EventList.hpp"
#include "../EventListIterator.hpp"

using namespace std;
using namespace VSQ_NS;

class EventListStub : public EventList
{
public:
    void pushBackWithoutSort( const Event &item, int id )
    {
        _events.push_back( item );
        _ids.push_back( id );
    }
};

class EventListTest : public CppUnit::TestCase
{
public:
    void testConstruct()
    {
        EventList list;
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
    }
    
    void testFindIndexFromId(){
        EventList list;
        Event a( 1, EventType::NOTE );
        Event b( 0, EventType::NOTE );
        list.add( a, 0 );
        list.add( b, 1 );
    
        // aのほうがclockが大きいので、後ろに並び替えられる
        CPPUNIT_ASSERT_EQUAL( 0, list.findIndexFromId( 1 ) );
        CPPUNIT_ASSERT_EQUAL( 1, list.findIndexFromId( 0 ) );
    }
    
    void testFindFromId()
    {
        EventList list;
        Event a( 0, EventType::NOTE );
        Event b( 0, EventType::SINGER );
        list.add( a, 0 );
        list.add( b, 1 );
    
        CPPUNIT_ASSERT_EQUAL( EventType::NOTE, list.findFromId( 0 )->type );
        CPPUNIT_ASSERT_EQUAL( EventType::SINGER, list.findFromId( 1 )->type );
        CPPUNIT_ASSERT( NULL == list.findFromId( 1000 ) );
    }
    
    void testSetForId()
    {
        EventList listA;
        Event event( 0, EventType::NOTE );
        event.note = 60;
        event.id = 10;
    
        Event replace( 0, EventType::NOTE );
        replace.note = 90;
        replace.id = 100;
    
        listA.add( event, 10 );
        listA.setForId( 10, replace );
    
        CPPUNIT_ASSERT_EQUAL( 10, listA.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( 90, listA.get( 0 ).note );
    
        // 無効なinternalIdを渡すので、setが行われない場合
        EventList listB;
        listB.add( event, 10 );
        listB.setForId( 9999, replace );
        CPPUNIT_ASSERT_EQUAL( 60, listB.get( 0 ).note );
    }
    
    void testSort()
    {
        EventListStub list;
        Event b( 480, EventType::NOTE );
        Event a( 0, EventType::NOTE );
        b.id = 14;
        a.id = 20;
        list.pushBackWithoutSort( b, 14 );
        list.pushBackWithoutSort( a, 20 );
    
        list.sort();
    
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, list.get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, list.get( 1 ).clock );
        CPPUNIT_ASSERT_EQUAL( 0, list.findIndexFromId( 20 ) );
        CPPUNIT_ASSERT_EQUAL( 1, list.findIndexFromId( 14 ) );
    }
    
    void testClear()
    {
        EventList list;
        Event b( 480, EventType::NOTE );
        Event a( 0, EventType::NOTE );
        list.add( b, 14 );
        list.add( a, 20 );
    
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
        CPPUNIT_ASSERT_EQUAL( 20, list.get( 0 ).id );
    
        list.clear();
    
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
    }
    
    void testAddWithoutInternalId()
    {
        EventList list;
        Event a( 1, EventType::NOTE );
        Event b( 0, EventType::NOTE );
        int idOfA = list.add( a );
        int idOfB = list.add( b );
    
        // bよりaのほうがclockが大きいので、並べ替えが起きるはず
        CPPUNIT_ASSERT_EQUAL( idOfB, list.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( idOfA, list.get( 1 ).id );
        CPPUNIT_ASSERT( idOfA != idOfB );
    }
    
    void testAddWithInternalId()
    {
        EventList list;
        Event a( 0, EventType::NOTE );
        Event b( 0, EventType::NOTE );
        int idOfA = list.add( a, 100 );
        int idOfB = list.add( b, 2 );
        CPPUNIT_ASSERT_EQUAL( 100, idOfA );
        CPPUNIT_ASSERT_EQUAL( 2, idOfB );
        CPPUNIT_ASSERT_EQUAL( 100, list.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 ).id );
    }
    
    void testRemoveAt()
    {
        EventList list;
        Event a( 0, EventType::NOTE );
        Event b( 0, EventType::NOTE );
        list.add( a, 100 );
        list.add( b, 2 );
        CPPUNIT_ASSERT_EQUAL( 100, list.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 ).id );
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
    
        list.removeAt( 0 );
    
        CPPUNIT_ASSERT_EQUAL( 1, list.size() );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 0 ).id );
    }
    
    void testSize()
    {
        EventList list;
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
        Event event( 0, EventType::NOTE );
        list.add( event );
        CPPUNIT_ASSERT_EQUAL( 1, list.size() );
    }
    
    void testGetAndSetElement()
    {
        EventList list;
        Event a( 0, EventType::NOTE );
        Event b( 0, EventType::NOTE );
        list.add( a, 100 );
        list.add( b, 2 );
        CPPUNIT_ASSERT_EQUAL( 100, list.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 ).id );
    
        Event c( 480, EventType::NOTE );
        c.id = 99;
        list.set( 1, c );
    
        CPPUNIT_ASSERT_EQUAL( 100, list.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 ).id );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, list.get( 1 ).clock );
    }
    
    void testIterator()
    {
        EventList list;
        EventListIterator iterator = list.iterator();
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        Event singerEvent( 0, EventType::SINGER );
        singerEvent.singerHandle = Handle( HandleType::SINGER );
        list.add( singerEvent, 1 );
    
        Event crescendoEvent( 240, EventType::ICON );
        crescendoEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS );
        crescendoEvent.iconDynamicsHandle.iconId = "$05020001";
        list.add( crescendoEvent, 2 );
    
        iterator = list.iterator();
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, iterator.next()->clock );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, iterator.next()->clock );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testWrite()
    {
        EventList list;
    
        Event singerEvent( 0, EventType::SINGER );
        singerEvent.singerHandle = Handle( HandleType::SINGER );
        list.add( singerEvent, 1 );
    
        Event crescendoEvent( 240, EventType::ICON );
        crescendoEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS );
        crescendoEvent.iconDynamicsHandle.iconId = "$05020001";
        list.add( crescendoEvent, 2 );
    
        Event dynaffEvent( 480, EventType::ICON );
        dynaffEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS );
        dynaffEvent.iconDynamicsHandle.iconId = "$05010001";
        list.add( dynaffEvent, 3 );
    
        Event decrescendoEvent( 720, EventType::ICON );
        decrescendoEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS );
        decrescendoEvent.iconDynamicsHandle.iconId = "$05030001";
        list.add( decrescendoEvent, 4 );
    
        Event singerEvent2( 1920, EventType::SINGER );
        singerEvent2.singerHandle = Handle( HandleType::SINGER );
        list.add( singerEvent2, 5 );
    
        Event noteEvent( 1920, EventType::NOTE );
        noteEvent.setLength( 480 );
        noteEvent.lyricHandle = Handle( HandleType::LYRIC );
        noteEvent.lyricHandle.setLyricAt( 0, Lyric( "ら", "4 a" ) );
        noteEvent.noteHeadHandle = Handle( HandleType::NOTE_HEAD );
        noteEvent.vibratoHandle = Handle( HandleType::VIBRATO );
        list.add( noteEvent, 6 );
    
        TextStream stream;
        vector<Handle> handles = list.write( stream, 2400 );
        string expected =
            "[EventList]\n"
            "0=ID#0000\n"
            "240=ID#0001\n"
            "480=ID#0002\n"
            "720=ID#0003\n"
            "1920=ID#0004,ID#0005\n"
            "2400=EOS\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    
        CPPUNIT_ASSERT_EQUAL( 8, (int)handles.size() );
        CPPUNIT_ASSERT_EQUAL( HandleType::SINGER, handles[0].getHandleType() );
        CPPUNIT_ASSERT_EQUAL( HandleType::DYNAMICS, handles[1].getHandleType() );
        CPPUNIT_ASSERT_EQUAL( HandleType::DYNAMICS, handles[2].getHandleType() );
        CPPUNIT_ASSERT_EQUAL( HandleType::DYNAMICS, handles[3].getHandleType() );
        CPPUNIT_ASSERT_EQUAL( HandleType::SINGER, handles[4].getHandleType() );
        CPPUNIT_ASSERT_EQUAL( HandleType::LYRIC, handles[5].getHandleType() );
        CPPUNIT_ASSERT_EQUAL( HandleType::VIBRATO, handles[6].getHandleType() );
        CPPUNIT_ASSERT_EQUAL( HandleType::NOTE_HEAD, handles[7].getHandleType() );
    }

    CPPUNIT_TEST_SUITE( EventListTest );
    CPPUNIT_TEST( testConstruct );
    CPPUNIT_TEST( testFindIndexFromId );
    CPPUNIT_TEST( testFindFromId );
    CPPUNIT_TEST( testSetForId );
    CPPUNIT_TEST( testSort );
    CPPUNIT_TEST( testClear );
    CPPUNIT_TEST( testIterator );
    CPPUNIT_TEST( testAddWithoutInternalId );
    CPPUNIT_TEST( testAddWithInternalId );
    CPPUNIT_TEST( testRemoveAt );
    CPPUNIT_TEST( testSize );
    CPPUNIT_TEST( testGetAndSetElement );
    CPPUNIT_TEST( testIterator );
    CPPUNIT_TEST( testWrite );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( EventListTest );

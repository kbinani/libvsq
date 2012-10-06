#include "Util.hpp"
#include "../Event.hpp"

using namespace std;
using namespace VSQ_NS;

class EventListStub : public Event::List
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
        Event::List list;
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
    }
    
    void testFindIndexFromId(){
        Event::List list;
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
        Event::List list;
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
        Event::List listA;
        Event event( 0, EventType::NOTE );
        event.note = 60;
        event.id = 10;
    
        Event replace( 0, EventType::NOTE );
        replace.note = 90;
        replace.id = 100;
    
        listA.add( event, 10 );
        listA.setForId( 10, replace );
    
        CPPUNIT_ASSERT_EQUAL( 10, listA.get( 0 )->id );
        CPPUNIT_ASSERT_EQUAL( 90, listA.get( 0 )->note );
    
        // 無効なinternalIdを渡すので、setが行われない場合
        Event::List listB;
        listB.add( event, 10 );
        listB.setForId( 9999, replace );
        CPPUNIT_ASSERT_EQUAL( 60, listB.get( 0 )->note );
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
    
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, list.get( 0 )->clock );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, list.get( 1 )->clock );
        CPPUNIT_ASSERT_EQUAL( 0, list.findIndexFromId( 20 ) );
        CPPUNIT_ASSERT_EQUAL( 1, list.findIndexFromId( 14 ) );
    }
    
    void testClear()
    {
        Event::List list;
        Event b( 480, EventType::NOTE );
        Event a( 0, EventType::NOTE );
        list.add( b, 14 );
        list.add( a, 20 );
    
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
        CPPUNIT_ASSERT_EQUAL( 20, list.get( 0 )->id );
    
        list.clear();
    
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
    }
    
    void testAddWithoutInternalId()
    {
        Event::List list;
        Event a( 1, EventType::NOTE );
        Event b( 0, EventType::NOTE );
        int idOfA = list.add( a );
        int idOfB = list.add( b );
    
        // bよりaのほうがclockが大きいので、並べ替えが起きるはず
        CPPUNIT_ASSERT_EQUAL( idOfB, list.get( 0 )->id );
        CPPUNIT_ASSERT_EQUAL( idOfA, list.get( 1 )->id );
        CPPUNIT_ASSERT( idOfA != idOfB );
    }
    
    void testAddWithInternalId()
    {
        Event::List list;
        Event a( 0, EventType::NOTE );
        Event b( 0, EventType::NOTE );
        int idOfA = list.add( a, 100 );
        int idOfB = list.add( b, 2 );
        CPPUNIT_ASSERT_EQUAL( 100, idOfA );
        CPPUNIT_ASSERT_EQUAL( 2, idOfB );
        CPPUNIT_ASSERT_EQUAL( 100, list.get( 0 )->id );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 )->id );
    }
    
    void testRemoveAt()
    {
        Event::List list;
        Event a( 0, EventType::NOTE );
        Event b( 0, EventType::NOTE );
        list.add( a, 100 );
        list.add( b, 2 );
        CPPUNIT_ASSERT_EQUAL( 100, list.get( 0 )->id );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 )->id );
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
    
        list.removeAt( 0 );
    
        CPPUNIT_ASSERT_EQUAL( 1, list.size() );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 0 )->id );
    }
    
    void testSize()
    {
        Event::List list;
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
        Event event( 0, EventType::NOTE );
        list.add( event );
        CPPUNIT_ASSERT_EQUAL( 1, list.size() );
    }
    
    void testGetAndSetElement()
    {
        Event::List list;
        Event a( 0, EventType::NOTE );
        Event b( 0, EventType::NOTE );
        list.add( a, 100 );
        list.add( b, 2 );
        CPPUNIT_ASSERT_EQUAL( 100, list.get( 0 )->id );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 )->id );
    
        Event c( 480, EventType::NOTE );
        c.id = 99;
        list.set( 1, c );
    
        CPPUNIT_ASSERT_EQUAL( 100, list.get( 0 )->id );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 )->id );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, list.get( 1 )->clock );
    }
    
    void testIterator()
    {
        Event::List list;
        Event::ListIterator iterator = list.iterator();
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
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( EventListTest );

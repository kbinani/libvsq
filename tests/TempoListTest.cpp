#include "Util.hpp"
#include "../TempoList.hpp"

using namespace std;
using namespace VSQ_NS;

class TempoListTest : public CppUnit::TestCase{
public:
    /**
     * push, get, set, size のテスト
     */
    void test(){
        TempoList list;
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
        list.push( Tempo( 0, 500000 ) );
        list.push( Tempo( 480, 525000 ) );
        list.updateTempoInfo();

        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
        CPPUNIT_ASSERT_EQUAL( 0, list.get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( 500000, list.get( 0 ).tempo );
        CPPUNIT_ASSERT_EQUAL( 0.0, list.get( 0 ).getTime() );
        CPPUNIT_ASSERT_EQUAL( 480, list.get( 1 ).clock );
        CPPUNIT_ASSERT_EQUAL( 525000, list.get( 1 ).tempo );
        CPPUNIT_ASSERT_EQUAL( 0.5, list.get( 1 ).getTime() );
    }

    void testIterator(){
        TempoList list;
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
        list.push( Tempo( 0, 500000 ) );
        list.push( Tempo( 480, 525000 ) );
        list.updateTempoInfo();

        TempoList::Iterator i = list.iterator();
        CPPUNIT_ASSERT( i.hasNext() );
        Tempo item = i.next();
        CPPUNIT_ASSERT_EQUAL( 0, item.clock );
        CPPUNIT_ASSERT_EQUAL( 500000, item.tempo );
        CPPUNIT_ASSERT_EQUAL( 0.0, item.getTime() );
        CPPUNIT_ASSERT( i.hasNext() );
        item = i.next();
        CPPUNIT_ASSERT_EQUAL( 480, item.clock );
        CPPUNIT_ASSERT_EQUAL( 525000, item.tempo );
        CPPUNIT_ASSERT_EQUAL( 0.5, item.getTime() );
        CPPUNIT_ASSERT( i.hasNext() == false );
    }

    void testSort(){
        TempoList list;
        list.push( Tempo( 480, 525000 ) );
        list.push( Tempo( 0, 500000 ) );

        list.sort();

        CPPUNIT_ASSERT_EQUAL( 0, list.get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( 500000, list.get( 0 ).tempo );
        CPPUNIT_ASSERT_EQUAL( 480, list.get( 1 ).clock );
        CPPUNIT_ASSERT_EQUAL( 525000, list.get( 1 ).tempo );
    }

    void testClockFromSec(){
        TempoList list;
        list.push( Tempo( 480, 480000 ) );
        list.push( Tempo( 0, 500000 ) );
        list.updateTempoInfo();

        CPPUNIT_ASSERT_EQUAL( 0.0, list.getClockFromSec( 0.0 ) );
        CPPUNIT_ASSERT_EQUAL( 480.0, list.getClockFromSec( 0.5 ) );
        CPPUNIT_ASSERT_EQUAL( 384.0, list.getClockFromSec( 0.4 ) );
        CPPUNIT_ASSERT_EQUAL( 680.0, list.getClockFromSec( 0.7 ) );
    }

    void testUpdateTempoInfo(){
        TempoList list;
        list.updateTempoInfo();
        CPPUNIT_ASSERT_EQUAL( 1, list.size() );
        CPPUNIT_ASSERT_EQUAL( 0, list.get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( 500000, list.get( 0 ).tempo );
        CPPUNIT_ASSERT_EQUAL( 0.0, list.get( 0 ).getTime() );

        list = TempoList();
        list.push( Tempo( 480, 525000 ) );
        list.push( Tempo( 0, 500000 ) );
        list.updateTempoInfo();
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
        CPPUNIT_ASSERT_EQUAL( 0, list.get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( 500000, list.get( 0 ).tempo );
        CPPUNIT_ASSERT_EQUAL( 0.0, list.get( 0 ).getTime() );
        CPPUNIT_ASSERT_EQUAL( 480, list.get( 1 ).clock );
        CPPUNIT_ASSERT_EQUAL( 525000, list.get( 1 ).tempo );
        CPPUNIT_ASSERT_EQUAL( 0.5, list.get( 1 ).getTime() );
    }

    void testGetSecFromClock(){
        TempoList list;
        list.push( Tempo( 480, 480000 ) );
        list.push( Tempo( 0, 500000 ) );
        list.updateTempoInfo();

        CPPUNIT_ASSERT_EQUAL( 0.0, list.getSecFromClock( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 0.5, list.getSecFromClock( 480 ) );
        CPPUNIT_ASSERT_EQUAL( 0.4, list.getSecFromClock( 384 ) );
        CPPUNIT_ASSERT_EQUAL( 0.7, list.getSecFromClock( 680 ) );
    }

    void testGetTempoAt(){
        TempoList list;
        list.push( Tempo( 480, 480000 ) );
        list.push( Tempo( 0, 500000 ) );
        list.updateTempoInfo();

        CPPUNIT_ASSERT_EQUAL( 500000, list.getTempoAt( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 500000, list.getTempoAt( 479 ) );
        CPPUNIT_ASSERT_EQUAL( 480000, list.getTempoAt( 480 ) );
    }

    CPPUNIT_TEST_SUITE( TempoListTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST( testIterator );
    CPPUNIT_TEST( testSort );
    CPPUNIT_TEST( testClockFromSec );
    CPPUNIT_TEST( testUpdateTempoInfo );
    CPPUNIT_TEST( testGetSecFromClock );
    CPPUNIT_TEST( testGetTempoAt );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( TempoListTest );

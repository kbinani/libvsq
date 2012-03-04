#include "Util.h"
#include "../BPListKeyClockIterator.h"

using namespace std;
using namespace VSQ_NS;

class BPListKeyClockIteratorTest : public CppUnit::TestCase
{
public:
    void test()
    {
        BPList list( "foo", 63, -10, 1000 );
        BPListKeyClockIterator iterator = BPListKeyClockIterator( &list );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
        list.add( 480, 1 );
        list.add( 1920, 2 );
    
        iterator = BPListKeyClockIterator( &list );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, iterator.next() );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1920, iterator.next() );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        iterator = BPListKeyClockIterator( &list );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, iterator.next() );
        iterator.remove();
        CPPUNIT_ASSERT_EQUAL( (tick_t)1920, iterator.next() );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }

    CPPUNIT_TEST_SUITE( BPListKeyClockIteratorTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( BPListKeyClockIteratorTest );

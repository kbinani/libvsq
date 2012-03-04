#include "Util.h"
#include "../BPListSearchResult.h"

using namespace std;
using namespace VSQ_NS;

class BPListSearchResultTest : public CppUnit::TestCase
{
public:
    void testConstruct()
    {
        BPListSearchResult result;
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, result.clock );
        CPPUNIT_ASSERT_EQUAL( 0, result.index );
    }

    CPPUNIT_TEST_SUITE( BPListSearchResultTest );
    CPPUNIT_TEST( testConstruct );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( BPListSearchResultTest );

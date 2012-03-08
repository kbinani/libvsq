#include "Util.hpp"
#include "../BP.hpp"

using namespace std;
using namespace VSQ_NS;

class BPTest : public CppUnit::TestCase
{
public:
    void testConstruct()
    {
        int value = 64;
        int id = 1;
        BP point( value, id );
        CPPUNIT_ASSERT_EQUAL( 64, point.value );
        CPPUNIT_ASSERT_EQUAL( 1, point.id );
    }

    CPPUNIT_TEST_SUITE( BPTest );
    CPPUNIT_TEST( testConstruct );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( BPTest );

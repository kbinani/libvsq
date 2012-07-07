#include "Util.hpp"
#include "../PlayMode.hpp"

using namespace std;
using namespace VSQ_NS;

class PlayModeTest : public CppUnit::TestCase
{
public:
    void test(){
        CPPUNIT_ASSERT_EQUAL( -1, (int)PlayMode::OFF );
        CPPUNIT_ASSERT_EQUAL( 0, (int)PlayMode::PLAY_AFTER_SYNTH );
        CPPUNIT_ASSERT_EQUAL( 1, (int)PlayMode::PLAY_WITH_SYNTH );
    }

    CPPUNIT_TEST_SUITE( PlayModeTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( PlayModeTest );

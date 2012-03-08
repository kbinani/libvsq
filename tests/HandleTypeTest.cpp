#include "Util.hpp"
#include "../HandleType.hpp"

class HandleTypeTest : public CppUnit::TestCase
{
public:
    void test()
    {
        CPPUNIT_ASSERT_EQUAL( 0, (int)VSQ_NS::HandleType::UNKNOWN );
        CPPUNIT_ASSERT_EQUAL( 1, (int)VSQ_NS::HandleType::LYRIC );
        CPPUNIT_ASSERT_EQUAL( 2, (int)VSQ_NS::HandleType::VIBRATO );
        CPPUNIT_ASSERT_EQUAL( 3, (int)VSQ_NS::HandleType::SINGER );
        CPPUNIT_ASSERT_EQUAL( 4, (int)VSQ_NS::HandleType::NOTE_HEAD );
        CPPUNIT_ASSERT_EQUAL( 5, (int)VSQ_NS::HandleType::DYNAMICS );
    }

    CPPUNIT_TEST_SUITE( HandleTypeTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( HandleTypeTest );

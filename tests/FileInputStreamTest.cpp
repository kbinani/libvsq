#include "Util.hpp"
#include "../FileInputStream.hpp"

using namespace std;
using namespace VSQ_NS;

class FileInputStreamTest : public CppUnit::TestCase{
public:
    void test(){
        FileInputStream stream( TestUtil::getFixtureRootPath() + "/FileInputStreamTest/fixture/data.bin" );
        CPPUNIT_ASSERT_EQUAL( (int64_t)0, stream.getPointer() );
        CPPUNIT_ASSERT_EQUAL( 0x00, stream.read() );
        CPPUNIT_ASSERT_EQUAL( (int64_t)1, stream.getPointer() );
        stream.seek( 0x10 );
        CPPUNIT_ASSERT_EQUAL( 0x10, stream.read() );
        CPPUNIT_ASSERT_EQUAL( (int64_t)0x11, stream.getPointer() );
        stream.close();
    }

    CPPUNIT_TEST_SUITE( FileInputStreamTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( FileInputStreamTest );

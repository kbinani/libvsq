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
        stream.seek( 0x05 );
        CPPUNIT_ASSERT_EQUAL( 0x05, stream.read() );
        CPPUNIT_ASSERT_EQUAL( (int64_t)0x06, stream.getPointer() );

        stream.seek( 0x10 );
        char buffer[10] = { 0 };
        stream.read( buffer, 5, 5 );
        CPPUNIT_ASSERT_EQUAL( (char)0, buffer[0] );
        CPPUNIT_ASSERT_EQUAL( (char)0, buffer[1] );
        CPPUNIT_ASSERT_EQUAL( (char)0, buffer[2] );
        CPPUNIT_ASSERT_EQUAL( (char)0, buffer[3] );
        CPPUNIT_ASSERT_EQUAL( (char)0, buffer[4] );
        CPPUNIT_ASSERT_EQUAL( (char)0x10, buffer[5] );
        CPPUNIT_ASSERT_EQUAL( (char)0x11, buffer[6] );
        CPPUNIT_ASSERT_EQUAL( (char)0x12, buffer[7] );
        CPPUNIT_ASSERT_EQUAL( (char)0x13, buffer[8] );
        CPPUNIT_ASSERT_EQUAL( (char)0x14, buffer[9] );

        stream.seek( 0x2F );
        int readCount = stream.read( buffer, 0, 2 );
        CPPUNIT_ASSERT_EQUAL( (char)0x2F, buffer[0] );
        CPPUNIT_ASSERT_EQUAL( 1, readCount );

        stream.seek( 0x2F );
        CPPUNIT_ASSERT_EQUAL( 0x2F, stream.read() );
        CPPUNIT_ASSERT( stream.read() < 0 );

        stream.close();
    }

    CPPUNIT_TEST_SUITE( FileInputStreamTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( FileInputStreamTest );

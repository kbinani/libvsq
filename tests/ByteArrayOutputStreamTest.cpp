#include "Util.h"
#include "../ByteArrayOutputStream.h"
#include <sstream>

using namespace std;
using namespace VSQ_NS;

class ByteArrayOutputStreamTest : public CppUnit::TestCase
{
public:
    void test()
    {
        ByteArrayOutputStream stream;
        CPPUNIT_ASSERT_EQUAL( 0, stream.getPointer() );
        stream.write( 0 );
        stream.write( 1 );
        CPPUNIT_ASSERT_EQUAL( 2, stream.getPointer() );
        char bufferA[] = { 0, 0, 127, 0, 0 };
        stream.write( bufferA, 2, 1 );
        char bufferB[] = { 128, 255 };
        stream.write( bufferB, 0, 2 );
        CPPUNIT_ASSERT_EQUAL( 5, stream.getPointer() );
        string actual = stream.toString();
        ostringstream expected;
        expected << (char)0 << (char)1 << (char)127 << (char)128 << (char)255;
        CPPUNIT_ASSERT_EQUAL( expected.str(), actual );
    
        stream.seek( 0 );
        stream.write( 64 );
        CPPUNIT_ASSERT_EQUAL( 1, stream.getPointer() );
        expected.str( "" );
        expected.setstate( ostringstream::goodbit );
        expected << (char)64 << (char)1 << (char)127 << (char)128 << (char)255;
        CPPUNIT_ASSERT_EQUAL( expected.str(), stream.toString() );

        stream.close();
    }

    CPPUNIT_TEST_SUITE( ByteArrayOutputStreamTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( ByteArrayOutputStreamTest );

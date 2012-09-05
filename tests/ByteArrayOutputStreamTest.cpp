#include "Util.hpp"
#include "../ByteArrayOutputStream.hpp"
#include "../StringUtil.hpp"
#include <sstream>

using namespace std;
using namespace VSQ_NS;

class ByteArrayOutputStreamStub : public ByteArrayOutputStream{
public:
    static int getUnitBufferLength(){
        return UNIT_BUFFER_LENGTH;
    }
};

class ByteArrayOutputStreamTest : public CppUnit::TestCase{
public:
    void test(){
        ByteArrayOutputStream stream;
        CPPUNIT_ASSERT_EQUAL( (int64_t)0, stream.getPointer() );
        stream.write( 0 );
        stream.write( 1 );
        CPPUNIT_ASSERT_EQUAL( (int64_t)2, stream.getPointer() );
        char bufferA[] = { 0, 0, 127, 0, 0 };
        stream.write( bufferA, 2, 1 );
        char bufferB[] = { 128, 255 };
        stream.write( bufferB, 0, 2 );
        CPPUNIT_ASSERT_EQUAL( (int64_t)5, stream.getPointer() );
        string actual = stream.toString();
        ostringstream expected;
        expected << (char)0 << (char)1 << (char)127 << (char)128 << (char)255;
        CPPUNIT_ASSERT_EQUAL( expected.str(), actual );
    
        stream.seek( 0 );
        stream.write( 64 );
        CPPUNIT_ASSERT_EQUAL( (int64_t)1, stream.getPointer() );
        expected.str( "" );
        expected.setstate( ostringstream::goodbit );
        expected << (char)64 << (char)1 << (char)127 << (char)128 << (char)255;
        CPPUNIT_ASSERT_EQUAL( expected.str(), stream.toString() );

        stream.close();
    }

    void testSeek(){
        ByteArrayOutputStream stream;

        // 負の値を指定した場合、0になる
        stream.seek( -100 );
        CPPUNIT_ASSERT_EQUAL( (int64_t)0, stream.getPointer() );

        // 初期に確保されるバッファーより後ろにseekしても、正しくseekできる
        stream.seek( ByteArrayOutputStreamStub::getUnitBufferLength() + 10 );
        stream.write( (int)'a' );

        string actualString = stream.toString();
        vector<char> actual;
        actual.insert( actual.begin(), actualString.begin(), actualString.end() );

        vector<char> expected;
        expected.insert( expected.begin(), ByteArrayOutputStreamStub::getUnitBufferLength() + 10, '\0' );
        expected.insert( expected.end(), 'a' );
        CPPUNIT_ASSERT( expected == actual );
    }

    CPPUNIT_TEST_SUITE( ByteArrayOutputStreamTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST( testSeek );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( ByteArrayOutputStreamTest );

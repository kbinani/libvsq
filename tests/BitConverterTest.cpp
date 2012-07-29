#include "Util.hpp"
#include "../BitConverter.hpp"

using namespace VSQ_NS;
using namespace std;

class BitConverterTest : public CppUnit::TestCase{
public:
    void testGetBytesUInt16BE(){
        vector<char> bytes = BitConverter::getBytesUInt16BE( 0x1234 );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, bytes.size() );
        CPPUNIT_ASSERT_EQUAL( (char)0x12, bytes[0] );
        CPPUNIT_ASSERT_EQUAL( (char)0x34, bytes[1] );
    }

    void testGetBytesUInt32BE(){
        vector<char> bytes = BitConverter::getBytesUInt32BE( 0x12345678 );
        CPPUNIT_ASSERT_EQUAL( (size_t)4, bytes.size() );
        CPPUNIT_ASSERT_EQUAL( (char)0x12, bytes[0] );
        CPPUNIT_ASSERT_EQUAL( (char)0x34, bytes[1] );
        CPPUNIT_ASSERT_EQUAL( (char)0x56, bytes[2] );
        CPPUNIT_ASSERT_EQUAL( (char)0x78, bytes[3] );
    }

    CPPUNIT_TEST_SUITE( BitConverterTest );
    CPPUNIT_TEST( testGetBytesUInt16BE );
    CPPUNIT_TEST( testGetBytesUInt32BE );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( BitConverterTest );

#include "Util.hpp"
#include "../StringUtil.hpp"

using namespace std;

class StringUtilTest : public CppUnit::TestFixture
{
public:
    void explode()
    {
        vector<string> splitted = StringUtil::explode( ",", "aaa,bbb,ccc", 2 );
        CPPUNIT_ASSERT_EQUAL( (vector<string>::size_type)2, splitted.size() );
        CPPUNIT_ASSERT_EQUAL( string( "aaa" ), splitted[0] );
        CPPUNIT_ASSERT_EQUAL( string( "bbb,ccc" ), splitted[1] );
    }

    void explodeNonLimit()
    {
        vector<string> splitted = StringUtil::explode( "_|_", "aaa_|__|_bb_|_" );
        CPPUNIT_ASSERT_EQUAL( (vector<string>::size_type)4, splitted.size() );
        CPPUNIT_ASSERT_EQUAL( string( "aaa" ), splitted[0] );
        CPPUNIT_ASSERT_EQUAL( string( "" ), splitted[1] );
        CPPUNIT_ASSERT_EQUAL( string( "bb" ), splitted[2] );
        CPPUNIT_ASSERT_EQUAL( string( "" ), splitted[3] );
    }

    void explodeWithEscape()
    {
        vector<string> splitted = StringUtil::explode( ",", "a,b__,c,d", string::npos, "__" );
        CPPUNIT_ASSERT_EQUAL( 3, (int)splitted.size() );
        CPPUNIT_ASSERT_EQUAL( string( "a" ), splitted[0] );
        CPPUNIT_ASSERT_EQUAL( string( "b__,c" ), splitted[1] );
        CPPUNIT_ASSERT_EQUAL( string( "d" ), splitted[2] );
    }

    void explodeDelimiterNotFound()
    {
        vector<string> splitted = StringUtil::explode( "|", "aaa,bbb,ccc" );
        CPPUNIT_ASSERT_EQUAL( (string::size_type)1, splitted.size() );
        CPPUNIT_ASSERT_EQUAL( string( "aaa,bbb,ccc" ), splitted[0] );
    }

    void testReplace()
    {
        string actual = StringUtil::replace( "abc", "a", "A" );
        string expected = "Abc";
        CPPUNIT_ASSERT_EQUAL( expected, actual );

        actual = StringUtil::replace( "abc", "a", "a" );
        expected = "abc";
        CPPUNIT_ASSERT_EQUAL( expected, actual );
    }

    void testParseInt(){
        int actual = StringUtil::parseInt( "1" );
        CPPUNIT_ASSERT_EQUAL( 1, actual );
    }

    void testToString(){
        std::string actual = StringUtil::toString( 10 );
        CPPUNIT_ASSERT_EQUAL( string( "10" ), actual );
    }

    CPPUNIT_TEST_SUITE( StringUtilTest );
    CPPUNIT_TEST( explode );
    CPPUNIT_TEST( explodeNonLimit );
    CPPUNIT_TEST( explodeWithEscape );
    CPPUNIT_TEST( explodeDelimiterNotFound );
    CPPUNIT_TEST( testReplace );
    CPPUNIT_TEST( testParseInt );
    CPPUNIT_TEST( testToString );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( StringUtilTest );

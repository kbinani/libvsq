#include "../Lyric.hpp"
#include "Util.hpp"

using namespace std;
using namespace VSQ_NS;

class LyricTest : public CppUnit::TestCase
{
public:
    void testConstructWithLine()
    {
        string line = "あ,a,0.4,0,0";
        Lyric lyric = Lyric( line );
        CPPUNIT_ASSERT( false == lyric.isProtected );
        CPPUNIT_ASSERT_EQUAL( string( "あ" ), lyric.phrase );
        CPPUNIT_ASSERT_EQUAL( string( "a" ), lyric.getPhoneticSymbol() );
        CPPUNIT_ASSERT_EQUAL( 0.4, lyric.lengthRatio );

        line = "あ\"\",a,1.0,0,0";
        lyric = Lyric( line );
        CPPUNIT_ASSERT_EQUAL( string( "あ\"" ), lyric.phrase );

        line = "は,h a,1.0,64,0,1";
        lyric = Lyric( line );
        CPPUNIT_ASSERT_EQUAL( string( "は" ), lyric.phrase );
        CPPUNIT_ASSERT( lyric.isProtected );
        vector<string> symbols = lyric.getPhoneticSymbolList();
        CPPUNIT_ASSERT_EQUAL( 2, (int)symbols.size() );
        CPPUNIT_ASSERT_EQUAL( string( "h" ), symbols[0] );
        CPPUNIT_ASSERT_EQUAL( string( "a" ), symbols[1] );

        line = "\"a\",\"b\",1,0,0";
        lyric = Lyric( line );
        CPPUNIT_ASSERT_EQUAL( string( "a" ), lyric.phrase );
        CPPUNIT_ASSERT_EQUAL( string( "b" ), lyric.getPhoneticSymbol() );
    }

    void testConstructWithPhrase()
    {
        Lyric lyric = Lyric( "は", "h a" );
        CPPUNIT_ASSERT_EQUAL( string( "は" ), lyric.phrase );
        CPPUNIT_ASSERT_EQUAL( string( "h a" ), lyric.getPhoneticSymbol() );
        CPPUNIT_ASSERT_EQUAL(string("64,0" ), lyric.getConsonantAdjustment());
        CPPUNIT_ASSERT( false == lyric.isProtected );
    }

    void testGetPhoneticSymbol()
    {
        Lyric lyric = Lyric( "は,h a,1.0,64,0,0" );
        string actual = lyric.getPhoneticSymbol();
        CPPUNIT_ASSERT_EQUAL( string( "h a" ), actual );
    }

    void testGetPhoneticSymbolList()
    {
        Lyric lyric = Lyric( "は,h a,1.0,64,0,0" );
        vector<string> actual = lyric.getPhoneticSymbolList();
        CPPUNIT_ASSERT_EQUAL( (vector<string>::size_type)2, actual.size() );
        CPPUNIT_ASSERT_EQUAL( string( "h" ), actual[0] );
        CPPUNIT_ASSERT_EQUAL( string( "a" ), actual[1] );
    }

    void testSetPhoneticSymbol()
    {
        Lyric lyric = Lyric( "あ,a,1.0,0,0" );
        lyric.setPhoneticSymbol( "h a" );
        vector<string> actual = lyric.getPhoneticSymbolList();
        CPPUNIT_ASSERT_EQUAL( 2, (int)actual.size() );
        CPPUNIT_ASSERT_EQUAL( string( "h" ), actual[0] );
        CPPUNIT_ASSERT_EQUAL( string( "a" ), actual[1] );
    }

    void testGetConsonantAdjustmentList()
    {
        Lyric lyric = Lyric( "は,h a,1,64,0,0" );
        const vector<int> actual = lyric.getConsonantAdjustmentList();
        CPPUNIT_ASSERT_EQUAL( 2, (int)actual.size() );
        CPPUNIT_ASSERT_EQUAL( 64, actual[0] );
        CPPUNIT_ASSERT_EQUAL( 0, actual[1] );
    }

    void testGetConsonantAdjustmentListWithNil()
    {
        Lyric lyric = Lyric( "は,h a,1,32,16,0" );
        vector<int> actual = lyric.getConsonantAdjustmentList();
        CPPUNIT_ASSERT_EQUAL( 2, (int)actual.size() );
        CPPUNIT_ASSERT_EQUAL( 32, actual[0] );
        CPPUNIT_ASSERT_EQUAL( 16, actual[1] );

        lyric._consonantAdjustment.clear();
        actual = lyric.getConsonantAdjustmentList();
        CPPUNIT_ASSERT_EQUAL( 2, (int)actual.size() );
        CPPUNIT_ASSERT_EQUAL( 64, actual[0] );
        CPPUNIT_ASSERT_EQUAL( 0, actual[1] );
    }

    void testGetConsonantAdjustment()
    {
        Lyric lyric = Lyric( "は,h a,1,64,0,0" );
        string actual = lyric.getConsonantAdjustment();
        CPPUNIT_ASSERT_EQUAL(string("64,0"), actual);
    }

    void testSetConsonantAdjustment() {
        Lyric lyric = Lyric("は,h a,1,64,0,0");
        lyric.setConsonantAdjustment("61,0");
        CPPUNIT_ASSERT_EQUAL(string("61,0"), lyric.getConsonantAdjustment());
    }

    void testToString()
    {
        string expected = "は,h a,1,64,0,0";
        Lyric lyric = Lyric( expected );
        CPPUNIT_ASSERT_EQUAL( expected, lyric.toString() );
    }

    void testEquals()
    {
        Lyric a = Lyric( "は,h a,1,64,0,0" );
        Lyric b = Lyric( "は,h a,1,64,0,0" );
        CPPUNIT_ASSERT( a.equals( b ) );
        Lyric c = Lyric( "あ,a,1.0,0,0" );
        CPPUNIT_ASSERT( false == a.equals( c ) );
    }

    void testEqualsForSynth()
    {
        Lyric a = Lyric( "は,h a,1,64,0,0" );
        Lyric b = Lyric( "あ,h a,0.5,64,0,1" );
        CPPUNIT_ASSERT( a.equalsForSynth( b ) );

        // consonantAdjustmentだけ違う場合
        Lyric c = Lyric( "は,h a,1,64,1,0" );
        CPPUNIT_ASSERT( false == a.equalsForSynth( c ) );

        // 発音記号が違う場合
        Lyric d = Lyric( "は,h e,1,64,0,0" );
        CPPUNIT_ASSERT( false == a.equalsForSynth( d ) );
    }

    void testClone(){
        Lyric lyric( "は,h a,1,64,0,0" );
        Lyric copy = lyric.clone();
        CPPUNIT_ASSERT_EQUAL( lyric.toString(), copy.toString() );
    }

    CPPUNIT_TEST_SUITE( LyricTest );
    CPPUNIT_TEST( testConstructWithLine );
    CPPUNIT_TEST( testConstructWithPhrase );
    CPPUNIT_TEST( testGetPhoneticSymbol );
    CPPUNIT_TEST( testGetPhoneticSymbolList );
    CPPUNIT_TEST( testSetPhoneticSymbol );
    CPPUNIT_TEST( testGetConsonantAdjustmentList );
    CPPUNIT_TEST( testGetConsonantAdjustmentListWithNil );
    CPPUNIT_TEST( testGetConsonantAdjustment );
    CPPUNIT_TEST(testSetConsonantAdjustment);
    CPPUNIT_TEST( testToString );
    CPPUNIT_TEST( testEquals );
    CPPUNIT_TEST( testEqualsForSynth );
    CPPUNIT_TEST( testClone );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( LyricTest );

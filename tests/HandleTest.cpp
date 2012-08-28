#include "Util.hpp"
#include "../Handle.hpp"

using namespace std;
using namespace VSQ_NS;

class HandleTest : public CppUnit::TestCase
{
public:
    void testConstructIconDynamicsHandle(){
        Handle handle( HandleType::DYNAMICS );
        CPPUNIT_ASSERT_EQUAL( ArticulationType::DYNAFF, handle.getArticulation() );
    }
    
    void testConstructNoteHeadHandle(){
        Handle handle( HandleType::NOTE_HEAD );
        CPPUNIT_ASSERT_EQUAL( ArticulationType::NOTE_ATTACK, handle.getArticulation() );
    }
    
    void testIsDynaffType(){
        Handle handle( HandleType::DYNAMICS );
        handle.iconId = "";
        CPPUNIT_ASSERT( false == handle.isDynaffType() );
        handle.iconId = "$05000000";
        CPPUNIT_ASSERT( false == handle.isDynaffType() );
        handle.iconId = "$05010000";
        CPPUNIT_ASSERT( handle.isDynaffType() );
    }
    
    void testIsCrescendType(){
        Handle handle( HandleType::DYNAMICS );
        handle.iconId = "";
        CPPUNIT_ASSERT( false == handle.isCrescendType() );
        handle.iconId = "$05000000";
        CPPUNIT_ASSERT( false == handle.isCrescendType() );
        handle.iconId = "$05020000";
        CPPUNIT_ASSERT( handle.isCrescendType() );
    }
    
    void testIsDecrescendType(){
        Handle handle( HandleType::DYNAMICS );
        handle.iconId = "";
        CPPUNIT_ASSERT( false == handle.isDecrescendType() );
        handle.iconId = "$05000000";
        CPPUNIT_ASSERT( false == handle.isDecrescendType() );
        handle.iconId = "$05030000";
        CPPUNIT_ASSERT( handle.isDecrescendType() );
    }
    
    void testGetterAndSetterCaption(){
        Handle handle( HandleType::DYNAMICS );
        handle.setCaption( "foo" );
        CPPUNIT_ASSERT_EQUAL( string( "foo" ), handle.getCaption() );
    }
    
    void testGetterAndSetterStartDyn(){
        Handle handle( HandleType::DYNAMICS );
        int expected = 100;
        CPPUNIT_ASSERT( expected != handle.getStartDyn() );
        handle.setStartDyn( expected );
        CPPUNIT_ASSERT( expected == handle.getStartDyn() );
    }
    
    void testGetterAndSetterEndDyn(){
        Handle handle( HandleType::DYNAMICS );
        int expected = 100;
        CPPUNIT_ASSERT( expected != handle.getEndDyn() );
        handle.setEndDyn( expected );
        CPPUNIT_ASSERT( expected == handle.getEndDyn() );
    }
    
    void testGetterAndSetterDynBP(){
        Handle handle( HandleType::DYNAMICS );
        VibratoBPList dynBP( "2", "0.0,1.0", "1,2" );
        CPPUNIT_ASSERT_EQUAL( 0, handle.getDynBP().size() );
        handle.setDynBP( dynBP );
        CPPUNIT_ASSERT_EQUAL( string( "0=1,1=2" ), handle.getDynBP().getData() );
    }
    
    void testGetterAndSetterLength(){
        Handle handle( HandleType::VIBRATO );
        tick_t expected = 847;
        CPPUNIT_ASSERT( expected != handle.getLength() );
        handle.setLength( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getLength() );
    }
    
    void testGetHandleIndexFromString(){
        CPPUNIT_ASSERT_EQUAL( 2, Handle::getHandleIndexFromString( "h#0002" ) );
    }
    
    void testGetterAndSetterDepth(){
        Handle handle( HandleType::NOTE_HEAD );
        int expected = 1234;
        CPPUNIT_ASSERT( expected != handle.getDepth() );
        handle.setDepth( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getDepth() );
    }
    
    void testGetterAndSetterDuration(){
        Handle handle( HandleType::NOTE_HEAD );
        int expected = 947;
        CPPUNIT_ASSERT( expected != handle.getDuration() );
        handle.setDuration( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getDuration() );
    }
    
    void testGetDisplayString(){
        Handle handle( HandleType::NOTE_HEAD );
        handle.ids = "goo";
        handle.setCaption( "gle" );
        CPPUNIT_ASSERT_EQUAL( string( "google" ), handle.getDisplayString() );
    }
    
    void testGetterAndSetterRateBP(){
        Handle handle( HandleType::VIBRATO );
        VibratoBPList rateBP( "2", "0.0,1.0", "1,128" );
        CPPUNIT_ASSERT( "0=1,1=128" != handle.getRateBP().getData() );
        handle.setRateBP( rateBP );
        CPPUNIT_ASSERT_EQUAL( string( "0=1,1=128" ), handle.getRateBP().getData() );
    }
    
    void testGetterAndSetterDepthBP(){
        Handle handle( HandleType::VIBRATO );
        VibratoBPList depthBP( "2", "0.0,1.0", "1,128" );
        CPPUNIT_ASSERT( "0=1,1=128" != handle.getDepthBP().getData() );
        handle.setDepthBP( depthBP );
        CPPUNIT_ASSERT_EQUAL( string( "0=1,1=128" ), handle.getDepthBP().getData() );
    }
    
    void testGetterAndSetterStartRate(){
        Handle handle( HandleType::VIBRATO );
        int expected = 12345;
        CPPUNIT_ASSERT( expected != handle.getStartRate() );
        handle.setStartRate( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getStartRate() );
    }
    
    void testGetterAndSetterStartDepth(){
        Handle handle( HandleType::VIBRATO );
        int expected = 12345;
        CPPUNIT_ASSERT( expected != handle.getStartDepth() );
        handle.setStartDepth( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getStartDepth() );
    }
    
    void testGetterAndSetterLyric(){
        Handle handle( HandleType::LYRIC );
        handle.setLyricAt( 0, Lyric( "は", "h a" ) );
        Lyric lyric( "ら", "4 a" );
        handle.setLyricAt( 1, lyric );
        CPPUNIT_ASSERT_EQUAL( 2, handle.getLyricCount() );
        CPPUNIT_ASSERT( handle.getLyricAt( 1 ).equals( lyric ) );
    }
    
    void testCloneIconDynamicsHandle(){
        Handle handle( HandleType::DYNAMICS );
        handle.iconId = "$05010000";
        handle.ids = "foo";
        handle.original = 1;
        handle.setCaption( "bar" );
        handle.setStartDyn( 2 );
        handle.setEndDyn( 3 );
        handle.setLength( 4 );
        Handle copy = handle.clone();
        CPPUNIT_ASSERT_EQUAL( string( "$05010000" ), copy.iconId );
        CPPUNIT_ASSERT_EQUAL( string( "foo" ), copy.ids );
        CPPUNIT_ASSERT_EQUAL( 1, copy.original );
        CPPUNIT_ASSERT_EQUAL( string( "bar" ), copy.getCaption() );
        CPPUNIT_ASSERT_EQUAL( 2, copy.getStartDyn() );
        CPPUNIT_ASSERT_EQUAL( 3, copy.getEndDyn() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)4, copy.getLength() );
        CPPUNIT_ASSERT_EQUAL( 0, copy.getDynBP().size() );
    
        VibratoBPList dynBP( "2", "0.0,1.0", "1,64" );
        handle.setDynBP( dynBP );
        copy = handle.clone();
        CPPUNIT_ASSERT_EQUAL( string( "0=1,1=64" ), copy.getDynBP().getData() );
    }
    
    void testCloneLyricHandle(){
        Handle handle( HandleType::LYRIC );
        handle.setLyricAt( 0, Lyric( "ら", "4 a" ) );
        handle.index = 10;
        Handle copy = handle.clone();
        CPPUNIT_ASSERT_EQUAL( handle.index, copy.index );
        Lyric original = handle.getLyricAt( 0 );
        Lyric copied = copy.getLyricAt( 0 );
        CPPUNIT_ASSERT( original.equals( copied ) );
    }
    
    void testCloneNoteHeadHandle(){
        Handle handle( HandleType::NOTE_HEAD );
        handle.index = 1;
        handle.iconId = "$05010000";
        handle.ids = "dwango";
        handle.original = 2;
        handle.setCaption( "niwango" );
        handle.setLength( 3 );
        handle.setDuration( 4 );
        handle.setDepth( 5 );
    
        Handle copy = handle.clone();
        CPPUNIT_ASSERT_EQUAL( 1, copy.index );
        CPPUNIT_ASSERT_EQUAL( string( "$05010000" ), copy.iconId );
        CPPUNIT_ASSERT_EQUAL( string( "dwango" ), copy.ids );
        CPPUNIT_ASSERT_EQUAL( 2, copy.original );
        CPPUNIT_ASSERT_EQUAL( string( "niwango" ), copy.getCaption() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)3, copy.getLength() );
        CPPUNIT_ASSERT_EQUAL( 4, copy.getDuration() );
        CPPUNIT_ASSERT_EQUAL( 5, copy.getDepth() );
    }
    
    void testCloneVibratoHandle(){
        Handle handle( HandleType::VIBRATO );
        handle.index = 1;
        handle.iconId = "hahaha";
        handle.ids = "baka";
        handle.original = 2;
        handle.setCaption( "aho" );
        handle.setLength( 3 );
        handle.setStartDepth( 4 );
        handle.setDepthBP( VibratoBPList( "2", "0.0,1.0", "32,56" ) );
        handle.setStartRate( 5 );
        handle.setRateBP( VibratoBPList( "2", "0.0,1.0", "64,128" ) );
        Handle copy = handle.clone();
        CPPUNIT_ASSERT_EQUAL( 1, copy.index );
        CPPUNIT_ASSERT_EQUAL( string( "hahaha" ), copy.iconId );
        CPPUNIT_ASSERT_EQUAL( string( "baka" ), copy.ids );
        CPPUNIT_ASSERT_EQUAL( 2, copy.original );
        CPPUNIT_ASSERT_EQUAL( string( "aho" ), copy.getCaption() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)3, copy.getLength() );
        CPPUNIT_ASSERT_EQUAL( 4, copy.getStartDepth() );
        CPPUNIT_ASSERT_EQUAL( string( "0=32,1=56" ), copy.getDepthBP().getData() );
        CPPUNIT_ASSERT_EQUAL( 5, copy.getStartRate() );
        CPPUNIT_ASSERT_EQUAL( string( "0=64,1=128" ), copy.getRateBP().getData() );
    }
    
    void testCloneSingerHandle(){
        Handle handle( HandleType::SINGER );
        handle.setCaption( "bar" );
        handle.iconId = "$07010001";
        handle.ids = "foo";
        handle.index = 1;
        handle.setLength( 2 );
        handle.original = 3;
        handle.program = 4;
        handle.language= 5;
    
        Handle copy = handle.clone();
        CPPUNIT_ASSERT_EQUAL( handle.getCaption(), copy.getCaption() );
        CPPUNIT_ASSERT_EQUAL( handle.iconId, copy.iconId );
        CPPUNIT_ASSERT_EQUAL( handle.ids, copy.ids );
        CPPUNIT_ASSERT_EQUAL( handle.index, copy.index );
        CPPUNIT_ASSERT_EQUAL( handle.language, copy.language );
        CPPUNIT_ASSERT_EQUAL( handle.getLength(), copy.getLength() );
        CPPUNIT_ASSERT_EQUAL( handle.original, copy.original );
        CPPUNIT_ASSERT_EQUAL( handle.program, copy.program );
    }
    
    void testAddLyric(){
        Handle handle( HandleType::LYRIC );
        handle.addLyric( Lyric( "ら", "4 a" ) );

        CPPUNIT_ASSERT_EQUAL( 1, handle.getLyricCount() );
        CPPUNIT_ASSERT_EQUAL( string( "ら" ), handle.getLyricAt( 0 ).phrase );
        CPPUNIT_ASSERT_EQUAL( string( "4 a" ), handle.getLyricAt( 0 ).getPhoneticSymbol() );
    }

    CPPUNIT_TEST_SUITE( HandleTest );
    CPPUNIT_TEST( testConstructIconDynamicsHandle );
    CPPUNIT_TEST( testConstructNoteHeadHandle );
    CPPUNIT_TEST( testIsDynaffType );
    CPPUNIT_TEST( testIsCrescendType );
    CPPUNIT_TEST( testIsDecrescendType );
    CPPUNIT_TEST( testGetterAndSetterCaption );
    CPPUNIT_TEST( testGetterAndSetterStartDyn );
    CPPUNIT_TEST( testGetterAndSetterEndDyn );
    CPPUNIT_TEST( testGetterAndSetterDynBP );
    CPPUNIT_TEST( testGetterAndSetterLength );
    CPPUNIT_TEST( testGetHandleIndexFromString );
    CPPUNIT_TEST( testGetterAndSetterDepth );
    CPPUNIT_TEST( testGetterAndSetterDuration );
    CPPUNIT_TEST( testGetDisplayString );
    CPPUNIT_TEST( testGetterAndSetterRateBP );
    CPPUNIT_TEST( testGetterAndSetterDepthBP );
    CPPUNIT_TEST( testGetterAndSetterStartRate );
    CPPUNIT_TEST( testGetterAndSetterStartDepth );
    CPPUNIT_TEST( testGetterAndSetterLyric );
    CPPUNIT_TEST( testCloneIconDynamicsHandle );
    CPPUNIT_TEST( testCloneLyricHandle );
    CPPUNIT_TEST( testCloneNoteHeadHandle );
    CPPUNIT_TEST( testCloneVibratoHandle );
    CPPUNIT_TEST( testCloneSingerHandle );
    CPPUNIT_TEST( testAddLyric );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( HandleTest );

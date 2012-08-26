#include "Util.hpp"
#include "../Handle.hpp"

using namespace std;
using namespace VSQ_NS;

class HandleTest : public CppUnit::TestCase
{
public:
    Handle getLyricHandle(){
        Handle handle( HandleType::LYRIC );
        Lyric lyric0( "あ", "a" );
        lyric0.setConsonantAdjustment( "0" );
        lyric0.lengthRatio = 0.4;
        lyric0.isProtected = true;
        Lyric lyric1( "は", "h a" );
        lyric1.setConsonantAdjustment( "64,0" );
        lyric1.lengthRatio = 0.6;
        lyric1.isProtected = false;
        handle.setLyricAt( 0, lyric0 );
        handle.addLyric( lyric1 );
        handle.index = 1;
        return handle;
    }
    
    Handle getVibratoHandle(){
        Handle result( HandleType::VIBRATO );
        result.iconId = "$04040004";
        result.ids = "normal-da-yo";
        result.setCaption( "キャプションです=あ" );
        result.original = 5;
        result.setLength( 120 );
        result.index = 1;

        result.setStartDepth( 64 );
        vector<double> depthBPX;
        depthBPX.push_back( 0.5 );
        depthBPX.push_back( 0.75 );
        depthBPX.push_back( 1.0 );
        vector<int> depthBPY;
        depthBPY.push_back( 64 );
        depthBPY.push_back( 32 );
        depthBPY.push_back( 0 );
        VibratoBPList depthBP( depthBPX, depthBPY );
        result.setDepthBP( depthBP );

        result.setStartRate( 64 );
        vector<double> rateBPX;
        rateBPX.push_back( 0.5 );
        rateBPX.push_back( 0.75 );
        rateBPX.push_back( 1.0 );
        vector<int> rateBPY;
        rateBPY.push_back( 64 );
        rateBPY.push_back( 32 );
        rateBPY.push_back( 0 );
        VibratoBPList rateBP( rateBPX, rateBPY );
        result.setRateBP( rateBP );

        return result;
    }
    
    Handle getSingerHandle(){
        Handle result( HandleType::SINGER );
        result.iconId = "$07010002";
        result.ids = "Miku3=God";
        result.original = 2;
        result.setCaption( "" );
        result.setLength( 1 );
        result.language = 1;
        result.program = 2;
        result.index = 2;
        return result;
    }
    
    Handle getAttackHandle(){
        Handle result( HandleType::NOTE_HEAD );
        result.iconId = "$01010002";
        result.ids = "accent";
        result.original = 2;
        result.index = 3;
        result.setCaption( "Accent" );
        result.setLength( 120 );
        result.setDuration( 64 );
        result.setDepth( 63 );
        return result;
    }
    
    Handle getCrescendoHandle(){
        Handle result( HandleType::DYNAMICS );
        result.iconId = "$05020001";
        result.ids = "Crescendo";
        result.original = 4;
        result.index = 4;
        result.setCaption( "Zero Crescendo Curve" );
        result.setLength( 960 );
        result.setStartDyn( 2 );
        result.setEndDyn( 38 );
        vector<double> dynBPX;
        dynBPX.push_back( 0.5 );
        vector<int> dynBPY;
        dynBPY.push_back( 11 );
        VibratoBPList dynBP( dynBPX, dynBPY );
        result.setDynBP( dynBP );
        return result;
    }
    
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
    
    void testWrite(){
        Handle handle = getLyricHandle();
        TextStream dest;
        handle.write( dest );
        dest.setPointer( -1 );
        CPPUNIT_ASSERT_EQUAL( string( "[h#0001]" ), dest.readLine() );
        CPPUNIT_ASSERT_EQUAL( string( "L0=\"あ\",\"a\",0.4,0,1" ), dest.readLine() );
        CPPUNIT_ASSERT_EQUAL( string( "L1=\"は\",\"h a\",0.6,64,0,0" ), dest.readLine() );
    }
    
    void testLyricToString(){
        Handle handle = getLyricHandle();

        string expected =
            "[h#0001]\n"
            "L0=\"あ\",\"a\",0.4,0,1\n"
            "L1=\"は\",\"h a\",0.6,64,0,0";
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
    }
    
    void testVibratoToString(){
        Handle handle = getVibratoHandle();
        string expected =
            "[h#0001]\n"
            "IconID=$04040004\n"
            "IDS=normal-da-yo\n"
            "Original=5\n"
            "Caption=キャプションです=あ\n"
            "Length=120\n"
            "StartDepth=64\n"
            "DepthBPNum=3\n"
            "DepthBPX=0.500000,0.750000,1.000000\n"
            "DepthBPY=64,32,0\n"
            "StartRate=64\n"
            "RateBPNum=3\n"
            "RateBPX=0.500000,0.750000,1.000000\n"
            "RateBPY=64,32,0";
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
    
        handle.setRateBP( VibratoBPList( vector<double>(), vector<int>() ) );
        handle.setDepthBP( VibratoBPList( vector<double>(), vector<int>() ) );
        expected =
            "[h#0001]\n"
            "IconID=$04040004\n"
            "IDS=normal-da-yo\n"
            "Original=5\n"
            "Caption=キャプションです=あ\n"
            "Length=120\n"
            "StartDepth=64\n"
            "DepthBPNum=0\n"
            "StartRate=64\n"
            "RateBPNum=0";
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
    }
    
    void testSingerToString(){
        Handle handle = getSingerHandle();
        string expected =
            "[h#0002]\n"
            "IconID=$07010002\n"
            "IDS=Miku3=God\n"
            "Original=2\n"
            "Caption=\n"
            "Length=1\n"
            "Language=1\n"
            "Program=2";
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
    }
    
    void testAttackToString(){
        Handle handle = getAttackHandle();
        string expected =
            "[h#0003]\n"
            "IconID=$01010002\n"
            "IDS=accent\n"
            "Original=2\n"
            "Caption=Accent\n"
            "Length=120\n"
            "Duration=64\n"
            "Depth=63";
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
    }
    
    void testCrescendoToString(){
        Handle handle = getCrescendoHandle();
        string expected =
            "[h#0004]\n"
            "IconID=$05020001\n"
            "IDS=Crescendo\n"
            "Original=4\n"
            "Caption=Zero Crescendo Curve\n"
            "StartDyn=2\n"
            "EndDyn=38\n"
            "Length=960\n"
            "DynBPNum=1\n"
            "DynBPX=0.500000\n"
            "DynBPY=11";
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
    
        // dynBPのデータ点が複数
        handle.setDynBP( VibratoBPList( "2", "0.4,0.8", "1,2" ) );
        expected =
            "[h#0004]\n"
            "IconID=$05020001\n"
            "IDS=Crescendo\n"
            "Original=4\n"
            "Caption=Zero Crescendo Curve\n"
            "StartDyn=2\n"
            "EndDyn=38\n"
            "Length=960\n"
            "DynBPNum=2\n"
            "DynBPX=0.400000,0.800000\n"
            "DynBPY=1,2";
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
    
        // dynBPのデータ点が 0 個
        handle.setDynBP( VibratoBPList( vector<double>(), vector<int>() ) );
        expected =
            "[h#0004]\n"
            "IconID=$05020001\n"
            "IDS=Crescendo\n"
            "Original=4\n"
            "Caption=Zero Crescendo Curve\n"
            "StartDyn=2\n"
            "EndDyn=38\n"
            "Length=960\n"
            "DynBPNum=0";
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
    
        // dynBPがから
        handle.setDynBP( VibratoBPList( vector<double>(), vector<int>() ) );
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
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
    CPPUNIT_TEST( testWrite );
    CPPUNIT_TEST( testLyricToString );
    CPPUNIT_TEST( testVibratoToString );
    CPPUNIT_TEST( testSingerToString );
    CPPUNIT_TEST( testAttackToString );
    CPPUNIT_TEST( testCrescendoToString );
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

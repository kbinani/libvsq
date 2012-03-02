#include "Util.h"
#include "../Handle.h"

using namespace std;
using namespace VSQ_NS;

class HandleStub : public Handle
{
public:
    static int getHandleIndexFromString(const string &s)
    {
        return Handle::getHandleIndexFromString( s );
    }
};

class HandleTest : public CppUnit::TestCase
{
public:
    void getLyricStream( TextStream &stream )
    {
        stream.writeLine( "L0=あ,a,0.4,0,1" );
        stream.writeLine( "L1=は,h a,0.6,64,0,0" );
        stream.setPointer( -1 );
    }
    
    void getVibratoStream( TextStream &stream )
    {
        stream.writeLine( "IconID=$04040004" );
        stream.writeLine( "IDS=normal-da-yo" );
        stream.writeLine( "Caption=キャプションです=あ" );
        stream.writeLine( "Original=5" );
        stream.writeLine( "Length=120" );
        stream.writeLine( "StartDepth=64" );
        stream.writeLine( "DepthBPNum=3" );
        stream.writeLine( "DepthBPX=0.500000,0.750000,1.000000" );
        stream.writeLine( "DepthBPY=64,32,0" );
        stream.writeLine( "StartRate=64" );
        stream.writeLine( "RateBPNum=3" );
        stream.writeLine( "RateBPX=0.500000,0.750000,1.000000" );
        stream.writeLine( "RateBPY=64,32,0" );
        stream.writeLine( "[h#0002]" );
        stream.setPointer( -1 );
    }
    
    void getSingerStream( TextStream &stream )
    {
        stream.writeLine( "IconID=$07010002" );
        stream.writeLine( "IDS=Miku3=God" );
        stream.writeLine( "Original=2" );
        stream.writeLine( "Caption=" );
        stream.writeLine( "Length=1" );
        stream.writeLine( "Language=1" );
        stream.writeLine( "Program=2" );
        stream.setPointer( -1 );
    }
    
    void getAttackStream( TextStream &stream )
    {
        stream.writeLine( "IconID=$01010002" );
        stream.writeLine( "IDS=accent" );
        stream.writeLine( "Original=2" );
        stream.writeLine( "Caption=Accent" );
        stream.writeLine( "Length=120" );
        stream.writeLine( "Duration=64" );
        stream.writeLine( "Depth=63" );
        stream.setPointer( -1 );
    }
    
    void getCrescendoStream( TextStream &stream )
    {
        stream.writeLine( "IconID=$05020001" );
        stream.writeLine( "IDS=Crescendo" );
        stream.writeLine( "Original=4" );
        stream.writeLine( "Caption=Zero Crescendo Curve" );
        stream.writeLine( "Length=960" );
        stream.writeLine( "StartDyn=2" );
        stream.writeLine( "EndDyn=38" );
        stream.writeLine( "DynBPNum=1" );
        stream.writeLine( "DynBPX=0.5" );
        stream.writeLine( "DynBPY=11" );
        stream.setPointer( -1 );
    }
    
    void testConstructIconDynamicsHandle()
    {
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
    
    /**
     * 歌詞ハンドルの読み込みテスト
     * EOFで読み込みが終了する場合
     */
    void testConstructLyricFromTextStreamStopWithEOF()
    {
        TextStream stream;
        getLyricStream( stream );
        string lastLine = "";
        int index = 100;
    
        {//TODO:
            cout << "testConstructLyricFromTextStreamStopWithEOF; before ctor" << endl;
        }
        Handle handle( stream, index, lastLine );
        {//TODO:
            cout << "testConstructLyricFromTextStreamStopWithEOF; after ctor" << endl;
        }
        CPPUNIT_ASSERT_EQUAL( HandleType::LYRIC, handle.getHandleType() );
        CPPUNIT_ASSERT_EQUAL( index, handle.index );
        CPPUNIT_ASSERT_EQUAL( 2, handle.getLyricCount() );
    
        Lyric lyric1 = handle.getLyricAt( 0 );
        CPPUNIT_ASSERT_EQUAL( string( "あ" ), lyric1.phrase );
        CPPUNIT_ASSERT_EQUAL( string( "a" ), lyric1.getPhoneticSymbol() );
        CPPUNIT_ASSERT_EQUAL( 0.4, lyric1.lengthRatio );
        CPPUNIT_ASSERT_EQUAL( string( "0" ), lyric1.getConsonantAdjustment() );
        CPPUNIT_ASSERT( lyric1.isProtected );
    
        Lyric lyric2 = handle.getLyricAt( 1 );
        CPPUNIT_ASSERT_EQUAL( string( "は" ), lyric2.phrase );
        CPPUNIT_ASSERT_EQUAL( string( "h a" ), lyric2.getPhoneticSymbol() );
        CPPUNIT_ASSERT_EQUAL( 0.6, lyric2.lengthRatio );
        CPPUNIT_ASSERT_EQUAL( string( "64 0" ), lyric2.getConsonantAdjustment() );
        CPPUNIT_ASSERT( false == lyric2.isProtected );
    }
    
    /**
     * 歌詞ハンドルの読み込みテスト
     * 次の歌詞ハンドルの先頭に到達して読み込みが終了する場合
     */
    void testConstructLyricFromTextStreamStopWithNextHandle()
    {
        TextStream stream;
        stream.writeLine( "L0=あ,a,0.4,0,1" );
        stream.writeLine( "[h#0002]" );
        stream.setPointer( -1 );
        string lastLine = "";
        int index = 100;
    
        Handle handle( stream, index, lastLine );
        CPPUNIT_ASSERT_EQUAL( HandleType::LYRIC, handle.getHandleType() );
        CPPUNIT_ASSERT_EQUAL( index, handle.index );
        CPPUNIT_ASSERT_EQUAL( 1, handle.getLyricCount() );
    
        CPPUNIT_ASSERT_EQUAL( 0, handle.getRateBP().size() );
        CPPUNIT_ASSERT_EQUAL( 0, handle.getDepthBP().size() );
        CPPUNIT_ASSERT_EQUAL( 0, handle.getDynBP().size() );
    
        Lyric lyric = handle.getLyricAt( 0 );
        CPPUNIT_ASSERT_EQUAL( string( "あ" ), lyric.phrase );
        CPPUNIT_ASSERT_EQUAL( string( "a" ), lyric.getPhoneticSymbol() );
        CPPUNIT_ASSERT_EQUAL( 0.4, lyric.lengthRatio );
        CPPUNIT_ASSERT_EQUAL( string( "0" ), lyric.getConsonantAdjustment() );
        CPPUNIT_ASSERT( lyric.isProtected );
    
        CPPUNIT_ASSERT_EQUAL( string( "[h#0002]" ), lastLine );
    }
    
    void testConstructVibratoFromTextStream()
    {
        TextStream stream;
        getVibratoStream( stream );
        string lastLine = "";
        int index = 101;
        Handle handle( stream, index, lastLine );
    
        CPPUNIT_ASSERT_EQUAL( HandleType::VIBRATO, handle.getHandleType() );
        CPPUNIT_ASSERT_EQUAL( string( "$04040004" ), handle.iconId );
        CPPUNIT_ASSERT_EQUAL( string( "normal-da-yo" ), handle.ids );
        CPPUNIT_ASSERT_EQUAL( string( "キャプションです=あ" ), handle.getCaption() );
        CPPUNIT_ASSERT_EQUAL( 5, handle.original );
        CPPUNIT_ASSERT_EQUAL( (tick_t)120, handle.getLength() );
        CPPUNIT_ASSERT_EQUAL( 64, handle.getStartDepth() );
        CPPUNIT_ASSERT_EQUAL( string( "0.5=64,0.75=32,1=0" ), handle.getDepthBP().getData() );
        CPPUNIT_ASSERT_EQUAL( 64, handle.getStartRate() );
        CPPUNIT_ASSERT_EQUAL( string( "0.5=64,0.75=32,1=0" ), handle.getRateBP().getData() );
    
        CPPUNIT_ASSERT_EQUAL( string( "[h#0002]" ), lastLine );
    }
    
    void testConstructVibratoFromTextStreamWithoutBP(){
        TextStream stream;
        stream.writeLine( "IconID=$04040004" );
        stream.writeLine( "IDS=normal-da-yo" );
        stream.writeLine( "Caption=キャプションです=あ" );
        stream.writeLine( "Original=5" );
        stream.writeLine( "Length=120" );
        stream.writeLine( "StartDepth=64" );
        stream.writeLine( "StartRate=64" );
        stream.writeLine( "[h#0002]" );
        stream.setPointer( -1 );
    
        string lastLine = "";
        int index = 101;
        Handle handle( stream, index, lastLine );
    
        CPPUNIT_ASSERT_EQUAL( 0, handle.getRateBP().size() );
        CPPUNIT_ASSERT_EQUAL( 0, handle.getDepthBP().size() );
    }
    
    void testConstructSingerFromTextStream(){
        TextStream stream;
        getSingerStream( stream );
        int index = 101;
        string lastLine = "";
        Handle handle( stream, index, lastLine );
        CPPUNIT_ASSERT_EQUAL( index, handle.index );
        CPPUNIT_ASSERT_EQUAL( HandleType::SINGER, handle.getHandleType() );
        CPPUNIT_ASSERT_EQUAL( string( "$07010002" ), handle.iconId );
        CPPUNIT_ASSERT_EQUAL( string( "Miku3=God" ), handle.ids );
        CPPUNIT_ASSERT_EQUAL( 2, handle.original );
        CPPUNIT_ASSERT_EQUAL( string( "" ), handle.getCaption() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1, handle.getLength() );
        CPPUNIT_ASSERT_EQUAL( 1, handle.language );
        CPPUNIT_ASSERT_EQUAL( 2, handle.program );
    }
    
    void testConstructAttackFromTextStream(){
        TextStream stream;
        getAttackStream( stream );
        string lastLine = "";
        int index = 204;
        Handle handle( stream, index, lastLine );
        CPPUNIT_ASSERT_EQUAL( HandleType::NOTE_HEAD, handle.getHandleType() );
        CPPUNIT_ASSERT_EQUAL( index, handle.index );
        CPPUNIT_ASSERT_EQUAL( string( "$01010002" ), handle.iconId );
        CPPUNIT_ASSERT_EQUAL( string( "accent" ), handle.ids );
        CPPUNIT_ASSERT_EQUAL( 2, handle.original );
        CPPUNIT_ASSERT_EQUAL( string( "Accent" ), handle.getCaption() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)120, handle.getLength() );
        CPPUNIT_ASSERT_EQUAL( 64, handle.getDuration() );
        CPPUNIT_ASSERT_EQUAL( 63, handle.getDepth() );
    }
    
    void testConstructCrescendFromTextStream()
    {
        TextStream stream;
        getCrescendoStream( stream );
        string lastLine;
        int index = 204;
        Handle handle( stream, index, lastLine );
        CPPUNIT_ASSERT_EQUAL( index, handle.index );
        CPPUNIT_ASSERT_EQUAL( HandleType::DYNAMICS, handle.getHandleType() );
        CPPUNIT_ASSERT_EQUAL( string( "$05020001" ), handle.iconId );
        CPPUNIT_ASSERT_EQUAL( string( "Crescendo" ), handle.ids );
        CPPUNIT_ASSERT_EQUAL( 4, handle.original );
        CPPUNIT_ASSERT_EQUAL( string( "Zero Crescendo Curve" ), handle.getCaption() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)960, handle.getLength() );
        CPPUNIT_ASSERT_EQUAL( 2, handle.getStartDyn() );
        CPPUNIT_ASSERT_EQUAL( 38, handle.getEndDyn() );
        CPPUNIT_ASSERT_EQUAL( string( "0.5=11" ), handle.getDynBP().getData() );
    }
    
    void testGetterAndSetterLength()
    {
        Handle handle( HandleType::VIBRATO );
        tick_t expected = 847;
        CPPUNIT_ASSERT( expected != handle.getLength() );
        handle.setLength( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getLength() );
    }
    
    void testWrite()
    {
        TextStream stream;
        getLyricStream( stream );
        string lastLine;
        Handle handle( stream, 1, lastLine );
        TextStream dest;
        handle.write( dest );
        dest.setPointer( -1 );
        CPPUNIT_ASSERT_EQUAL( string( "[h#0001]" ), dest.readLine() );
        CPPUNIT_ASSERT_EQUAL( string( "L0=\"あ\",\"a\",0.4,0,1" ), dest.readLine() );
        CPPUNIT_ASSERT_EQUAL( string( "L1=\"は\",\"h a\",0.6,64,0,0" ), dest.readLine() );
    }
    
    void testLyricToString()
    {
        TextStream stream;
        getLyricStream( stream );
        string lastLine;
        Handle handle( stream, 1, lastLine );
        string expected =
            "[h#0001]\n"
            "L0=\"あ\",\"a\",0.4,0,1\n"
            "L1=\"は\",\"h a\",0.6,64,0,0";
        CPPUNIT_ASSERT_EQUAL( expected, handle.toString() );
    }
    
    void testVibratoToString()
    {
        TextStream stream;
        getVibratoStream( stream );
        string lastLine;
        Handle handle( stream, 1, lastLine );
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
    
    void testSingerToString()
    {
        TextStream stream;
        getSingerStream( stream );
        string lastLine;
        Handle handle( stream, 2, lastLine );
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
    
    void testAttackToString()
    {
        TextStream stream;
        getAttackStream( stream );
        string lastLine;
        Handle handle( stream, 3, lastLine );
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
    
    void testCrescendoToString()
    {
        TextStream stream;
        getCrescendoStream( stream );
        string lastLine;
        Handle handle( stream, 4, lastLine );
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
    
    void testGetHandleIndexFromString()
    {
        CPPUNIT_ASSERT_EQUAL( 2, HandleStub::getHandleIndexFromString( "h#0002" ) );
    }
    
    void testGetterAndSetterDepth()
    {
        Handle handle( HandleType::NOTE_HEAD );
        int expected = 1234;
        CPPUNIT_ASSERT( expected != handle.getDepth() );
        handle.setDepth( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getDepth() );
    }
    
    void testGetterAndSetterDuration()
    {
        Handle handle( HandleType::NOTE_HEAD );
        int expected = 947;
        CPPUNIT_ASSERT( expected != handle.getDuration() );
        handle.setDuration( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getDuration() );
    }
    
    void testGetDisplayString()
    {
        Handle handle( HandleType::NOTE_HEAD );
        handle.ids = "goo";
        handle.setCaption( "gle" );
        CPPUNIT_ASSERT_EQUAL( string( "google" ), handle.getDisplayString() );
    }
    
    void testGetterAndSetterRateBP()
    {
        Handle handle( HandleType::VIBRATO );
        VibratoBPList rateBP( "2", "0.0,1.0", "1,128" );
        CPPUNIT_ASSERT( "0=1,1=128" != handle.getRateBP().getData() );
        handle.setRateBP( rateBP );
        CPPUNIT_ASSERT_EQUAL( string( "0=1,1=128" ), handle.getRateBP().getData() );
    }
    
    void testGetterAndSetterDepthBP()
    {
        Handle handle( HandleType::VIBRATO );
        VibratoBPList depthBP( "2", "0.0,1.0", "1,128" );
        CPPUNIT_ASSERT( "0=1,1=128" != handle.getDepthBP().getData() );
        handle.setDepthBP( depthBP );
        CPPUNIT_ASSERT_EQUAL( string( "0=1,1=128" ), handle.getDepthBP().getData() );
    }
    
    void testGetterAndSetterStartRate()
    {
        Handle handle( HandleType::VIBRATO );
        int expected = 12345;
        CPPUNIT_ASSERT( expected != handle.getStartRate() );
        handle.setStartRate( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getStartRate() );
    }
    
    void testGetterAndSetterStartDepth()
    {
        Handle handle( HandleType::VIBRATO );
        int expected = 12345;
        CPPUNIT_ASSERT( expected != handle.getStartDepth() );
        handle.setStartDepth( expected );
        CPPUNIT_ASSERT_EQUAL( expected, handle.getStartDepth() );
    }
    
    void testGetterAndSetterLyric()
    {
        Handle handle( HandleType::LYRIC );
        handle.setLyricAt( 0, Lyric( "は", "h a" ) );
        Lyric lyric( "ら", "4 a" );
        handle.setLyricAt( 1, lyric );
        CPPUNIT_ASSERT_EQUAL( 2, handle.getLyricCount() );
        CPPUNIT_ASSERT( handle.getLyricAt( 1 ).equals( lyric ) );
    }
    
    void testCloneIconDynamicsHandle()
    {
        Handle handle( HandleType::DYNAMICS );
        handle.iconId = "$05010000";
        handle.ids = "foo";
        handle.original = 1;
        handle.setCaption( "bar" );
        handle.setStartDyn( 2 );
        handle.setEndDyn( 3 );
        handle.setLength( 4 );
        Handle copy = handle;
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
        copy = handle;
        CPPUNIT_ASSERT_EQUAL( string( "0=1,1=64" ), copy.getDynBP().getData() );
    }
    
    void testCloneLyricHandle()
    {
        Handle handle( HandleType::LYRIC );
        handle.setLyricAt( 0, Lyric( "ら", "4 a" ) );
        handle.index = 10;
        Handle copy = handle;
        CPPUNIT_ASSERT_EQUAL( handle.index, copy.index );
        Lyric original = handle.getLyricAt( 0 );
        Lyric copied = copy.getLyricAt( 0 );
        CPPUNIT_ASSERT( original.equals( copied ) );
    }
    
    void testCloneNoteHeadHandle()
    {
        Handle handle( HandleType::NOTE_HEAD );
        handle.index = 1;
        handle.iconId = "$05010000";
        handle.ids = "dwango";
        handle.original = 2;
        handle.setCaption( "niwango" );
        handle.setLength( 3 );
        handle.setDuration( 4 );
        handle.setDepth( 5 );
    
        Handle copy = handle;
        CPPUNIT_ASSERT_EQUAL( 1, copy.index );
        CPPUNIT_ASSERT_EQUAL( string( "$05010000" ), copy.iconId );
        CPPUNIT_ASSERT_EQUAL( string( "dwango" ), copy.ids );
        CPPUNIT_ASSERT_EQUAL( 2, copy.original );
        CPPUNIT_ASSERT_EQUAL( string( "niwango" ), copy.getCaption() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)3, copy.getLength() );
        CPPUNIT_ASSERT_EQUAL( 4, copy.getDuration() );
        CPPUNIT_ASSERT_EQUAL( 5, copy.getDepth() );
    }
    
    void testCloneVibratoHandle()
    {
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
        Handle copy = handle;
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
    
    void testCloneSingerHandle()
    {
        Handle handle( HandleType::SINGER );
        handle.setCaption( "bar" );
        handle.iconId = "$07010001";
        handle.ids = "foo";
        handle.index = 1;
        handle.setLength( 2 );
        handle.original = 3;
        handle.program = 4;
        handle.language= 5;
    
        Handle copy = handle;
        CPPUNIT_ASSERT_EQUAL( handle.getCaption(), copy.getCaption() );
        CPPUNIT_ASSERT_EQUAL( handle.iconId, copy.iconId );
        CPPUNIT_ASSERT_EQUAL( handle.ids, copy.ids );
        CPPUNIT_ASSERT_EQUAL( handle.index, copy.index );
        CPPUNIT_ASSERT_EQUAL( handle.language, copy.language );
        CPPUNIT_ASSERT_EQUAL( handle.getLength(), copy.getLength() );
        CPPUNIT_ASSERT_EQUAL( handle.original, copy.original );
        CPPUNIT_ASSERT_EQUAL( handle.program, copy.program );
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
    CPPUNIT_TEST( testConstructLyricFromTextStreamStopWithEOF );
    CPPUNIT_TEST( testConstructLyricFromTextStreamStopWithNextHandle );
    CPPUNIT_TEST( testConstructVibratoFromTextStream );
    CPPUNIT_TEST( testConstructVibratoFromTextStreamWithoutBP );
    CPPUNIT_TEST( testConstructSingerFromTextStream );
    CPPUNIT_TEST( testConstructAttackFromTextStream );
    CPPUNIT_TEST( testConstructCrescendFromTextStream );
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
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( HandleTest );

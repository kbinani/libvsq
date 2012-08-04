#include "Util.hpp"
#include "../Sequence.hpp"
#include "../ByteArrayOutputStream.hpp"

using namespace std;
using namespace VSQ_NS;

class SequenceStub : public Sequence{
public:
    explicit SequenceStub( const string &singer, int preMeasure, int numerator, int denominator, int tempo )
        : Sequence( singer, preMeasure, numerator, denominator, tempo )
    {
    }

    static vector<MidiEvent> getMidiEventsFromMetaText( TextStream *stream, const string &encoding ){
        return _getMidiEventsFromMetaText( stream, encoding );
    }

    static vector<int> getLinePrefixBytes( int count ){
        return _getLinePrefixBytes( count );
    }

    static int getHowManyDigits( int number ){
        return _getHowManyDigits( number );
    }

    static void writeUnsignedShort( OutputStream *stream, int data ){
        _writeUnsignedShort( stream, data );
    }

    static void writeUnsignedInt( OutputStream *stream, int data ){
        _writeUnsignedInt( stream, data );
    }
};

class SequenceTest : public CppUnit::TestCase{
public:
    /**
     * 指定されたシーケンスがデフォルトのシーケンスと等しいかどうかを検査する
     */
    void isEqualToDefaultSequence( const Sequence &sequence ){
        CPPUNIT_ASSERT_EQUAL( (tick_t)(1 * 480 * 4 / 4 * 4), sequence.getTotalClocks() );

        // track
        CPPUNIT_ASSERT_EQUAL( (size_t)2, sequence.track.size() );
        // 第1トラック, master track
        Track track0 = sequence.track[0];
        /*assert_nil( track0.common );
        assert_nil( track0:getCurve( "pit" ) );
        assert_nil( track0:getCurve( "pbs" ) );
        assert_nil( track0:getCurve( "dyn" ) );
        assert_nil( track0:getCurve( "bre" ) );
        assert_nil( track0:getCurve( "bri" ) );
        assert_nil( track0:getCurve( "cle" ) );
        assert_nil( track0:getCurve( "reso1Freq" ) );
        assert_nil( track0:getCurve( "reso2Freq" ) );
        assert_nil( track0:getCurve( "reso3Freq" ) );
        assert_nil( track0:getCurve( "reso4Freq" ) );
        assert_nil( track0:getCurve( "reso1BW" ) );
        assert_nil( track0:getCurve( "reso2BW" ) );
        assert_nil( track0:getCurve( "reso3BW" ) );
        assert_nil( track0:getCurve( "reso4BW" ) );
        assert_nil( track0:getCurve( "reso1Amp" ) );
        assert_nil( track0:getCurve( "reso2Amp" ) );
        assert_nil( track0:getCurve( "reso3Amp" ) );
        assert_nil( track0:getCurve( "reso4Amp" ) );
        assert_nil( track0:getCurve( "harmonics" ) );
        assert_nil( track0:getCurve( "fx2depth" ) );
        assert_nil( track0:getCurve( "gen" ) );
        assert_nil( track0:getCurve( "por" ) );
        assert_nil( track0:getCurve( "ope" ) );
        assert_nil( track0:getCurve( "pitch" ) );
        assert_nil( track0.events );*/
        // 第2トラック, 普通のトラック
        Track track1 = sequence.track[1];
        /*assert_not_nil( track1.common );
        assert_not_nil( track1:getCurve( "pit" ) );
        assert_not_nil( track1:getCurve( "pbs" ) );
        assert_not_nil( track1:getCurve( "dyn" ) );
        assert_not_nil( track1:getCurve( "bre" ) );
        assert_not_nil( track1:getCurve( "bri" ) );
        assert_not_nil( track1:getCurve( "cle" ) );
        assert_not_nil( track1:getCurve( "reso1Freq" ) );
        assert_not_nil( track1:getCurve( "reso2Freq" ) );
        assert_not_nil( track1:getCurve( "reso3Freq" ) );
        assert_not_nil( track1:getCurve( "reso4Freq" ) );
        assert_not_nil( track1:getCurve( "reso1BW" ) );
        assert_not_nil( track1:getCurve( "reso2BW" ) );
        assert_not_nil( track1:getCurve( "reso3BW" ) );
        assert_not_nil( track1:getCurve( "reso4BW" ) );
        assert_not_nil( track1:getCurve( "reso1Amp" ) );
        assert_not_nil( track1:getCurve( "reso2Amp" ) );
        assert_not_nil( track1:getCurve( "reso3Amp" ) );
        assert_not_nil( track1:getCurve( "reso4Amp" ) );
        assert_not_nil( track1:getCurve( "harmonics" ) );
        assert_not_nil( track1:getCurve( "fx2depth" ) );
        assert_not_nil( track1:getCurve( "gen" ) );
        assert_not_nil( track1:getCurve( "por" ) );
        assert_not_nil( track1:getCurve( "ope" ) );
        assert_not_nil( track1:getCurve( "pitch" ) );
        assert_not_nil( track1.events );*/
        CPPUNIT_ASSERT_EQUAL( 1, track1.getEvents()->size() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, track1.getEvents()->get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( EventType::SINGER, track1.getEvents()->get( 0 ).type );

        // master
        CPPUNIT_ASSERT_EQUAL( 1, sequence.master.preMeasure );

        // mixer
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.masterFeder );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.masterMute );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.masterPanpot );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.outputMode );
        // mixer.slave
        CPPUNIT_ASSERT_EQUAL( (size_t)1, sequence.mixer.slave.size() );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.slave[0].feder );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.slave[0].panpot );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.slave[0].mute );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.slave[0].solo );

        // timesigTable
        CPPUNIT_ASSERT_EQUAL( 1, sequence.timesigList.size() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, sequence.timesigList.get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( 4, sequence.timesigList.get( 0 ).denominator );
        CPPUNIT_ASSERT_EQUAL( 4, sequence.timesigList.get( 0 ).numerator );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.timesigList.get( 0 ).barCount );

        // tempoTable
        CPPUNIT_ASSERT_EQUAL( 1, sequence.tempoList.size() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, sequence.tempoList.get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( 500000, sequence.tempoList.get( 0 ).tempo );
        CPPUNIT_ASSERT_EQUAL( 0.0, sequence.tempoList.get( 0 ).getTime() );
    }

    void testConstruct(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        isEqualToDefaultSequence( sequence );
    }

    void testClone(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        Sequence copy = sequence.clone();
        isEqualToDefaultSequence( copy );
    }

    void testGetBaseTempo(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        CPPUNIT_ASSERT_EQUAL( 500000, sequence.getBaseTempo() );
    }

    void testGetPreMeasure(){
        int preMeasure = 1;
        Sequence sequence( "Miku", preMeasure, 4, 4, 500000 );
        CPPUNIT_ASSERT_EQUAL( preMeasure, sequence.getPreMeasure() );
    }

    void testGetPreMeasureClocks(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1920, sequence.getPreMeasureClocks() );
    }

    void testGetTickPerQuarter(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, sequence.getTickPerQuarter() );
    }

    void testUpdateTotalClocks(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        CPPUNIT_ASSERT_EQUAL( (tick_t)(1 * 480 * 4 / 4 * 4), sequence.getTotalClocks() );
        Event note( 1920, EventType::NOTE );
        note.setLength( 480 );
        note.note = 60;
        sequence.track[1].getEvents()->add( note );
        sequence.updateTotalClocks();
        CPPUNIT_ASSERT_EQUAL( (tick_t)2400, sequence.getTotalClocks() );
    }

    void test_getMidiEventsFromMetaText(){
        TextStream stream;
        // 「あ」が Shift_JIS になった時分割される「あ」を Shift_JIS にすると「0x82 0xA0」
        stream.write( StringUtil::repeat( "a", 118 ) + "あ" );
        stream.write( StringUtil::repeat( "b", 63 ) );
        vector<MidiEvent> events = SequenceStub::getMidiEventsFromMetaText( &stream, "Shift_JIS" );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, events.size() );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, events[0].clock );
        CPPUNIT_ASSERT_EQUAL( 0xFF, events[0].firstByte );
        CPPUNIT_ASSERT_EQUAL( (size_t)128, events[0].data.size() );
        CPPUNIT_ASSERT_EQUAL( 0x01, events[0].data[0] );
        ostringstream actual;
        for( int i = 1; i < events[0].data.size(); i++ ){
            actual << (char)events[0].data[i];
        }
        CPPUNIT_ASSERT_EQUAL( string( "DM:0000:" ) + StringUtil::repeat( "a", 118 ) + (char)0x82, actual.str() );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, events[1].clock );
        CPPUNIT_ASSERT_EQUAL( 0xFF, events[1].firstByte );
        CPPUNIT_ASSERT_EQUAL( (size_t)73, events[1].data.size() );
        CPPUNIT_ASSERT_EQUAL( 0x01, events[1].data[0] );
        actual.str( "" );
        for( int i = 1; i < events[1].data.size(); i++ ){
            actual << (char)events[1].data[i];
        }
        CPPUNIT_ASSERT_EQUAL( string( "DM:0001:" ) + (char)0xA0 + StringUtil::repeat( "b", 63 ), actual.str() );
    }

    void testGetMaximumNoteLengthAt(){
    //    fail();
    }

    /**
     * @todo
     */
    void testWriteWithoutPitch(){
        /*
        Sequence sequence( "Foo", 1, 4, 4, 500000 );
        const int CURVE_COUNT = 19;
        string curveNames[CURVE_COUNT] = { "BRE", "BRI", "CLE", "POR", "GEN", "harmonics", "OPE",
                       "reso1amp", "reso1bw", "reso1freq",
                       "reso2amp", "reso2bw", "reso2freq",
                       "reso3amp", "reso3bw", "reso3freq",
                       "reso4amp", "reso4bw", "reso4freq" };
        for( int i = 0; i < CURVE_COUNT; i++ ){
            BPList *list = sequence.track[1].getCurve( curveName[i] );
            list->add( 1920, 0 + i );
        }
        Event noteEvent( 1920, EventType::NOTE );
        sequence.track[1].getEvents()->add( noteEvent );

        ByteArrayOutputStream stream;
        Sequence::_WRITE_NRPN = true;
        sequence:write( stream, 500, "Shift_JIS" );
        stream:close();

        // 期待値と比較する
        string actual = stream.toString();
        local fileHandle = io.open( "./expected/expected.vsq", "rb" );
        local expected = fileHandle:read( "*a" );

        CPPUNIT_ASSERT_EQUAL( expected, actual );
        */
    }

    /**
     * @todo
     */
    void testWriteWithPitch(){
    //    fail();
    }

    /**
     * @todo
     */
    void testGenerateFx2DepthNRPN(){
    //    fail();
    }

    /**
     * @todo
     */
    void testGenerateNRPNAll(){
    //    fail();
    }

    /**
     * @todo
     */
    void testGenerateNRPNPartial(){
    //    fail();
    }

    void test_getLinePrefixBytes(){
        // 4 桁
        vector<int> expected;
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '1' );
        expected.push_back( 0xff & '2' );
        expected.push_back( 0xff & '3' );
        expected.push_back( 0xff & ':' );
        vector<int> actual = SequenceStub::getLinePrefixBytes( 123 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }

        // 4 桁
        expected.clear();
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & ':' );
        actual = SequenceStub::getLinePrefixBytes( 9999 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }

        // 8 桁
        expected.clear();
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '1' );
        expected.push_back( 0xff & '2' );
        expected.push_back( 0xff & '3' );
        expected.push_back( 0xff & '4' );
        expected.push_back( 0xff & '5' );
        expected.push_back( 0xff & ':' );
        actual = SequenceStub::getLinePrefixBytes( 12345 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }

        // 8 桁
        expected.clear();
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '1' );
        expected.push_back( 0xff & '2' );
        expected.push_back( 0xff & '3' );
        expected.push_back( 0xff & '4' );
        expected.push_back( 0xff & '5' );
        expected.push_back( 0xff & '6' );
        expected.push_back( 0xff & '7' );
        expected.push_back( 0xff & ':' );
        actual = SequenceStub::getLinePrefixBytes( 1234567 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }
    }

    void test_getHowManyDigits(){
        CPPUNIT_ASSERT_EQUAL( 1, SequenceStub::getHowManyDigits( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 1, SequenceStub::getHowManyDigits( 9 ) );
        CPPUNIT_ASSERT_EQUAL( 2, SequenceStub::getHowManyDigits( 99 ) );
        CPPUNIT_ASSERT_EQUAL( 10, SequenceStub::getHowManyDigits( 1000000000 ) );
        CPPUNIT_ASSERT_EQUAL( 2, SequenceStub::getHowManyDigits( -10 ) );
    }

    void test_writeUnsignedShort(){
        ByteArrayOutputStream stream;
        SequenceStub::writeUnsignedShort( &stream, 0x8421 );
        string actual = stream.toString();
        ostringstream expected;
        expected << (char)0x84 << (char)0x21;
        CPPUNIT_ASSERT_EQUAL( expected.str(), actual );
    }

    void test_writeUnsignedInt(){
        ByteArrayOutputStream stream;
        SequenceStub::writeUnsignedInt( &stream, 0x84212184 );
        string actual = stream.toString();
        ostringstream expected;
        expected << (char)0x84 << (char)0x21 << (char)0x21 << (char)0x84;
        CPPUNIT_ASSERT_EQUAL( expected.str(), actual );
    }

    CPPUNIT_TEST_SUITE( SequenceTest );
    CPPUNIT_TEST( testConstruct );
    CPPUNIT_TEST( testClone );
    CPPUNIT_TEST( testGetBaseTempo );
    CPPUNIT_TEST( testGetPreMeasure );
    CPPUNIT_TEST( testGetPreMeasureClocks );
    CPPUNIT_TEST( testGetTickPerQuarter );
    CPPUNIT_TEST( testUpdateTotalClocks );
    CPPUNIT_TEST( test_getMidiEventsFromMetaText );
    CPPUNIT_TEST( testGetMaximumNoteLengthAt );
    CPPUNIT_TEST( testWriteWithoutPitch );
    CPPUNIT_TEST( testWriteWithPitch );
    CPPUNIT_TEST( testGenerateFx2DepthNRPN );
    CPPUNIT_TEST( testGenerateNRPNAll );
    CPPUNIT_TEST( testGenerateNRPNPartial );
    CPPUNIT_TEST( test_getLinePrefixBytes );
    CPPUNIT_TEST( test_getHowManyDigits );
    CPPUNIT_TEST( test_writeUnsignedShort );
    CPPUNIT_TEST( test_writeUnsignedInt );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( SequenceTest );

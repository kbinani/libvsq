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

    void getActualClockAndDelay( tick_t clock, int msPreSend, tick_t *actualClock, int *delay ){
        _getActualClockAndDelay( clock, msPreSend, actualClock, delay );
    }

    static vector<MidiEvent> getMidiEventsFromMetaText( TextStream *stream, const string &encoding ){
        return _getMidiEventsFromMetaText( stream, encoding );
    }

    static void getMsbAndLsb( int value, int *msb, int *lsb ){
        _getMsbAndLsb( value, msb, lsb );
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
        {//TODO:
            cout << endl << "SequenceTest::testClone; before isEqualToDefaultSequence" << endl;
        }
        isEqualToDefaultSequence( copy );
        {//TODO:
            cout << endl << "SequenceTest::testClone; after isEqualToDefaultSequence" << endl;
        }
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

    void test_getActualClockAndDelay(){
        SequenceStub sequence( "Miku", 1, 4, 4, 500000 );
        tick_t actualClock;
        int delay;

        sequence.getActualClockAndDelay( 1920, 500, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actualClock );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        sequence.getActualClockAndDelay( 1920, 499, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actualClock );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        sequence.getActualClockAndDelay( 1920, 498, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1441, actualClock );
        CPPUNIT_ASSERT_EQUAL( 498, delay );

        sequence.getActualClockAndDelay( 0, 500, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actualClock );
        CPPUNIT_ASSERT_EQUAL( 0, delay );

        sequence.getActualClockAndDelay( 0, 0, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actualClock );
        CPPUNIT_ASSERT_EQUAL( 0, delay );
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
    void test_generateExpressionNRPN(){
        /*
        local sequence = luavsq.Sequence.new( "Miku", 1, 4, 4, 500000 );
        local dynamics = sequence.track:get( 1 ):getCurve( "DYN" );
        dynamics:add( 480, 127 );
        dynamics:add( 1920, 0 );

        local actual = luavsq.Sequence._generateExpressionNRPN( sequence, 1, 500 );
        CPPUNIT_ASSERT_EQUAL( 3, #actual );

        CPPUNIT_ASSERT_EQUAL( 0, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_E_DELAY, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[1].dataLSB );
        CPPUNIT_ASSERT( actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( 0, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_E_EXPRESSION, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 127, actual[2].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( 1440, actual[3].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_E_EXPRESSION, actual[3].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[3].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[3].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[3].isMSBOmittingRequired );
        */
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
    void test_generateHeaderNRPN(){
        /*
        local actual = luavsq.Sequence._generateHeaderNRPN():expand();
        CPPUNIT_ASSERT_EQUAL( 3, #actual );

        CPPUNIT_ASSERT_EQUAL( 0, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_BS_VERSION_AND_DEVICE, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[1].dataLSB );
        CPPUNIT_ASSERT( actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( 0, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_BS_DELAY, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[2].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[2].dataLSB );
        CPPUNIT_ASSERT( actual[2].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( 0, actual[3].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_BS_LANGUAGE_TYPE, actual[3].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[3].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[3].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[3].isMSBOmittingRequired );
        */
    }

    /**
     * @todo
     */
    void test_generateSingerNRPN(){
        /*
        local sequence = luavsq.Sequence.new( "Miku", 1, 4, 4, 500000 );
        local singerEvent = luavsq.Event.new( 1920, luavsq.EventTypeEnum.SINGER );
        singerEvent.singerHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.SINGER );
        sequence.track:get( 1 ).events:add( singerEvent );
        local actual = luavsq.Sequence._generateSingerNRPN( sequence, singerEvent, 500 );

        CPPUNIT_ASSERT_EQUAL( 1, #actual );
        local actualExpanded = actual[1]:expand();

        CPPUNIT_ASSERT_EQUAL( 4, #actualExpanded );
        local item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_BS_VERSION_AND_DEVICE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = actualExpanded[2];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_BS_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );

        item = actualExpanded[3];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_BS_LANGUAGE_TYPE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( singerEvent.singerHandle.language, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );

        item = actualExpanded[4];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.PC_VOICE_TYPE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( singerEvent.singerHandle.program, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        */
    }

    /**
     * @todo
     */
    void test_generateNoteNRPN(){
        /*
        local sequence = luavsq.Sequence.new( "Miku", 1, 4, 4, 500000 );
        local noteEvent = luavsq.Event.new( 1920, luavsq.EventTypeEnum.NOTE );
        noteEvent:setLength( 480 );
        noteEvent.note = 60;
        noteEvent.dynamics = 127;
        noteEvent.pmBendDepth = 8;
        noteEvent.pmBendLength = 0;
        noteEvent.d4mean = 63;
        noteEvent.pMeanOnsetFirstNote = 65;
        noteEvent.vMeanNoteTransition = 66;
        noteEvent.pMeanEndingNote = 67;
        noteEvent.pmbPortamentoUse = 3;
        noteEvent.demDecGainRate = 50;
        noteEvent.demAccent = 50;
        noteEvent.vibratoDelay = 240;
        noteEvent.vibratoHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.VIBRATO );
        noteEvent.vibratoHandle:setLength( 240 );
        noteEvent.vibratoHandle.iconId = "$04040005";
        noteEvent.lyricHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.LYRIC );
        noteEvent.lyricHandle:setLyricAt( 0, luavsq.Lyric.new( "あ", "a" ) );
        sequence.track:get( 1 ).common.version = "DSB3";

        // lastDelay が nil であるために、CVM_NM_VERSION_AND_DEVICE が出力されるケース
        local lastDelay = nil;
        local noteLocation = 1;
        local msPreSend = 500;
        local track = 1;
        local actual, delay = luavsq.Sequence._generateNoteNRPN( sequence, track, noteEvent, msPreSend, noteLocation, lastDelay );
        local actualExpanded = actual:expand();
        CPPUNIT_ASSERT_EQUAL( 24, #actualExpanded );
        CPPUNIT_ASSERT_EQUAL( 500, delay );
        local item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_VERSION_AND_DEVICE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[2];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[3];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_NOTE_NUMBER, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 60, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[4];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_VELOCITY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 127, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[5];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_NOTE_DURATION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[6];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_NOTE_LOCATION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 1, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[7];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_INDEX_OF_VIBRATO_DB, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[8];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_VIBRATO_CONFIG, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 5, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 63, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[9];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_VIBRATO_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 64, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[10];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_PHONETIC_SYMBOL_BYTES, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 1, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[11];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( 0x5013, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( string.byte( "a" ), item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[12];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_PHONETIC_SYMBOL_CONTINUATION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x7f, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[13];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_V1MEAN, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 4, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[14];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_D1MEAN, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 8, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[15];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_D1MEAN_FIRST_NOTE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x14, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[16];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_D2MEAN, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 28, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[17];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_D4MEAN, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 63, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[18];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_PMEAN_ONSET_FIRST_NOTE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 65, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[19];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_VMEAN_NOTE_TRNSITION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 66, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[20];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_PMEAN_ENDING_NOTE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 67, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[21];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_ADD_PORTAMENTO, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 3, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[22];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_CHANGE_AFTER_PEAK, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 50, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[23];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_ACCENT, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 50, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[24];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CVM_NM_NOTE_MESSAGE_CONTINUATION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x7f, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );

        //lastDelay が nil でないために、CVM_NM_VERSION_AND_DEVICE が出力されないパターン
        lastDelay = 0;
        actual, delay = luavsq.Sequence._generateNoteNRPN( sequence, track, noteEvent, msPreSend, noteLocation, lastDelay );
        actualExpanded = actual:expand();
        CPPUNIT_ASSERT_EQUAL( 23, #actualExpanded );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        // lastDelay が、該当音符についての delay と同じであるために、CVM_NM_DELAY が出力されないパターン
        lastDelay = 500;
        actual, delay = luavsq.Sequence._generateNoteNRPN( sequence, track, noteEvent, msPreSend, noteLocation, lastDelay );
        actualExpanded = actual:expand();
        CPPUNIT_ASSERT_EQUAL( 22, #actualExpanded );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        // vibratoHandle が nil であるために、CVM_NM_INDEX_OF_VIBRATO_DB, CVM_NM_VIBRATO_CONFIG, CVM_NM_VIBRATO_DELAY
        // が出力されないパターン
        lastDelay = 500;
        noteEvent.vibratoHandle = nil;
        actual, delay = luavsq.Sequence._generateNoteNRPN( sequence, track, noteEvent, msPreSend, noteLocation, lastDelay );
        actualExpanded = actual:expand();
        CPPUNIT_ASSERT_EQUAL( 19, #actualExpanded );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        sequence.track:get( 1 ).common.version = "DSB2";
        // VOCALOID1 であるために、0x5011が出力され、CVM_NM_PHONETIC_SYMBOL_CONTINUATIONとVOCALOID2用のNRPNが出力されない
        lastDelay = 500;
        noteEvent.vibratoHandle = nil;
        actual, delay = luavsq.Sequence._generateNoteNRPN( sequence, track, noteEvent, msPreSend, noteLocation, lastDelay );
        actualExpanded = actual:expand();
        CPPUNIT_ASSERT_EQUAL( 8, #actualExpanded );
        item = actualExpanded[5];
        CPPUNIT_ASSERT_EQUAL( 1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( 0x5011, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x01, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        */
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

    /**
     * @todo
     */
    void test_generatePitchBendNRPN(){
        /*
        local sequence = luavsq.Sequence.new( "Miku", 1, 4, 4, 500000 );
        local pit = sequence.track:get( 1 ):getCurve( "PIT" );
        pit:add( 480, 8191 );
        pit:add( 1920, -8192 );

        local actual = luavsq.Sequence._generatePitchBendNRPN( sequence, 1, 500 );
        CPPUNIT_ASSERT_EQUAL( 3, #actual );

        CPPUNIT_ASSERT_EQUAL( 0, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.PB_DELAY, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[1].dataLSB );
        CPPUNIT_ASSERT( actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( 0, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.PB_PITCH_BEND, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x7F, actual[2].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x7F, actual[2].dataLSB );
        CPPUNIT_ASSERT( actual[2].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( 1440, actual[3].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.PB_PITCH_BEND, actual[3].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[3].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[3].dataLSB );
        CPPUNIT_ASSERT( actual[3].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[3].isMSBOmittingRequired );
        */
    }

    /**
     * @todo
     */
    void test_generatePitchBendSensitivityNRPN(){
        /*
        local sequence = luavsq.Sequence.new( "Miku", 1, 4, 4, 500000 );
        local pbs = sequence.track:get( 1 ):getCurve( "PBS" );
        pbs:add( 480, 0 );
        pbs:add( 1920, 24 );

        local actual = luavsq.Sequence._generatePitchBendSensitivityNRPN( sequence, 1, 500 );
        CPPUNIT_ASSERT_EQUAL( 3, #actual );

        CPPUNIT_ASSERT_EQUAL( 0, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_PBS_DELAY, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[1].dataLSB );
        CPPUNIT_ASSERT( actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( 0, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_PBS_PITCH_BEND_SENSITIVITY, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[2].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[2].dataLSB );
        CPPUNIT_ASSERT( actual[2].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( 1440, actual[3].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_PBS_PITCH_BEND_SENSITIVITY, actual[3].nrpn );
        CPPUNIT_ASSERT_EQUAL( 24, actual[3].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[3].dataLSB );
        CPPUNIT_ASSERT( actual[3].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[3].isMSBOmittingRequired );
        */
    }

    /**
     * @todo
     */
    void test_generateVibratoNRPN(){
        /*
        local sequence = luavsq.Sequence.new( "Miku", 1, 4, 4, 500000 );
        local noteEvent = luavsq.Event.new( 480, luavsq.EventTypeEnum.NOTE );
        noteEvent.vibratoHandle = nil;

        // ビブラートがないため、NRPN が生成されない場合
        local actual = luavsq.Sequence._generateVibratoNRPN( sequence, noteEvent, 500 );
        CPPUNIT_ASSERT_EQUAL( 0, #actual );

        // ビブラートがある場合
        noteEvent.vibratoHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.VIBRATO );
        noteEvent:setLength( 480 );
        noteEvent.vibratoDelay = 240;
        noteEvent.vibratoHandle:setLength( 240 );
        noteEvent.vibratoHandle:setStartDepth( 71 );
        noteEvent.vibratoHandle:setStartRate( 72 );
        local rateCurve = luavsq.VibratoBPList.new( { 0.5, 1.0 }, { 11, 12 } );
        local depthCurve = luavsq.VibratoBPList.new( { 0.4, 0.9 }, { 13, 14 } );
        noteEvent.vibratoHandle:setRateBP( rateCurve );
        noteEvent.vibratoHandle:setDepthBP( depthCurve );
        actual = luavsq.Sequence._generateVibratoNRPN( sequence, noteEvent, 500 );
        CPPUNIT_ASSERT_EQUAL( 5, #actual );

        local actualExpanded = actual[1]:expand();
        CPPUNIT_ASSERT_EQUAL( 6, #actualExpanded );
        local item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( 240, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VD_VERSION_AND_DEVICE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[2];
        CPPUNIT_ASSERT_EQUAL( 240, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VR_VERSION_AND_DEVICE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[3];
        CPPUNIT_ASSERT_EQUAL( 240, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VD_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[4];
        CPPUNIT_ASSERT_EQUAL( 240, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VR_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[5];
        CPPUNIT_ASSERT_EQUAL( 240, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VD_VIBRATO_DEPTH, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 71, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[6];
        CPPUNIT_ASSERT_EQUAL( 240, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VR_VIBRATO_RATE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 72, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        actualExpanded = actual[2]:expand();
        CPPUNIT_ASSERT_EQUAL( 2, #actualExpanded );
        item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( 336, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VD_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[2];
        CPPUNIT_ASSERT_EQUAL( 336, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VD_VIBRATO_DEPTH, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 13, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = actual[3];
        actualExpanded = item:expand();
        CPPUNIT_ASSERT_EQUAL( 2, #actualExpanded );
        item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( 360, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VR_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[2];
        CPPUNIT_ASSERT_EQUAL( 360, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VR_VIBRATO_RATE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 11, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = actual[4];
        CPPUNIT_ASSERT_EQUAL( 456, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VD_VIBRATO_DEPTH, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 14, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = actual[5];
        CPPUNIT_ASSERT_EQUAL( 480, item.clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.CC_VR_VIBRATO_RATE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 12, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        */
    }

    /**
     * @todo
     */
    void testGenerateVoiceChangeParameterNRPN(){
        /*
        local sequence = luavsq.Sequence.new( "Foo", 1, 4, 4, 500000 );

        // 全種類のカーブに、データ点を1個ずつ入れる
        local curveNames = { "BRE", "BRI", "CLE", "POR", "GEN", "harmonics", "OPE",
                       "reso1amp", "reso1bw", "reso1freq",
                       "reso2amp", "reso2bw", "reso2freq",
                       "reso3amp", "reso3bw", "reso3freq",
                       "reso4amp", "reso4bw", "reso4freq" };
        for i, curveName in pairs( curveNames ) do
            local list = luavsq.BPList.new( curveName, 0, 0, 127 );
            list:add( 480, 64 );
            sequence.track:get( 1 ):setCurve( curveName, list );
        }

        // VOCALOID1 の場合
        sequence.track:get( 1 ).common.version = "DSB2";
        local events = luavsq.Sequence._generateVoiceChangeParameterNRPN( sequence, 1, 500 );
        // 中身は見ない。各カーブに MIDI イベントが1つずつできることだけをチェック
        // 各イベントの子にあたるイベントのテストは、test_addVoiceChangeParameters で行う
        // vocaloid1 で出力されるのは 18 種類
        CPPUNIT_ASSERT_EQUAL( 18, #events );

        // VOCALOID2 の場合
        // 6 種類
        sequence.track:get( 1 ).common.version = "DSB3";
        local events = luavsq.Sequence._generateVoiceChangeParameterNRPN( sequence, 1, 500 );
        CPPUNIT_ASSERT_EQUAL( 6, #events );

        // UNKNOWN の場合
        // 5 種類
        sequence.track:get( 1 ).common.version = "";
        local events = luavsq.Sequence._generateVoiceChangeParameterNRPN( sequence, 1, 500 );
        CPPUNIT_ASSERT_EQUAL( 5, #events );
        */
    }

    /**
     * @todo
     */
    void test_addVoiceChangeParameters(){
        /*
        local list = luavsq.BPList.new( "BRE", 0, 0, 127 );
        list:add( 480, 0 );
        list:add( 1920, 127 );

        local sequence = luavsq.Sequence.new( "Foo", 1, 4, 4, 500000 );
        sequence.track:get( 1 ):setCurve( "BRE", list );
        local dest = {};
        local preSend = 500;
        local delay = luavsq.Sequence._addVoiceChangeParameters( dest, list, sequence, preSend, 0 );

        CPPUNIT_ASSERT_EQUAL( 2, #dest );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        local actual = dest[1]:expand();
        CPPUNIT_ASSERT_EQUAL( 3, #actual );
        CPPUNIT_ASSERT_EQUAL( 0, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.VCP_DELAY, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[1].dataLSB );
        CPPUNIT_ASSERT( actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );
        CPPUNIT_ASSERT_EQUAL( 0, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.VCP_VOICE_CHANGE_PARAMETER_ID, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x31, actual[2].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].isMSBOmittingRequired );
        CPPUNIT_ASSERT_EQUAL( 0, actual[3].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.VCP_VOICE_CHANGE_PARAMETER, actual[3].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[3].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[3].hasLSB );
        CPPUNIT_ASSERT( actual[3].isMSBOmittingRequired );

        actual = dest[2]:expand();
        CPPUNIT_ASSERT_EQUAL( 1440, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.VCP_VOICE_CHANGE_PARAMETER_ID, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x31, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );
        CPPUNIT_ASSERT_EQUAL( 1440, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.MidiParameterEnum.VCP_VOICE_CHANGE_PARAMETER, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 127, actual[2].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].hasLSB );
        CPPUNIT_ASSERT( actual[2].isMSBOmittingRequired );
        */
    }

    void test_getMsbAndLsb(){
        int msb, lsb;
        SequenceStub::getMsbAndLsb( 264, &msb, &lsb );
        CPPUNIT_ASSERT_EQUAL( 2, msb );
        CPPUNIT_ASSERT_EQUAL( 8, lsb );
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
    CPPUNIT_TEST( test_getActualClockAndDelay );
    CPPUNIT_TEST( testGetMaximumNoteLengthAt );
    CPPUNIT_TEST( testWriteWithoutPitch );
    CPPUNIT_TEST( testWriteWithPitch );
    CPPUNIT_TEST( test_generateExpressionNRPN );
    CPPUNIT_TEST( testGenerateFx2DepthNRPN );
    CPPUNIT_TEST( test_generateHeaderNRPN );
    CPPUNIT_TEST( test_generateSingerNRPN );
    CPPUNIT_TEST( test_generateNoteNRPN );
    CPPUNIT_TEST( testGenerateNRPNAll );
    CPPUNIT_TEST( testGenerateNRPNPartial );
    CPPUNIT_TEST( test_generatePitchBendNRPN );
    CPPUNIT_TEST( test_generatePitchBendSensitivityNRPN );
    CPPUNIT_TEST( test_generateVibratoNRPN );
    CPPUNIT_TEST( testGenerateVoiceChangeParameterNRPN );
    CPPUNIT_TEST( test_addVoiceChangeParameters );
    CPPUNIT_TEST( test_getMsbAndLsb );
    CPPUNIT_TEST( test_getLinePrefixBytes );
    CPPUNIT_TEST( test_getHowManyDigits );
    CPPUNIT_TEST( test_writeUnsignedShort );
    CPPUNIT_TEST( test_writeUnsignedInt );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( SequenceTest );

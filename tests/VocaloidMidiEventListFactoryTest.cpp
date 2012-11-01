#include "Util.hpp"
#include "../VocaloidMidiEventListFactory.hpp"
#include "../Sequence.hpp"

using namespace std;
using namespace VSQ_NS;

class VocaloidMidiEventListFactoryStub : public VocaloidMidiEventListFactory{
public:
    static vector<NrpnEvent> generateNRPN(
        Track *target, TempoList *tempoList, tick_t totalClocks, tick_t preMeasureClock, int msPreSend )
    {
        return VocaloidMidiEventListFactory::generateNRPN( target, tempoList, totalClocks, preMeasureClock, msPreSend );
    }

    static vector<NrpnEvent> generateExpressionNRPN( Track *track, TempoList *tempoList, int preSendMilliseconds ){
        return VocaloidMidiEventListFactory::generateExpressionNRPN( track, tempoList, preSendMilliseconds );
    }

    static NrpnEvent generateHeaderNRPN(){
        return VocaloidMidiEventListFactory::generateHeaderNRPN();
    }

    static vector<NrpnEvent> generateSingerNRPN( TempoList *tempoList, const Event *singerEvent, int preSendMilliseconds ){
        return VocaloidMidiEventListFactory::generateSingerNRPN( tempoList, singerEvent, preSendMilliseconds );
    }

    static NrpnEvent generateNoteNRPN( Track *track, TempoList *tempoList, const Event *noteEvent, int msPreSend, int noteLocation, int *lastDelay, int *delay ){
        return VocaloidMidiEventListFactory::generateNoteNRPN( track, tempoList, noteEvent, msPreSend, noteLocation, lastDelay, delay );
    }

    static vector<NrpnEvent> generatePitchBendNRPN( Track *track, TempoList *tempoList, int msPreSend ){
        return VocaloidMidiEventListFactory::generatePitchBendNRPN( track, tempoList, msPreSend );
    }

    static vector<NrpnEvent> generatePitchBendSensitivityNRPN( Track *track, TempoList *tempoList, int msPreSend ){
        return VocaloidMidiEventListFactory::generatePitchBendSensitivityNRPN( track, tempoList, msPreSend );
    }

    static vector<NrpnEvent> generateVibratoNRPN( TempoList *tempoList, Event *noteEvent, int msPreSend ){
        return VocaloidMidiEventListFactory::generateVibratoNRPN( tempoList, noteEvent, msPreSend );
    }

    static vector<NrpnEvent> generateVoiceChangeParameterNRPN( Track *track, TempoList *tempoList, int msPreSend, tick_t premeasure_clock ){
        return VocaloidMidiEventListFactory::generateVoiceChangeParameterNRPN( track, tempoList, msPreSend, premeasure_clock );
    }

    static vector<NrpnEvent> generateFx2DepthNRPN( Track *track, TempoList *tempoList, int preSendMilliseconds ){
        return VocaloidMidiEventListFactory::generateFx2DepthNRPN( track, tempoList, preSendMilliseconds );
    }

    static int addVoiceChangeParameters( vector<NrpnEvent> &dest, BPList *list, TempoList *tempoList, int msPreSend, int lastDelay ){
        return VocaloidMidiEventListFactory::addVoiceChangeParameters( dest, list, tempoList, msPreSend, lastDelay );
    }

    static void getActualClockAndDelay( TempoList *tempoList, tick_t clock, int msPreSend, tick_t *actualClock, int *delay ){
        _getActualClockAndDelay( tempoList, clock, msPreSend, actualClock, delay );
    }

    static void getMsbAndLsb( int value, int *msb, int *lsb ){
        _getMsbAndLsb( value, msb, lsb );
    }
};

class VocaloidMidiEventListFactoryTest : public CppUnit::TestCase{
public:
    void test_generateExpressionNRPN(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        Track *track = &(sequence.track[0]);
        BPList *dynamics = track->getCurve( "DYN" );
        dynamics->add( 480, 127 );
        dynamics->add( 1920, 0 );

        vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateExpressionNRPN( track, &sequence.tempoList, 500 );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, actual.size() );

        vector<NrpnEvent> expandedActual = actual[0].expand();
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, expandedActual[0].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_E_DELAY, expandedActual[0].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, expandedActual[0].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, expandedActual[0].dataLSB );
        CPPUNIT_ASSERT( expandedActual[0].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, expandedActual[0].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, expandedActual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_E_EXPRESSION, expandedActual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 127, expandedActual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, expandedActual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, expandedActual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_E_EXPRESSION, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );
    }

    void test_generateHeaderNRPN(){
        vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateHeaderNRPN().expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)3, actual.size() );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[0].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_VERSION_AND_DEVICE, actual[0].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[0].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[0].dataLSB );
        CPPUNIT_ASSERT( actual[0].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[0].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_DELAY, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[1].dataLSB );
        CPPUNIT_ASSERT( actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_LANGUAGE_TYPE, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[2].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].isMSBOmittingRequired );
    }

    void testGenerateSingerNRPN(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        Event singerEvent( 1920, EventType::SINGER );
        singerEvent.singerHandle = Handle( HandleType::SINGER );
        sequence.track[0].events()->add( singerEvent );
        vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateSingerNRPN( &sequence.tempoList, &singerEvent, 500 );

        CPPUNIT_ASSERT_EQUAL( (size_t)1, actual.size() );
        vector<NrpnEvent> actualExpanded = actual[0].expand();

        CPPUNIT_ASSERT_EQUAL( (size_t)4, actualExpanded.size() );
        NrpnEvent item = actualExpanded[0];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_VERSION_AND_DEVICE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );

        item = actualExpanded[2];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_LANGUAGE_TYPE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( singerEvent.singerHandle.language, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );

        item = actualExpanded[3];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::PC_VOICE_TYPE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( singerEvent.singerHandle.program, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
    }

    void testGenerateNoteNRPN(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        Event noteEvent( 1920, EventType::NOTE );
        noteEvent.setLength( 480 );
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
        noteEvent.vibratoHandle = Handle( HandleType::VIBRATO );
        noteEvent.vibratoHandle.setLength( 240 );
        noteEvent.vibratoHandle.iconId = "$04040005";
        noteEvent.lyricHandle = Handle( HandleType::LYRIC );
        noteEvent.lyricHandle.setLyricAt( 0, Lyric( "あ", "a" ) );
        sequence.track[0].getCommon()->version = "DSB3";

        // lastDelay が nil であるために、CVM_NM_VERSION_AND_DEVICE が出力されるケース
        int lastDelay;// = nil;
        int noteLocation = 1;
        int msPreSend = 500;
        int track = 0;
        int delay;
        NrpnEvent actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN( &sequence.track[0], &sequence.tempoList, &noteEvent, msPreSend, noteLocation, (int *)0, &delay );
        vector<NrpnEvent> actualExpanded = actual.expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)24, actualExpanded.size() );
        CPPUNIT_ASSERT_EQUAL( 500, delay );
        NrpnEvent item = actualExpanded[0];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_VERSION_AND_DEVICE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[2];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_NOTE_NUMBER, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 60, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[3];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_VELOCITY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 127, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[4];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_NOTE_DURATION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[5];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_NOTE_LOCATION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 1, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[6];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_INDEX_OF_VIBRATO_DB, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[7];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_VIBRATO_CONFIG, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 5, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 63, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[8];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_VIBRATO_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 64, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[9];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_PHONETIC_SYMBOL_BYTES, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 1, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[10];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( (MidiParameterType::MidiParameterTypeEnum)0x5013, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( (int)'a', item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[11];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_PHONETIC_SYMBOL_CONTINUATION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x7f, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[12];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_V1MEAN, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 4, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[13];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_D1MEAN, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 8, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[14];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_D1MEAN_FIRST_NOTE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x14, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[15];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_D2MEAN, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 28, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[16];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_D4MEAN, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 63, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[17];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_PMEAN_ONSET_FIRST_NOTE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 65, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[18];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_VMEAN_NOTE_TRNSITION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 66, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[19];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_PMEAN_ENDING_NOTE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 67, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[20];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_ADD_PORTAMENTO, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 3, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[21];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_CHANGE_AFTER_PEAK, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 50, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[22];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_ACCENT, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 50, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
        item = actualExpanded[23];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x7f, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );

        // lastDelay が nil でないために、CVM_NM_VERSION_AND_DEVICE が出力されないパターン
        lastDelay = 0;
        actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN( &sequence.track[track], &sequence.tempoList, &noteEvent, msPreSend, noteLocation, &lastDelay, &delay );
        actualExpanded = actual.expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)23, actualExpanded.size() );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        // lastDelay が、該当音符についての delay と同じであるために、CVM_NM_DELAY が出力されないパターン
        lastDelay = 500;
        actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN( &sequence.track[track], &sequence.tempoList, &noteEvent, msPreSend, noteLocation, &lastDelay, &delay );
        actualExpanded = actual.expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)22, actualExpanded.size() );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        // vibratoHandle が nil であるために、CVM_NM_INDEX_OF_VIBRATO_DB, CVM_NM_VIBRATO_CONFIG, CVM_NM_VIBRATO_DELAY
        // が出力されないパターン
        lastDelay = 500;
        noteEvent.vibratoHandle = Handle();
        actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN( &sequence.track[track], &sequence.tempoList, &noteEvent, msPreSend, noteLocation, &lastDelay, &delay );
        actualExpanded = actual.expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)19, actualExpanded.size() );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        sequence.track[0].getCommon()->version = "DSB2";
        // VOCALOID1 であるために、0x5011が出力され、CVM_NM_PHONETIC_SYMBOL_CONTINUATIONとVOCALOID2用のNRPNが出力されない
        lastDelay = 500;
        noteEvent.vibratoHandle = Handle();
        actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN( &sequence.track[track], &sequence.tempoList, &noteEvent, msPreSend, noteLocation, &lastDelay, &delay );
        actualExpanded = actual.expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)8, actualExpanded.size() );
        item = actualExpanded[4];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( (MidiParameterType::MidiParameterTypeEnum)0x5011, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x01, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT( item.isMSBOmittingRequired );
    }

    void testGeneratePitchBendNRPN(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        BPList *pit = sequence.track[0].getCurve( "PIT" );
        pit->add( 480, 8191 );
        pit->add( 1920, -8192 );

        vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generatePitchBendNRPN( &sequence.track[0], &sequence.tempoList, 500 );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, actual.size() );

        vector<NrpnEvent> expandedActual = actual[0].expand();
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, expandedActual[0].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::PB_DELAY, expandedActual[0].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, expandedActual[0].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, expandedActual[0].dataLSB );
        CPPUNIT_ASSERT( expandedActual[0].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, expandedActual[0].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, expandedActual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::PB_PITCH_BEND, expandedActual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x7F, expandedActual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x7F, expandedActual[1].dataLSB );
        CPPUNIT_ASSERT( expandedActual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, expandedActual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::PB_PITCH_BEND, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[1].dataLSB );
        CPPUNIT_ASSERT( actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );
    }

    void testGeneratePitchBendSensitivityNRPN(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        BPList *pbs = sequence.track[0].getCurve( "PBS" );
        pbs->add( 480, 0 );
        pbs->add( 1920, 24 );

        vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generatePitchBendSensitivityNRPN( &sequence.track[0], &sequence.tempoList, 500 );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, actual.size() );

        vector<NrpnEvent> expandedActual = actual[0].expand();
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, expandedActual[0].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_PBS_DELAY, expandedActual[0].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, expandedActual[0].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, expandedActual[0].dataLSB );
        CPPUNIT_ASSERT( expandedActual[0].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, expandedActual[0].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, expandedActual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY, expandedActual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, expandedActual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, expandedActual[1].dataLSB );
        CPPUNIT_ASSERT( expandedActual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, expandedActual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 24, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[1].dataLSB );
        CPPUNIT_ASSERT( actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );
    }

    void testGenerateVibratoNRPN(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        Event noteEvent( 480, EventType::NOTE );
        noteEvent.vibratoHandle = Handle();

        // ビブラートがないため、NRPN が生成されない場合
        vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateVibratoNRPN( &sequence.tempoList, &noteEvent, 500 );
        CPPUNIT_ASSERT_EQUAL( (size_t)0, actual.size() );

        // ビブラートがある場合
        noteEvent.vibratoHandle = Handle( HandleType::VIBRATO );
        noteEvent.setLength( 480 );
        noteEvent.vibratoDelay = 240;
        noteEvent.vibratoHandle.setLength( 240 );
        noteEvent.vibratoHandle.startDepth = 71;
        noteEvent.vibratoHandle.startRate = 72;
        VibratoBPList rateCurve( "2", "0.5,1.0", "11,12" );
        VibratoBPList depthCurve( "2", "0.4,0.9", "13,14" );
        noteEvent.vibratoHandle.rateBP = rateCurve;
        noteEvent.vibratoHandle.depthBP = depthCurve;
        actual = VocaloidMidiEventListFactoryStub::generateVibratoNRPN( &sequence.tempoList, &noteEvent, 500 );
        CPPUNIT_ASSERT_EQUAL( (size_t)5, actual.size() );

        vector<NrpnEvent> actualExpanded = actual[0].expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)6, actualExpanded.size() );
        NrpnEvent item = actualExpanded[0];
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VD_VERSION_AND_DEVICE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VR_VERSION_AND_DEVICE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[2];
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VD_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[3];
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VR_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[4];
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VD_VIBRATO_DEPTH, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 71, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[5];
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VR_VIBRATO_RATE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 72, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        actualExpanded = actual[1].expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)2, actualExpanded.size() );
        item = actualExpanded[0];
        CPPUNIT_ASSERT_EQUAL( (tick_t)336, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VD_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( (tick_t)336, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VD_VIBRATO_DEPTH, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 13, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = actual[2];
        actualExpanded = item.expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)2, actualExpanded.size() );
        item = actualExpanded[0];
        CPPUNIT_ASSERT_EQUAL( (tick_t)360, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VR_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
        item = actualExpanded[1];
        CPPUNIT_ASSERT_EQUAL( (tick_t)360, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VR_VIBRATO_RATE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 11, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = actual[3];
        CPPUNIT_ASSERT_EQUAL( (tick_t)456, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VD_VIBRATO_DEPTH, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 14, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = actual[4];
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_VR_VIBRATO_RATE, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 12, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
    }

    void testGenerateVoiceChangeParameterNRPN(){
        Sequence sequence( "Foo", 1, 4, 4, 500000 );
        Track *track = &sequence.track[0];

        // 全種類のカーブに、データ点を1個ずつ入れる
        vector<string> curveNames;
        curveNames.push_back( "BRE" );
        curveNames.push_back( "BRI" );
        curveNames.push_back( "CLE" );
        curveNames.push_back( "POR" );
        curveNames.push_back( "GEN" );
        curveNames.push_back( "harmonics" );
        curveNames.push_back( "OPE" );
        curveNames.push_back( "reso1amp" );
        curveNames.push_back( "reso1bw" );
        curveNames.push_back( "reso1freq" );
        curveNames.push_back( "reso2amp" );
        curveNames.push_back( "reso2bw" );
        curveNames.push_back( "reso2freq" );
        curveNames.push_back( "reso3amp" );
        curveNames.push_back( "reso3bw" );
        curveNames.push_back( "reso3freq" );
        curveNames.push_back( "reso4amp" );
        curveNames.push_back( "reso4bw" );
        curveNames.push_back( "reso4freq" );
        for( int i = 0; i < curveNames.size(); i++ ){
            string curveName = curveNames[i];
            BPList *list = track->getCurve( curveName );
            list->clear();
            list->setName( curveName );
            list->setDefault( 0 );
            list->setMinimum( 0 );
            list->setMaximum( 127 );
            list->add( 480, 64 );
        }

        // VOCALOID1 の場合
        track->getCommon()->version = "DSB2";
        vector<NrpnEvent> events = VocaloidMidiEventListFactoryStub::generateVoiceChangeParameterNRPN( track, &sequence.tempoList, 500, sequence.getPreMeasureClocks() );
        // 中身は見ない。各カーブに MIDI イベントが1つずつできることだけをチェック
        // 各イベントの子にあたるイベントのテストは、test_addVoiceChangeParameters で行う
        // vocaloid1 で出力されるのは 18 種類
        CPPUNIT_ASSERT_EQUAL( (size_t)18, events.size() );

        // VOCALOID2 の場合
        // 6 種類
        track->getCommon()->version = "DSB3";
        events = VocaloidMidiEventListFactoryStub::generateVoiceChangeParameterNRPN( track, &sequence.tempoList, 500, sequence.getPreMeasureClocks() );
        CPPUNIT_ASSERT_EQUAL( (size_t)6, events.size() );

        // UNKNOWN の場合
        // 5 種類
        track->getCommon()->version = "";
        events = VocaloidMidiEventListFactoryStub::generateVoiceChangeParameterNRPN( track, &sequence.tempoList, 500, sequence.getPreMeasureClocks() );
        CPPUNIT_ASSERT_EQUAL( (size_t)5, events.size() );
    }

    void testAddVoiceChangeParameters(){
        Sequence sequence( "Foo", 1, 4, 4, 500000 );
        BPList *list = sequence.track[0].getCurve( "BRE" );
        list->clear();
        list->add( 480, 0 );
        list->add( 1920, 127 );
        vector<NrpnEvent> dest;
        int preSend = 500;
        int delay = VocaloidMidiEventListFactoryStub::addVoiceChangeParameters( dest, list, &sequence.tempoList, preSend, 0 );

        CPPUNIT_ASSERT_EQUAL( (size_t)2, dest.size() );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        vector<NrpnEvent> actual = dest[0].expand();
        CPPUNIT_ASSERT_EQUAL( (size_t)3, actual.size() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[0].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_DELAY, actual[0].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[0].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[0].dataLSB );
        CPPUNIT_ASSERT( actual[0].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[0].isMSBOmittingRequired );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x31, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[2].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].hasLSB );
        CPPUNIT_ASSERT( actual[2].isMSBOmittingRequired );

        actual = dest[1].expand();
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[0].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[0].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x31, actual[0].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[0].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[0].isMSBOmittingRequired );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 127, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].hasLSB );
        CPPUNIT_ASSERT( actual[1].isMSBOmittingRequired );
    }

    void testGenerateFx2DepthNRPN(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        Track *track = &sequence.track[0];
        BPList *list = track->getCurve( "fx2depth" );
        list->clear();
        list->add( 480, 64 );
        list->add( 1920, 63 );

        vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateFx2DepthNRPN( track, &sequence.tempoList, 500 );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, actual.size() );

        vector<NrpnEvent> expandedActual = actual[0].expand();
        NrpnEvent item = expandedActual[0];
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_FX2_DELAY, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, item.dataLSB );
        CPPUNIT_ASSERT( item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = expandedActual[1];
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_FX2_EFFECT2_DEPTH, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 64, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );

        item = actual[1];
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, item.clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_FX2_EFFECT2_DEPTH, item.nrpn );
        CPPUNIT_ASSERT_EQUAL( 63, item.dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, item.hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, item.isMSBOmittingRequired );
    }

    void test_getActualClockAndDelay(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        tick_t actualClock;
        int delay;

        VocaloidMidiEventListFactoryStub::getActualClockAndDelay( &sequence.tempoList, 1920, 500, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actualClock );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        VocaloidMidiEventListFactoryStub::getActualClockAndDelay( &sequence.tempoList, 1920, 499, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actualClock );
        CPPUNIT_ASSERT_EQUAL( 500, delay );

        VocaloidMidiEventListFactoryStub::getActualClockAndDelay( &sequence.tempoList, 1920, 498, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1441, actualClock );
        CPPUNIT_ASSERT_EQUAL( 498, delay );

        VocaloidMidiEventListFactoryStub::getActualClockAndDelay( &sequence.tempoList, 0, 500, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actualClock );
        CPPUNIT_ASSERT_EQUAL( 0, delay );

        VocaloidMidiEventListFactoryStub::getActualClockAndDelay( &sequence.tempoList, 0, 0, &actualClock, &delay );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actualClock );
        CPPUNIT_ASSERT_EQUAL( 0, delay );
    }

    void test_getMsbAndLsb(){
        int msb, lsb;
        VocaloidMidiEventListFactoryStub::getMsbAndLsb( 264, &msb, &lsb );
        CPPUNIT_ASSERT_EQUAL( 2, msb );
        CPPUNIT_ASSERT_EQUAL( 8, lsb );
    }

    void testGenerateNRPN(){
        Sequence sequence( "Miku", 1, 4, 4, 500000 );
        Track *track = &sequence.track[0];

        // singerEvent
        Event singerEvent( 1920, EventType::SINGER );
        singerEvent.singerHandle = Handle( HandleType::SINGER );
        track->events()->add( singerEvent );

        // voiceChangeParameter
        vector<string> curveNames;
        curveNames.push_back( "BRE" );
        curveNames.push_back( "BRI" );
        curveNames.push_back( "CLE" );
        curveNames.push_back( "POR" );
        curveNames.push_back( "GEN" );
        curveNames.push_back( "OPE" );
        for( int i = 0; i < curveNames.size(); i++ ){
            string curveName = curveNames[i];
            BPList *list = track->getCurve( curveName );
            list->clear();
            list->setDefault( 0 );
            list->setMinimum( 0 );
            list->setMaximum( 127 );
            list->add( 480, 64 );
        }

        // expression
        track->getCurve( "dyn" )->clear();
        track->getCurve( "dyn" )->add( 480, 127 );
        track->getCurve( "dyn" )->add( 1920, 0 );

        // pitchBendSensitivity
        track->getCurve( "pbs" )->add( 480, 0 );
        track->getCurve( "pbs" )->add( 1920, 24 );

        // pitchBend
        track->getCurve( "pit" )->add( 480, 8191 );
        track->getCurve( "pit" )->add( 1920, -8192 );

        // note
        Event noteEvent( 1920, EventType::NOTE );
        noteEvent.setLength( 480 );
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
        noteEvent.vibratoHandle = Handle( HandleType::UNKNOWN );
        noteEvent.lyricHandle = Handle( HandleType::LYRIC );
        noteEvent.lyricHandle.setLyricAt( 0, Lyric( "あ", "a" ) );
        track->events()->add( noteEvent );

        int preSendMilliseconds = 500;
        vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateNRPN(
            track, &sequence.tempoList, sequence.getTotalClocks(), sequence.getPreMeasureClocks(), preSendMilliseconds );
        CPPUNIT_ASSERT_EQUAL( (size_t)50, actual.size() );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[0].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_PBS_DELAY, actual[0].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[0].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[0].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[0].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[1].dataMSB );
        CPPUNIT_ASSERT( actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[1].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_E_DELAY, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[2].dataMSB );
        CPPUNIT_ASSERT( actual[2].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[2].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[3].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_E_EXPRESSION, actual[3].nrpn );
        CPPUNIT_ASSERT_EQUAL( 127, actual[3].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[3].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[3].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[4].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_VERSION_AND_DEVICE, actual[4].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[4].dataMSB );
        CPPUNIT_ASSERT( actual[4].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[4].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[4].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[5].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_DELAY, actual[5].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[5].dataMSB );
        CPPUNIT_ASSERT( actual[5].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[5].dataLSB );
        CPPUNIT_ASSERT( actual[5].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[6].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_LANGUAGE_TYPE, actual[6].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[6].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[6].hasLSB );
        CPPUNIT_ASSERT( actual[6].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[7].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::PC_VOICE_TYPE, actual[7].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[7].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[7].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[7].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[8].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_DELAY, actual[8].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[8].dataMSB );
        CPPUNIT_ASSERT( actual[8].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[8].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[8].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[9].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[9].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x31, actual[9].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[9].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[9].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[10].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[10].nrpn );
        CPPUNIT_ASSERT_EQUAL( 64, actual[10].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[10].hasLSB );
        CPPUNIT_ASSERT( actual[10].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[11].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[11].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x32, actual[11].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[11].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[11].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[12].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[12].nrpn );
        CPPUNIT_ASSERT_EQUAL( 64, actual[12].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[12].hasLSB );
        CPPUNIT_ASSERT( actual[12].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[13].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[13].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x33, actual[13].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[13].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[13].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[14].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[14].nrpn );
        CPPUNIT_ASSERT_EQUAL( 64, actual[14].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[14].hasLSB );
        CPPUNIT_ASSERT( actual[14].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[15].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[15].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x34, actual[15].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[15].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[15].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[16].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[16].nrpn );
        CPPUNIT_ASSERT_EQUAL( 64, actual[16].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[16].hasLSB );
        CPPUNIT_ASSERT( actual[16].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[17].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[17].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x35, actual[17].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[17].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[17].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[18].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[18].nrpn );
        CPPUNIT_ASSERT_EQUAL( 64, actual[18].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[18].hasLSB );
        CPPUNIT_ASSERT( actual[18].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[19].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[19].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x70, actual[19].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[19].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[19].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[20].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[20].nrpn );
        CPPUNIT_ASSERT_EQUAL( 64, actual[20].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[20].hasLSB );
        CPPUNIT_ASSERT( actual[20].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[21].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::PB_DELAY, actual[21].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[21].dataMSB );
        CPPUNIT_ASSERT( actual[21].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[21].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[21].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[22].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::PB_PITCH_BEND, actual[22].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x7F, actual[22].dataMSB );
        CPPUNIT_ASSERT( actual[22].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x7F, actual[22].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[22].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[23].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY, actual[23].nrpn );
        CPPUNIT_ASSERT_EQUAL( 24, actual[23].dataMSB );
        CPPUNIT_ASSERT( actual[23].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[23].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[23].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[24].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_E_EXPRESSION, actual[24].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[24].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[24].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[24].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[25].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_VERSION_AND_DEVICE, actual[25].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[25].dataMSB );
        CPPUNIT_ASSERT( actual[25].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[25].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[25].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[26].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_DELAY, actual[26].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[26].dataMSB );
        CPPUNIT_ASSERT( actual[26].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[26].dataLSB );
        CPPUNIT_ASSERT( actual[26].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[27].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_BS_LANGUAGE_TYPE, actual[27].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[27].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[27].hasLSB );
        CPPUNIT_ASSERT( actual[27].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[28].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::PC_VOICE_TYPE, actual[28].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[28].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[28].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[28].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[29].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::PB_PITCH_BEND, actual[29].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[29].dataMSB );
        CPPUNIT_ASSERT( actual[29].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x00, actual[29].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[29].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[30].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_DELAY, actual[30].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[30].dataMSB );
        CPPUNIT_ASSERT( actual[30].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[30].dataLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[30].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[31].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_NOTE_NUMBER, actual[31].nrpn );
        CPPUNIT_ASSERT_EQUAL( 60, actual[31].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[31].hasLSB );
        CPPUNIT_ASSERT( actual[31].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[32].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_VELOCITY, actual[32].nrpn );
        CPPUNIT_ASSERT_EQUAL( 127, actual[32].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[32].hasLSB );
        CPPUNIT_ASSERT( actual[32].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[33].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_NOTE_DURATION, actual[33].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[33].dataMSB );
        CPPUNIT_ASSERT( actual[33].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[33].dataLSB );
        CPPUNIT_ASSERT( actual[33].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[34].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_NOTE_LOCATION, actual[34].nrpn );
        CPPUNIT_ASSERT_EQUAL( 3, actual[34].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[34].hasLSB );
        CPPUNIT_ASSERT( actual[34].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[35].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_PHONETIC_SYMBOL_BYTES, actual[35].nrpn );
        CPPUNIT_ASSERT_EQUAL( 1, actual[35].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[35].hasLSB );
        CPPUNIT_ASSERT( actual[35].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[36].clock );
        CPPUNIT_ASSERT_EQUAL( (MidiParameterType::MidiParameterTypeEnum)0x5013, actual[36].nrpn );
        CPPUNIT_ASSERT_EQUAL( (int)'a', actual[36].dataMSB );
        CPPUNIT_ASSERT( actual[36].hasLSB );
        CPPUNIT_ASSERT_EQUAL( 0, actual[36].dataLSB );
        CPPUNIT_ASSERT( actual[36].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[37].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_PHONETIC_SYMBOL_CONTINUATION, actual[37].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x7f, actual[37].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[37].hasLSB );
        CPPUNIT_ASSERT( actual[37].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[38].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_V1MEAN, actual[38].nrpn );
        CPPUNIT_ASSERT_EQUAL( 4, actual[38].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[38].hasLSB );
        CPPUNIT_ASSERT( actual[38].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[39].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_D1MEAN, actual[39].nrpn );
        CPPUNIT_ASSERT_EQUAL( 8, actual[39].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[39].hasLSB );
        CPPUNIT_ASSERT( actual[39].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[40].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_D1MEAN_FIRST_NOTE, actual[40].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x14, actual[40].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[40].hasLSB );
        CPPUNIT_ASSERT( actual[40].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[41].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_D2MEAN, actual[41].nrpn );
        CPPUNIT_ASSERT_EQUAL( 28, actual[41].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[41].hasLSB );
        CPPUNIT_ASSERT( actual[41].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[42].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_D4MEAN, actual[42].nrpn );
        CPPUNIT_ASSERT_EQUAL( 63, actual[42].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[42].hasLSB );
        CPPUNIT_ASSERT( actual[42].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[43].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_PMEAN_ONSET_FIRST_NOTE, actual[43].nrpn );
        CPPUNIT_ASSERT_EQUAL( 65, actual[43].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[43].hasLSB );
        CPPUNIT_ASSERT( actual[43].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[44].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_VMEAN_NOTE_TRNSITION, actual[44].nrpn );
        CPPUNIT_ASSERT_EQUAL( 66, actual[44].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[44].hasLSB );
        CPPUNIT_ASSERT( actual[44].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[45].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_PMEAN_ENDING_NOTE, actual[45].nrpn );
        CPPUNIT_ASSERT_EQUAL( 67, actual[45].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[45].hasLSB );
        CPPUNIT_ASSERT( actual[45].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[46].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_ADD_PORTAMENTO, actual[46].nrpn );
        CPPUNIT_ASSERT_EQUAL( 3, actual[46].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[46].hasLSB );
        CPPUNIT_ASSERT( actual[46].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[47].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_CHANGE_AFTER_PEAK, actual[47].nrpn );
        CPPUNIT_ASSERT_EQUAL( 50, actual[47].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[47].hasLSB );
        CPPUNIT_ASSERT( actual[47].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[48].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_ACCENT, actual[48].nrpn );
        CPPUNIT_ASSERT_EQUAL( 50, actual[48].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[48].hasLSB );
        CPPUNIT_ASSERT( actual[48].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[49].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION, actual[49].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x7f, actual[49].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[49].hasLSB );
        CPPUNIT_ASSERT( actual[49].isMSBOmittingRequired );
    }

    CPPUNIT_TEST_SUITE( VocaloidMidiEventListFactoryTest );
    CPPUNIT_TEST( test_generateExpressionNRPN );
    CPPUNIT_TEST( test_generateHeaderNRPN );
    CPPUNIT_TEST( testGenerateSingerNRPN );
    CPPUNIT_TEST( testGenerateNoteNRPN );
    CPPUNIT_TEST( testGeneratePitchBendNRPN );
    CPPUNIT_TEST( testGeneratePitchBendSensitivityNRPN );
    CPPUNIT_TEST( testGenerateVibratoNRPN );
    CPPUNIT_TEST( testGenerateVoiceChangeParameterNRPN );
    CPPUNIT_TEST( testAddVoiceChangeParameters );
    CPPUNIT_TEST( testGenerateFx2DepthNRPN );
    CPPUNIT_TEST( test_getActualClockAndDelay );
    CPPUNIT_TEST( test_getMsbAndLsb );
    CPPUNIT_TEST( testGenerateNRPN );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( VocaloidMidiEventListFactoryTest );

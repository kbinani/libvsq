#include "Util.hpp"
#include "../VocaloidMidiEventListFactory.hpp"
#include "../Sequence.hpp"

using namespace std;
using namespace VSQ_NS;

class VocaloidMidiEventListFactoryStub : public VocaloidMidiEventListFactory{
public:
    static vector<NrpnEvent> generateExpressionNRPN( Track *track, TempoList *tempoList, int preSendMilliseconds ){
        return VocaloidMidiEventListFactory::generateExpressionNRPN( track, tempoList, preSendMilliseconds );
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
        Track *track = &(sequence.track[1]);
        BPList *dynamics = track->getCurve( "DYN" );
        dynamics->add( 480, 127 );
        dynamics->add( 1920, 0 );

        vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateExpressionNRPN( track, &sequence.tempoList, 500 );
        CPPUNIT_ASSERT_EQUAL( (size_t)3, actual.size() );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[0].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_E_DELAY, actual[0].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0x03, actual[0].dataMSB );
        CPPUNIT_ASSERT_EQUAL( 0x74, actual[0].dataLSB );
        CPPUNIT_ASSERT( actual[0].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[0].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, actual[1].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_E_EXPRESSION, actual[1].nrpn );
        CPPUNIT_ASSERT_EQUAL( 127, actual[1].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[1].isMSBOmittingRequired );

        CPPUNIT_ASSERT_EQUAL( (tick_t)1440, actual[2].clock );
        CPPUNIT_ASSERT_EQUAL( MidiParameterType::CC_E_EXPRESSION, actual[2].nrpn );
        CPPUNIT_ASSERT_EQUAL( 0, actual[2].dataMSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].hasLSB );
        CPPUNIT_ASSERT_EQUAL( false, actual[2].isMSBOmittingRequired );
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

    CPPUNIT_TEST_SUITE( VocaloidMidiEventListFactoryTest );
    CPPUNIT_TEST( test_generateExpressionNRPN );
    CPPUNIT_TEST( test_getActualClockAndDelay );
    CPPUNIT_TEST( test_getMsbAndLsb );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( VocaloidMidiEventListFactoryTest );

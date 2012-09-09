#include "Util.hpp"
#include "../Sequence.hpp"
#include "../ByteArrayOutputStream.hpp"

using namespace std;
using namespace VSQ_NS;

class SequenceTest : public CppUnit::TestCase{
public:
    /**
     * 指定されたシーケンスがデフォルトのシーケンスと等しいかどうかを検査する
     */
    void isEqualToDefaultSequence( const Sequence &sequence ){
        CPPUNIT_ASSERT_EQUAL( (tick_t)(1 * 480 * 4 / 4 * 4), sequence.getTotalClocks() );

        // track
        CPPUNIT_ASSERT_EQUAL( (size_t)1, sequence.track.size() );
        // 第1トラック
        Track track1 = sequence.track[0];
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
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, sequence.timesigList.get( 0 ).getClock() );
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
        sequence.track[0].getEvents()->add( note );
        sequence.updateTotalClocks();
        CPPUNIT_ASSERT_EQUAL( (tick_t)2400, sequence.getTotalClocks() );
    }

    void testGetMaximumNoteLengthAt(){
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

    CPPUNIT_TEST_SUITE( SequenceTest );
    CPPUNIT_TEST( testConstruct );
    CPPUNIT_TEST( testClone );
    CPPUNIT_TEST( testGetBaseTempo );
    CPPUNIT_TEST( testGetPreMeasure );
    CPPUNIT_TEST( testGetPreMeasureClocks );
    CPPUNIT_TEST( testGetTickPerQuarter );
    CPPUNIT_TEST( testUpdateTotalClocks );
    CPPUNIT_TEST( testGetMaximumNoteLengthAt );
    CPPUNIT_TEST( testGenerateNRPNAll );
    CPPUNIT_TEST( testGenerateNRPNPartial );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( SequenceTest );

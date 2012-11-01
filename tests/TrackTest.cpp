#include "Util.hpp"
#include "../Track.hpp"

using namespace std;
using namespace VSQ_NS;

class TrackStub : public Track{
public:
    explicit TrackStub( std::string name, std::string singer )
        : Track( name, singer )
    {
    }
};

class TrackTest : public CppUnit::TestCase{
public:
    void testConstructNormalTrack(){
        Track track( "DummyTrackName", "DummySingerName" );
        CPPUNIT_ASSERT_EQUAL( string( "DummyTrackName" ), track.getName() );
        Event::List *events = track.events();
        CPPUNIT_ASSERT_EQUAL( 1, events->size() );
        CPPUNIT_ASSERT_EQUAL( EventType::SINGER, events->get( 0 )->type );
        CPPUNIT_ASSERT_EQUAL( string( "DummySingerName" ), events->get( 0 )->singerHandle.ids );

        //TODO:
/*
        assert_not_nil( trackgCurve( "pit" ) );
        assert_not_nil( track.getCurve( "pbs" ) );
        assert_not_nil( track.getCurve( "dyn" ) );
        assert_not_nil( track.getCurve( "bre" ) );
        assert_not_nil( track.getCurve( "bri" ) );
        assert_not_nil( track.getCurve( "cle" ) );
        assert_not_nil( track.getCurve( "reso1Freq" ) );
        assert_not_nil( track.getCurve( "reso2Freq" ) );
        assert_not_nil( track.getCurve( "reso3Freq" ) );
        assert_not_nil( track.getCurve( "reso4Freq" ) );
        assert_not_nil( track.getCurve( "reso1BW" ) );
        assert_not_nil( track.getCurve( "reso2BW" ) );
        assert_not_nil( track.getCurve( "reso3BW" ) );
        assert_not_nil( track.getCurve( "reso4BW" ) );
        assert_not_nil( track.getCurve( "reso1Amp" ) );
        assert_not_nil( track.getCurve( "reso2Amp" ) );
        assert_not_nil( track.getCurve( "reso3Amp" ) );
        assert_not_nil( track.getCurve( "reso4Amp" ) );
        assert_not_nil( track.getCurve( "harmonics" ) );
        assert_not_nil( track.getCurve( "fx2depth" ) );
        assert_not_nil( track.getCurve( "gen" ) );
        assert_not_nil( track.getCurve( "por" ) );
        assert_not_nil( track.getCurve( "ope" ) );
*/

        CPPUNIT_ASSERT_EQUAL( string( "pit" ), track.getCurve( "pit" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "pit" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( -8192, track.getCurve( "pit" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 8191, track.getCurve( "pit" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "pbs" ), track.getCurve( "pbs" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 2, track.getCurve( "pbs" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "pbs" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 24, track.getCurve( "pbs" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "dyn" ), track.getCurve( "dyn" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "dyn" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "dyn" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "bre" ), track.getCurve( "bre" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "bre" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "bre" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "bre" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "bri" ), track.getCurve( "bri" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "bri" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "bri" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "bri" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "cle" ), track.getCurve( "cle" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "cle" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "cle" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "cle" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso1freq" ), track.getCurve( "reso1Freq" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso1Freq" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso1Freq" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso1Freq" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso2freq" ), track.getCurve( "reso2Freq" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso2Freq" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso2Freq" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso2Freq" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso3freq" ), track.getCurve( "reso3Freq" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso3Freq" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso3Freq" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso3Freq" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso4freq" ), track.getCurve( "reso4Freq" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso4Freq" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso4Freq" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso4Freq" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso1bw" ), track.getCurve( "reso1BW" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso1BW" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso1BW" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso1BW" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso2bw" ), track.getCurve( "reso2BW" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso2BW" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso2BW" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso2BW" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso3bw" ), track.getCurve( "reso3BW" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso3BW" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso3BW" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso3BW" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso4bw" ), track.getCurve( "reso4BW" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso4BW" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso4BW" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso4BW" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso1amp" ), track.getCurve( "reso1Amp" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso1Amp" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso1Amp" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso1Amp" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso2amp" ), track.getCurve( "reso2Amp" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso2Amp" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso2Amp" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso2Amp" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso3amp" ), track.getCurve( "reso3Amp" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso3Amp" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso3Amp" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso3Amp" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "reso4amp" ), track.getCurve( "reso4Amp" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso4Amp" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso4Amp" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso4Amp" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "harmonics" ), track.getCurve( "harmonics" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "harmonics" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "harmonics" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "harmonics" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "fx2depth" ), track.getCurve( "fx2depth" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "fx2depth" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "fx2depth" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "fx2depth" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "gen" ), track.getCurve( "gen" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "gen" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "gen" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "gen" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "por" ), track.getCurve( "por" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "por" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "por" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "por" )->getMaximum() );
        CPPUNIT_ASSERT_EQUAL( string( "ope" ), track.getCurve( "ope" )->getName() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "ope" )->getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "ope" )->getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "ope" )->getMaximum() );
    }
    
    void testConstructFromMidiEvents(){
        //fail();
    }

    void testGetIndexIterator(){
        //TODO
        /*
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
        local iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.NOTE );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.SINGER );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( 0, iterator.next() );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
        */
    }
    
    /**
     * @todo
     */
    void testGetterAndSetterPlayMode(){
        //fail();
    }
    
    /**
     * @todo
     */
    void testGetterAndSetterTrackOn(){
        //fail();
    }
    
    void testGetterAndSetterName(){
        Track track( "DummyTrackName", "DummySingerName" );
        CPPUNIT_ASSERT_EQUAL( string( "DummyTrackName" ), track.getName() );
        track.setName( "foo" );
        CPPUNIT_ASSERT_EQUAL( string( "foo" ), track.getName() );
    }
    
    /**
     * @todo
     */
    void testGetPitchAt(){
        //fail();
    }
    
    /**
     * @todo
     */
    void testReflectDynamics(){
        //fail();
    }
    
    void testGetSingerEventAt(){
        Track track( "", "" );
        track.events()->clear();
        {
            const Event *actual = track.getSingerEventAt( 0 );
            CPPUNIT_ASSERT( 0 == actual );
        }

        Event singer1( 0, EventType::SINGER );
        track.events()->add( singer1, 1 );

        Event note1( 480, EventType::NOTE );
        track.events()->add( note1, 2 );

        Event singer2( 480, EventType::SINGER );
        track.events()->add( singer2, 3 );

        {
            const Event *actual = track.getSingerEventAt( 0 );
            CPPUNIT_ASSERT_EQUAL( 1, actual->id );
        }
        {
            const Event *actual = track.getSingerEventAt( 479 );
            CPPUNIT_ASSERT_EQUAL( 1, actual->id );
        }
        {
            const Event *actual = track.getSingerEventAt( 480 );
            CPPUNIT_ASSERT_EQUAL( 3, actual->id );
        }
        {
            const Event *actual = track.getSingerEventAt( 10000 );
            CPPUNIT_ASSERT_EQUAL( 3, actual->id );
        }
        {
            const Event *actual = track.getSingerEventAt( -100 );
            CPPUNIT_ASSERT( 0 == actual );
        }
    }

    /**
     * @todo
     */
    void testSortEvent(){
        //fail();
    }
    
    void testGetIndexIteratorSinger(){
        Track track( "DummyTrackName", "DummySingerName" );
        EventListIndexIterator iterator = track.getIndexIterator( EventListIndexIteratorKind::SINGER );
        CPPUNIT_ASSERT( iterator.hasNext() );
        const Event *event = track.events()->get( iterator.next() );
        CPPUNIT_ASSERT_EQUAL( string( "DummySingerName" ), event->singerHandle.ids );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testGetIndexIteratorNote(){
        Track track( "DummyTrackName", "DummySingerName" );
        EventListIndexIterator iterator = track.getIndexIterator( EventListIndexIteratorKind::NOTE );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        Event event( 480, EventType::NOTE );
        track.events()->add( event, 10 );
        iterator = track.getIndexIterator( EventListIndexIteratorKind::NOTE );
        CPPUNIT_ASSERT( iterator.hasNext() );
        const Event *obtained = track.events()->get( iterator.next() );
        //TODO: Event::equalが実装されたらコメントアウトを元に戻す
        //CPPUNIT_ASSERT_EQUAL( event, obtained );
        CPPUNIT_ASSERT_EQUAL( 10, obtained->id );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testGetIndexIteratorDynamics(){
        Track track( "DummyTrackName", "DummySingerName" );
        EventListIndexIterator iterator = track.getIndexIterator( EventListIndexIteratorKind::DYNAFF );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        Event event( 480, EventType::ICON );
        event.iconDynamicsHandle = Handle( HandleType::DYNAMICS );
        event.iconDynamicsHandle.iconId = "$05019999";
        track.events()->add( event, 10 );
        iterator = track.getIndexIterator( EventListIndexIteratorKind::DYNAFF );
        CPPUNIT_ASSERT( iterator.hasNext() );
        const Event *obtained = track.events()->get( iterator.next() );
        //TODO: Event::equalが実装されたらコメントアウトを元に戻す
        //CPPUNIT_ASSERT_EQUAL( event, obtained );
        CPPUNIT_ASSERT_EQUAL( 10, obtained->id );
        CPPUNIT_ASSERT_EQUAL( string( "$05019999" ), obtained->iconDynamicsHandle.iconId );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    /**
     * @todo
     */
    void testChangeRenderer(){
        //fail();
    }
    
    /**
     * @todo
     */
    void testGetterAndSetterCurve(){
/*
        Track track( "DummyTrackName", "DummySingerName" );
        CPPUNIT_ASSERT_EQUAL( track._pit, track.getCurve( "PiT" ) );
        CPPUNIT_ASSERT_EQUAL( track._pbs, track.getCurve( "PBS" ) );
        CPPUNIT_ASSERT_EQUAL( track._dyn, track.getCurve( "DYN" ) );
        CPPUNIT_ASSERT_EQUAL( track._bre, track.getCurve( "bre" ) );
        CPPUNIT_ASSERT_EQUAL( track._bri, track.getCurve( "BRI" ) )
        CPPUNIT_ASSERT_EQUAL( track._cle, track.getCurve( "CLE" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso1FreqBPList, track.getCurve( "RESO1FREQ" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso2FreqBPList, track.getCurve( "RESO2FREQ" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso3FreqBPList, track.getCurve( "RESO3FREQ" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso4FreqBPList, track.getCurve( "RESO4FREQ" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso1BWBPList, track.getCurve( "RESO1BW" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso2BWBPList, track.getCurve( "RESO2BW" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso3BWBPList, track.getCurve( "RESO3BW" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso4BWBPList, track.getCurve( "RESO4BW" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso1AmpBPList, track.getCurve( "RESO1amp" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso2AmpBPList, track.getCurve( "RESO2amp" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso3AmpBPList, track.getCurve( "RESO3amp" ) );
        CPPUNIT_ASSERT_EQUAL( track._reso4AmpBPList, track.getCurve( "RESO4amp" ) );
        CPPUNIT_ASSERT_EQUAL( track._harmonics, track.getCurve( "HARMONICS" ) );
        CPPUNIT_ASSERT_EQUAL( track._fx2depth, track.getCurve( "fx2depth" ) );
        CPPUNIT_ASSERT_EQUAL( track._gen, track.getCurve( "GEN" ) );
        CPPUNIT_ASSERT_EQUAL( track._por, track.getCurve( "pOr" ) );
        CPPUNIT_ASSERT_EQUAL( track._ope, track.getCurve( "OPE" ) );
        assert_nil( track.getCurve( "__UNKNOWN_CURVE_NAME__" ) );
    
        local curves = {
            "PiT", "PBS", "DYN", "bre", "BRI", "CLE",
            "RESO1FREQ", "RESO2FREQ", "RESO3FREQ", "RESO4FREQ",
            "RESO1BW", "RESO2BW", "RESO3BW", "RESO4BW",
            "RESO1amp", "RESO2amp", "RESO3amp", "RESO4amp",
            "HARMONICS", "fx2depth", "GEN", "pOr", "OPE"
        };
        local i;
        for i = 1, #curves, 1 do
            track.setCurve( curves[i], nil );
        }
        for i = 1, #curves, 1 do
            assert_nil( track.getCurve( curves[i] ) );
        }
    
        local dummyList = luavsq.BPList.new( "foo", 1, -1000, 1000 );
        for i = 1, #curves, 1 do
            local j;
            for j = 1, #curves, 1 do
                track.setCurve( curves[j], nil );
            }
            track.setCurve( curves[i], dummyList );
            for j = 1, #curves, 1 do
                if( curves[i] == curves[j] ){
                    CPPUNIT_ASSERT_EQUAL( dummyList, track.getCurve( curves[j] ) );
                }else{
                    assert_nil( track.getCurve( curves[j] ) );
                }
            }
        }
*/
    }

    /**
     * @todo
     */
    void testClone(){
        Track track( "DummyTrackName", "DummySingerName" );
        Event event( 480, EventType::NOTE );
        track.events()->add( event );
        track.getCurve( "pit" )->add( 480, 100 );
    
        Track copy = track.clone();
        CPPUNIT_ASSERT_EQUAL( 2, copy.events()->size() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, copy.events()->get( 0 )->clock );
        CPPUNIT_ASSERT_EQUAL( EventType::SINGER, copy.events()->get( 0 )->type );
        CPPUNIT_ASSERT_EQUAL( string( "DummySingerName" ), copy.events()->get( 0 )->singerHandle.ids );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, copy.events()->get( 1 )->clock );
        CPPUNIT_ASSERT_EQUAL( EventType::NOTE, copy.events()->get( 1 )->type );
        CPPUNIT_ASSERT_EQUAL( 1, copy.getCurve( "pit" )->size() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, copy.getCurve( "pit" )->getKeyClock( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 100, copy.getCurve( "pit" )->get( 0 ).value );
        CPPUNIT_ASSERT_EQUAL( string( "DummyTrackName" ), copy.getName() );
    }

    /**
     * @todo
     */
    void testGetLyricLength(){
        //fail();
    }

    CPPUNIT_TEST_SUITE( TrackTest );
    CPPUNIT_TEST( testConstructNormalTrack );
    CPPUNIT_TEST( testConstructFromMidiEvents );
    CPPUNIT_TEST( testGetIndexIterator );
    CPPUNIT_TEST( testGetterAndSetterPlayMode );
    CPPUNIT_TEST( testGetterAndSetterTrackOn );
    CPPUNIT_TEST( testGetterAndSetterName );
    CPPUNIT_TEST( testGetPitchAt );
    CPPUNIT_TEST( testReflectDynamics );
    CPPUNIT_TEST( testGetSingerEventAt );
    CPPUNIT_TEST( testSortEvent );
    CPPUNIT_TEST( testGetIndexIteratorSinger );
    CPPUNIT_TEST( testGetIndexIteratorNote );
    CPPUNIT_TEST( testGetIndexIteratorDynamics );
    CPPUNIT_TEST( testChangeRenderer );
    CPPUNIT_TEST( testGetterAndSetterCurve );
    CPPUNIT_TEST( testClone );
    CPPUNIT_TEST( testGetLyricLength );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( TrackTest );

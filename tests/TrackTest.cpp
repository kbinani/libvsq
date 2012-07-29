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

    Master getMaster() const{
        return master;
    }
};

class TrackTest : public CppUnit::TestCase{
public:
    void testConstructNormalTrack(){
        Track track( "DummyTrackName", "DummySingerName" );
        CPPUNIT_ASSERT_EQUAL( string( "DummyTrackName" ), track.getName() );
        Event::List *events = track.getEvents();
        CPPUNIT_ASSERT_EQUAL( 1, events->size() );
        CPPUNIT_ASSERT_EQUAL( EventType::SINGER, events->get( 0 ).type );
        CPPUNIT_ASSERT_EQUAL( string( "DummySingerName" ), events->get( 0 ).singerHandle.ids );

        //TODO:
/*
        assert_not_nil( track.common );
        assert_nil( track.master );
        assert_nil( track.mixer );
        assert_not_nil( track.events );
*/

        //TODO:
/*
        assert_not_nil( track.getCurve( "pit" ) );
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
    
    /**
     * @todo
     */
    void testGetSingerEventAt(){
        //fail();
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
        Event event = track.getEvents()->get( iterator.next() );
        CPPUNIT_ASSERT_EQUAL( string( "DummySingerName" ), event.singerHandle.ids );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testGetIndexIteratorNote(){
        Track track( "DummyTrackName", "DummySingerName" );
        EventListIndexIterator iterator = track.getIndexIterator( EventListIndexIteratorKind::NOTE );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        Event event( 480, EventType::NOTE );
        track.getEvents()->add( event, 10 );
        iterator = track.getIndexIterator( EventListIndexIteratorKind::NOTE );
        CPPUNIT_ASSERT( iterator.hasNext() );
        Event obtained = track.getEvents()->get( iterator.next() );
        //TODO: Event::equalが実装されたらコメントアウトを元に戻す
        //CPPUNIT_ASSERT_EQUAL( event, obtained );
        CPPUNIT_ASSERT_EQUAL( 10, obtained.id );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testGetIndexIteratorDynamics(){
        Track track( "DummyTrackName", "DummySingerName" );
        EventListIndexIterator iterator = track.getIndexIterator( EventListIndexIteratorKind::DYNAFF );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        Event event( 480, EventType::ICON );
        event.iconDynamicsHandle = Handle( HandleType::DYNAMICS );
        event.iconDynamicsHandle.iconId = "$05019999";
        track.getEvents()->add( event, 10 );
        iterator = track.getIndexIterator( EventListIndexIteratorKind::DYNAFF );
        CPPUNIT_ASSERT( iterator.hasNext() );
        Event obtained = track.getEvents()->get( iterator.next() );
        //TODO: Event::equalが実装されたらコメントアウトを元に戻す
        //CPPUNIT_ASSERT_EQUAL( event, obtained );
        CPPUNIT_ASSERT_EQUAL( 10, obtained.id );
        CPPUNIT_ASSERT_EQUAL( string( "$05019999" ), obtained.iconDynamicsHandle.iconId );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testPrintMetaText(){
        TrackStub track( "DummyTrackName", "DummySingerName" );
    
        Event singerEvent( 0, EventType::SINGER );
        singerEvent.singerHandle = Handle( HandleType::SINGER ); //h#0000
        singerEvent.singerHandle.iconId = "$07010002";
        singerEvent.singerHandle.ids = "Miku";
        singerEvent.singerHandle.original = 1;
        singerEvent.singerHandle.setCaption( "caption for miku" );
        singerEvent.singerHandle.language = 1;
        singerEvent.singerHandle.program = 2;
        track.getEvents()->set( 0, singerEvent );
    
        Event crescendoEvent( 240, EventType::ICON );
        crescendoEvent.note = 64;
        crescendoEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS ); //h#0001
        crescendoEvent.iconDynamicsHandle.iconId = "$05020001";
        crescendoEvent.iconDynamicsHandle.ids = "crescendo";
        crescendoEvent.iconDynamicsHandle.original = 1;
        crescendoEvent.iconDynamicsHandle.setCaption( "caption for crescendo" );
        crescendoEvent.iconDynamicsHandle.setStartDyn( 4 );
        crescendoEvent.iconDynamicsHandle.setEndDyn( 7 );
        crescendoEvent.setLength( 10 );
        track.getEvents()->add( crescendoEvent, 2 );
    
        Event dynaffEvent( 480, EventType::ICON );
        dynaffEvent.note = 65;
        dynaffEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS );//h#0002
        dynaffEvent.iconDynamicsHandle.iconId = "$05010001";
        dynaffEvent.iconDynamicsHandle.ids = "dynaff";
        dynaffEvent.iconDynamicsHandle.original = 2;
        dynaffEvent.iconDynamicsHandle.setCaption( "caption for dynaff" );
        dynaffEvent.iconDynamicsHandle.setStartDyn( 5 );
        dynaffEvent.iconDynamicsHandle.setEndDyn( 8 );
        dynaffEvent.setLength( 11 );
        track.getEvents()->add( dynaffEvent, 3 );
    
        Event decrescendoEvent( 720, EventType::ICON );
        decrescendoEvent.note = 66;
        decrescendoEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS );//h#0003
        decrescendoEvent.iconDynamicsHandle.iconId = "$05030001";
        decrescendoEvent.iconDynamicsHandle.ids = "decrescendo";
        decrescendoEvent.iconDynamicsHandle.original = 3;
        decrescendoEvent.iconDynamicsHandle.setCaption( "caption for decrescendo" );
        decrescendoEvent.iconDynamicsHandle.setStartDyn( 6 );
        decrescendoEvent.iconDynamicsHandle.setEndDyn( 9 );
        decrescendoEvent.setLength( 12 );
        track.getEvents()->add( decrescendoEvent, 4 );
    
        Event singerEvent2( 1920, EventType::SINGER );
        singerEvent2.singerHandle = Handle( HandleType::SINGER );//h#0004
        singerEvent2.singerHandle.iconId = "$07020003";
        singerEvent2.singerHandle.ids = "Luka_EN";
        singerEvent2.singerHandle.original = 0x82;
        singerEvent2.singerHandle.setCaption( "caption for luka" );
        singerEvent2.singerHandle.language = 2;
        singerEvent2.singerHandle.program = 3;
        track.getEvents()->add( singerEvent2, 5 );
    
        Event noteEvent( 1920, EventType::NOTE );
        noteEvent.note = 67;
        noteEvent.dynamics = 68;
        noteEvent.pmBendDepth = 69;
        noteEvent.pmBendLength = 70;
        noteEvent.pmbPortamentoUse = 3;
        noteEvent.demDecGainRate = 71;
        noteEvent.demAccent = 72;
        noteEvent.setLength( 480 );
        noteEvent.lyricHandle = Handle( HandleType::LYRIC );
        noteEvent.lyricHandle.setLyricAt( 0, Lyric( "ら", "4 a" ) );//h#0005
        noteEvent.vibratoHandle = Handle( HandleType::VIBRATO );//h#0006
        noteEvent.vibratoDelay = 73;
        noteEvent.vibratoHandle.iconId ="$04040004";
        noteEvent.vibratoHandle.ids = "vibrato";
        noteEvent.vibratoHandle.original = 1;
        noteEvent.vibratoHandle.setCaption( "caption for vibrato" );
        noteEvent.vibratoHandle.setLength( 407 );
        noteEvent.vibratoHandle.setStartDepth( 13 );
        noteEvent.vibratoHandle.setStartRate( 14 );
        noteEvent.noteHeadHandle = Handle( HandleType::NOTE_HEAD );//h#0007
        noteEvent.noteHeadHandle.iconId = "$05030000";
        noteEvent.noteHeadHandle.ids = "attack";
        noteEvent.noteHeadHandle.original = 15;
        noteEvent.noteHeadHandle.setCaption( "caption for attack" );
        noteEvent.noteHeadHandle.setLength( 120 );
        noteEvent.noteHeadHandle.setDuration( 62 );
        noteEvent.noteHeadHandle.setDepth( 65 );
        track.getEvents()->add( noteEvent, 6 );
    
        track.setPreMeasure( 1 );
    
        track.getMixer()->masterFeder = 1;
        track.getMixer()->masterPanpot = 2;
        track.getMixer()->masterMute = 3;
        track.getMixer()->outputMode = 4;
        track.getMixer()->slave.push_back( MixerItem( 5, 6, 7, 8 ) );
    
        track.getCommon()->version = "DSB301";
        track.getCommon()->name = "foo";
        track.getCommon()->color = "1,2,3";
        track.getCommon()->dynamicsMode = DynamicsMode::STANDARD;
        track.getCommon()->playMode = PlayMode::PLAY_WITH_SYNTH;

        vector<string> curves;
        curves.push_back( "pit" );
        curves.push_back( "pbs" );
        curves.push_back( "dyn" );
        curves.push_back( "bre" );
        curves.push_back( "bri" );
        curves.push_back( "cle" );
        curves.push_back( "RESO1FREQ" );
        curves.push_back( "RESO2FREQ" );
        curves.push_back( "RESO3FREQ" );
        curves.push_back( "RESO4FREQ" );
        curves.push_back( "RESO1BW" );
        curves.push_back( "RESO2BW" );
        curves.push_back( "RESO3BW" );
        curves.push_back( "RESO4BW" );
        curves.push_back( "RESO1amp" );
        curves.push_back( "RESO2amp" );
        curves.push_back( "RESO3amp" );
        curves.push_back( "RESO4amp" );
        curves.push_back( "HARMONICS" );
        curves.push_back( "fx2depth" );
        curves.push_back( "GEN" );
        curves.push_back( "pOr" );
        curves.push_back( "OPE" );
        for( int i = 0; i < curves.size(); i++ ){
            string curveName = curves[i];
            track.getCurve( curveName )->add( 480 + i, i );
        }
    
        TextStream stream;

        track.printMetaText( stream, 2400, 0, false, &track.getMaster(), track.getMixer() );
        string expected =
            "[Common]\n"
            "Version=DSB301\n"
            "Name=foo\n"
            "Color=1,2,3\n"
            "DynamicsMode=0\n"
            "PlayMode=1\n"
    
            "[Master]\n"
            "PreMeasure=1\n"
    
            "[Mixer]\n"
            "MasterFeder=1\n"
            "MasterPanpot=2\n"
            "MasterMute=3\n"
            "OutputMode=4\n"
            "Tracks=1\n"
            "Feder0=5\n"
            "Panpot0=6\n"
            "Mute0=7\n"
            "Solo0=8\n"
    
            "[EventList]\n"
            "0=ID#0000\n"
            "240=ID#0001\n"
            "480=ID#0002\n"
            "720=ID#0003\n"
            "1920=ID#0004,ID#0005\n"
            "2400=EOS\n"
    
            "[ID#0000]\n"
            "Type=Singer\n"
            "IconHandle=h#0000\n"
    
            "[ID#0001]\n"
            "Type=Aicon\n"
            "IconHandle=h#0001\n"
            "Note#=64\n"
    
            "[ID#0002]\n"
            "Type=Aicon\n"
            "IconHandle=h#0002\n"
            "Note#=65\n"
    
            "[ID#0003]\n"
            "Type=Aicon\n"
            "IconHandle=h#0003\n"
            "Note#=66\n"
    
            "[ID#0004]\n"
            "Type=Singer\n"
            "IconHandle=h#0004\n"
    
            "[ID#0005]\n"
            "Type=Anote\n"
            "Length=480\n"
            "Note#=67\n"
            "Dynamics=68\n"
            "PMBendDepth=69\n"
            "PMBendLength=70\n"
            "PMbPortamentoUse=3\n"
            "DEMdecGainRate=71\n"
            "DEMaccent=72\n"
            "LyricHandle=h#0005\n"
            "VibratoHandle=h#0006\n"
            "VibratoDelay=73\n"
            "NoteHeadHandle=h#0007\n"
    
            "[h#0000]\n"
            "IconID=$07010002\n"
            "IDS=Miku\n"
            "Original=1\n"
            "Caption=caption for miku\n"
            "Length=0\n"
            "Language=1\n"
            "Program=2\n"
    
            "[h#0001]\n"
            "IconID=$05020001\n"
            "IDS=crescendo\n"
            "Original=1\n"
            "Caption=caption for crescendo\n"
            "StartDyn=4\n"
            "EndDyn=7\n"
            "Length=10\n"
            "DynBPNum=0\n"
    
            "[h#0002]\n"
            "IconID=$05010001\n"
            "IDS=dynaff\n"
            "Original=2\n"
            "Caption=caption for dynaff\n"
            "StartDyn=5\n"
            "EndDyn=8\n"
            "Length=11\n"
            "DynBPNum=0\n"
    
            "[h#0003]\n"
            "IconID=$05030001\n"
            "IDS=decrescendo\n"
            "Original=3\n"
            "Caption=caption for decrescendo\n"
            "StartDyn=6\n"
            "EndDyn=9\n"
            "Length=12\n"
            "DynBPNum=0\n"
    
            "[h#0004]\n"
            "IconID=$07020003\n"
            "IDS=Luka_EN\n"
            "Original=130\n"
            "Caption=caption for luka\n"
            "Length=0\n"
            "Language=2\n"
            "Program=3\n"
    
            "[h#0005]\n"
            "L0=\"ら\",\"4 a\",1,64,0,0\n"
    
            "[h#0006]\n"
            "IconID=$04040004\n"
            "IDS=vibrato\n"
            "Original=1\n"
            "Caption=caption for vibrato\n"
            "Length=407\n"
            "StartDepth=13\n"
            "DepthBPNum=0\n"
            "StartRate=14\n"
            "RateBPNum=0\n"
    
            "[h#0007]\n"
            "IconID=$05030000\n"
            "IDS=attack\n"
            "Original=15\n"
            "Caption=caption for attack\n"
            "Length=120\n"
            "Duration=62\n"
            "Depth=65\n"
    
            "[PitchBendBPList]\n"
            "480=0\n"
    
            "[PitchBendSensBPList]\n"
            "481=1\n"
    
            "[DynamicsBPList]\n"
            "482=2\n"
    
            "[EpRResidualBPList]\n"
            "483=3\n"
    
            "[EpRESlopeBPList]\n"
            "484=4\n"
    
            "[EpRESlopeDepthBPList]\n"
            "485=5\n"
    
            "[GenderFactorBPList]\n"
            "500=20\n"
    
            "[PortamentoTimingBPList]\n"
            "501=21\n"
    
            "[OpeningBPList]\n"
            "502=22\n";
    
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
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
/*
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
        local event = luavsq.Event.new( 480, EventType::NOTE );
        track.events.add( event );
        track.getCurve( "pit" ).add( 480, 100 );
        track.tag = "valueOfTag";
    
        local copy = track.clone();
        CPPUNIT_ASSERT_EQUAL( 2, copy.events.size() );
        CPPUNIT_ASSERT_EQUAL( 0, copy.events.get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( EventType::SINGER, copy.events.get( 0 ).type );
        CPPUNIT_ASSERT_EQUAL( "DummySingerName", copy.events.get( 0 ).singerHandle.ids );
        CPPUNIT_ASSERT_EQUAL( 480, copy.events.get( 1 ).clock );
        CPPUNIT_ASSERT_EQUAL( EventType::NOTE, copy.events.get( 1 ).type );
        CPPUNIT_ASSERT_EQUAL( 1, copy.getCurve( "pit" ).size() );
        CPPUNIT_ASSERT_EQUAL( 480, copy.getCurve( "pit" ).getKeyClock( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 100, copy.getCurve( "pit" ).get( 0 ).value );
        CPPUNIT_ASSERT_EQUAL( "DummyTrackName", copy.getName() );
        CPPUNIT_ASSERT_EQUAL( "valueOfTag", copy.tag );
*/
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
    CPPUNIT_TEST( testPrintMetaText );
    CPPUNIT_TEST( testChangeRenderer );
    CPPUNIT_TEST( testGetterAndSetterCurve );
    CPPUNIT_TEST( testClone );
    CPPUNIT_TEST( testGetLyricLength );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( TrackTest );

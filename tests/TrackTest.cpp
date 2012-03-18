#include "Util.h"
#include "../Track.hpp"

using namespace std;
using namespace VSQ_NS;

class TrackTest : public CppUnit::TestCase
{
public:
    void testConstructNormalTrack()
    {
        Track track( "DummyTrackName", "DummySingerName" );
        assertEqual( string( "DummyTrackName" ), track.getName() );
        assertEqual( 1, track.events.size() );
        assertEqual( EventType::SINGER, track.events.get( 0 ).type );
        assertEqual( string( "DummySingerName" ), track.events.get( 0 ).singerHandle.ids );
    
        assert_not_nil( track.common );
        assert_nil( track.master );
        assert_nil( track.mixer );
        assert_not_nil( track.events );
    
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
    
        CPPUNIT_ASSERT_EQUAL( "pit", track.getCurve( "pit" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "pit" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( -8192, track.getCurve( "pit" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 8191, track.getCurve( "pit" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "pbs", track.getCurve( "pbs" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 2, track.getCurve( "pbs" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "pbs" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 24, track.getCurve( "pbs" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "dyn", track.getCurve( "dyn" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "dyn" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "dyn" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "bre", track.getCurve( "bre" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "bre" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "bre" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "bre" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "bri", track.getCurve( "bri" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "bri" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "bri" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "bri" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "cle", track.getCurve( "cle" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "cle" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "cle" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "cle" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso1freq", track.getCurve( "reso1Freq" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso1Freq" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso1Freq" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso1Freq" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso2freq", track.getCurve( "reso2Freq" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso2Freq" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso2Freq" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso2Freq" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso3freq", track.getCurve( "reso3Freq" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso3Freq" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso3Freq" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso3Freq" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso4freq", track.getCurve( "reso4Freq" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso4Freq" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso4Freq" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso4Freq" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso1bw", track.getCurve( "reso1BW" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso1BW" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso1BW" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso1BW" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso2bw", track.getCurve( "reso2BW" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso2BW" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso2BW" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso2BW" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso3bw", track.getCurve( "reso3BW" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso3BW" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso3BW" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso3BW" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso4bw", track.getCurve( "reso4BW" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso4BW" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso4BW" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso4BW" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso1amp", track.getCurve( "reso1Amp" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso1Amp" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso1Amp" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso1Amp" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso2amp", track.getCurve( "reso2Amp" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso2Amp" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso2Amp" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso2Amp" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso3amp", track.getCurve( "reso3Amp" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso3Amp" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso3Amp" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso3Amp" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "reso4amp", track.getCurve( "reso4Amp" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "reso4Amp" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "reso4Amp" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "reso4Amp" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "harmonics", track.getCurve( "harmonics" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "harmonics" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "harmonics" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "harmonics" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "fx2depth", track.getCurve( "fx2depth" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "fx2depth" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "fx2depth" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "fx2depth" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "gen", track.getCurve( "gen" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "gen" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "gen" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "gen" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "por", track.getCurve( "por" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 64, track.getCurve( "por" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "por" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "por" ).getMaximum() );
        CPPUNIT_ASSERT_EQUAL( "ope", track.getCurve( "ope" ).getName() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "ope" ).getDefault() );
        CPPUNIT_ASSERT_EQUAL( 0, track.getCurve( "ope" ).getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 127, track.getCurve( "ope" ).getMaximum() );
    }
    
    void testConstructFromMidiEvents(){
    --    fail();
    }
    
    void testGetIndexIterator(){
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
        local iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.NOTE );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.SINGER );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( 0, iterator.next() );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testGetterAndSetterPlayMode(){
    --    fail();
    }
    
    void testGetterAndSetterTrackOn(){
    --    fail();
    }
    
    void testGetterAndSetterName(){
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
        CPPUNIT_ASSERT_EQUAL( "DummyTrackName", track.getName() );
        track.setName( "foo" );
        CPPUNIT_ASSERT_EQUAL( "foo", track.getName() );
    }
    
    void testGetPitchAt(){
    --    fail();
    }
    
    void testReflectDynamics(){
    --    fail();
    }
    
    void testGetSingerEventAt(){
    --    fail();
    }
    
    void testSortEvent(){
    --    fail();
    }
    
    void testGetIndexIteratorSinger(){
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
        local iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.SINGER );
        CPPUNIT_ASSERT( iterator.hasNext() );
        local event = track.events.get( iterator.next() );
        CPPUNIT_ASSERT_EQUAL( "DummySingerName", event.singerHandle.ids );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testGetIndexIteratorNote(){
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
        local iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.NOTE );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        local event = luavsq.Event.new( 480, luavsq.EventTypeEnum.NOTE );
        track.events.add( event, 10 );
        iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.NOTE );
        CPPUNIT_ASSERT( iterator.hasNext() );
        local obtained = track.events.get( iterator.next() );
        CPPUNIT_ASSERT_EQUAL( event, obtained );
        CPPUNIT_ASSERT_EQUAL( 10, obtained.id );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testGetIndexIteratorDynamics(){
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
        local iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.DYNAFF );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    
        local event = luavsq.Event.new( 480, luavsq.EventTypeEnum.ICON );
        event.iconDynamicsHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.DYNAMICS );
        event.iconDynamicsHandle.iconId = "$05019999";
        track.events.add( event, 10 );
        iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.DYNAFF );
        CPPUNIT_ASSERT( iterator.hasNext() );
        local obtained = track.events.get( iterator.next() );
        CPPUNIT_ASSERT_EQUAL( event, obtained );
        CPPUNIT_ASSERT_EQUAL( 10, obtained.id );
        CPPUNIT_ASSERT_EQUAL( "$05019999", obtained.iconDynamicsHandle.iconId );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testPrintMetaText(){
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
    
        local singerEvent = luavsq.Event.new( 0, luavsq.EventTypeEnum.SINGER );
        singerEvent.singerHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.SINGER ); --h#0000
        singerEvent.singerHandle.iconId = "$07010002";
        singerEvent.singerHandle.ids = "Miku";
        singerEvent.singerHandle.original = 1;
        singerEvent.singerHandle.setCaption( "caption for miku" );
        singerEvent.singerHandle.language = 1;
        singerEvent.singerHandle.program = 2;
        track.events.set( 0, singerEvent );
    
        local cresc}oEvent = luavsq.Event.new( 240, luavsq.EventTypeEnum.ICON );
        cresc}oEvent.note = 64;
        cresc}oEvent.iconDynamicsHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.DYNAMICS ); --h#0001
        cresc}oEvent.iconDynamicsHandle.iconId = "$05020001";
        cresc}oEvent.iconDynamicsHandle.ids = "cresc}o";
        cresc}oEvent.iconDynamicsHandle.original = 1;
        cresc}oEvent.iconDynamicsHandle.setCaption( "caption for cresc}o" );
        cresc}oEvent.iconDynamicsHandle.setStartDyn( 4 );
        cresc}oEvent.iconDynamicsHandle.setEndDyn( 7 );
        cresc}oEvent.setLength( 10 );
        track.events.add( cresc}oEvent, 2 );
    
        local dynaffEvent = luavsq.Event.new( 480, luavsq.EventTypeEnum.ICON );
        dynaffEvent.note = 65;
        dynaffEvent.iconDynamicsHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.DYNAMICS );--h#0002
        dynaffEvent.iconDynamicsHandle.iconId = "$05010001";
        dynaffEvent.iconDynamicsHandle.ids = "dynaff";
        dynaffEvent.iconDynamicsHandle.original = 2;
        dynaffEvent.iconDynamicsHandle.setCaption( "caption for dynaff" );
        dynaffEvent.iconDynamicsHandle.setStartDyn( 5 );
        dynaffEvent.iconDynamicsHandle.setEndDyn( 8 );
        dynaffEvent.setLength( 11 );
        track.events.add( dynaffEvent, 3 );
    
        local decresc}oEvent = luavsq.Event.new( 720, luavsq.EventTypeEnum.ICON );
        decresc}oEvent.note = 66;
        decresc}oEvent.iconDynamicsHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.DYNAMICS );--h#0003
        decresc}oEvent.iconDynamicsHandle.iconId = "$05030001";
        decresc}oEvent.iconDynamicsHandle.ids = "decresc}o";
        decresc}oEvent.iconDynamicsHandle.original = 3;
        decresc}oEvent.iconDynamicsHandle.setCaption( "caption for decresc}o" );
        decresc}oEvent.iconDynamicsHandle.setStartDyn( 6 );
        decresc}oEvent.iconDynamicsHandle.setEndDyn( 9 );
        decresc}oEvent.setLength( 12 );
        track.events.add( decresc}oEvent, 4 );
    
        local singerEvent2 = luavsq.Event.new( 1920, luavsq.EventTypeEnum.SINGER );
        singerEvent2.singerHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.SINGER );--h#0004
        singerEvent2.singerHandle.iconId = "$07020003";
        singerEvent2.singerHandle.ids = "Luka_EN";
        singerEvent2.singerHandle.original = 0x82;
        singerEvent2.singerHandle.setCaption( "caption for luka" );
        singerEvent2.singerHandle.language = 2;
        singerEvent2.singerHandle.program = 3;
        track.events.add( singerEvent2, 5 );
    
        local noteEvent = luavsq.Event.new( 1920, luavsq.EventTypeEnum.NOTE );
        noteEvent.note = 67;
        noteEvent.dynamics = 68;
        noteEvent.pmB}Depth = 69;
        noteEvent.pmB}Length = 70;
        noteEvent.pmbPortamentoUse = 3;
        noteEvent.demDecGainRate = 71;
        noteEvent.demAccent = 72;
        noteEvent.setLength( 480 );
        noteEvent.lyricHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.LYRIC );
        noteEvent.lyricHandle.setLyricAt( 0, luavsq.Lyric.new( "ら", "4 a" ) );--h#0005
        noteEvent.vibratoHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.VIBRATO );--h#0006
        noteEvent.vibratoDelay = 73;
        noteEvent.vibratoHandle.iconId ="$04040004";
        noteEvent.vibratoHandle.ids = "vibrato";
        noteEvent.vibratoHandle.original = 1;
        noteEvent.vibratoHandle.setCaption( "caption for vibrato" );
        noteEvent.vibratoHandle.setLength( 407 );
        noteEvent.vibratoHandle.setStartDepth( 13 );
        noteEvent.vibratoHandle.setStartRate( 14 );
        noteEvent.noteHeadHandle = luavsq.Handle.new( luavsq.HandleTypeEnum.NOTE_HEAD );--h#0007
        noteEvent.noteHeadHandle.iconId = "$05030000";
        noteEvent.noteHeadHandle.ids = "attack";
        noteEvent.noteHeadHandle.original = 15;
        noteEvent.noteHeadHandle.setCaption( "caption for attack" );
        noteEvent.noteHeadHandle.setLength( 120 );
        noteEvent.noteHeadHandle.setDuration( 62 );
        noteEvent.noteHeadHandle.setDepth( 65 );
        track.events.add( noteEvent, 6 );
    
        track.master = luavsq.Master.new( 1 );
    
        track.mixer = luavsq.Mixer.new( 1, 2, 3, 4 );
        track.mixer.slave = {};
        track.mixer.slave[1] = luavsq.MixerItem.new( 5, 6, 7, 8 );
    
        track.common.version = "DSB301";
        track.common.name = "foo";
        track.common.color = "1,2,3";
        track.common.dynamicsMode = luavsq.DynamicsModeEnum.STANDARD;
        track.common.playMode = luavsq.PlayModeEnum.PLAY_WITH_SYNTH;
    
        local curves = {
            "pit", "pbs", "dyn", "bre", "bri", "cle",
            "RESO1FREQ", "RESO2FREQ", "RESO3FREQ", "RESO4FREQ",
            "RESO1BW", "RESO2BW", "RESO3BW", "RESO4BW",
            "RESO1amp", "RESO2amp", "RESO3amp", "RESO4amp",
            "HARMONICS", "fx2depth", "GEN", "pOr", "OPE"
        };
        local i, curveName;
        for i, curveName in ipairs( curves ) do
            track.getCurve( curveName ).add( 480 + i, i );
        }
    
        local stream = luavsq.TextStream.new();
        track.printMetaText( stream, 2400, 0 );
        local expected =
            "[Common]\n" ..
            "Version=DSB301\n" ..
            "Name=foo\n" ..
            "Color=1,2,3\n" ..
            "DynamicsMode=0\n" ..
            "PlayMode=1\n" ..
    
            "[Master]\n" ..
            "PreMeasure=1\n" ..
    
            "[Mixer]\n" ..
            "MasterFeder=1\n" ..
            "MasterPanpot=2\n" ..
            "MasterMute=3\n" ..
            "OutputMode=4\n" ..
            "Tracks=1\n" ..
            "Feder0=5\n" ..
            "Panpot0=6\n" ..
            "Mute0=7\n" ..
            "Solo0=8\n" ..
    
            "[EventList]\n" ..
            "0=ID#0000\n" ..
            "240=ID#0001\n" ..
            "480=ID#0002\n" ..
            "720=ID#0003\n" ..
            "1920=ID#0004,ID#0005\n" ..
            "2400=EOS\n" ..
    
            "[ID#0000]\n" ..
            "Type=Singer\n" ..
            "IconHandle=h#0000\n" ..
    
            "[ID#0001]\n" ..
            "Type=Aicon\n" ..
            "IconHandle=h#0001\n" ..
            "Note#=64\n" ..
    
            "[ID#0002]\n" ..
            "Type=Aicon\n" ..
            "IconHandle=h#0002\n" ..
            "Note#=65\n" ..
    
            "[ID#0003]\n" ..
            "Type=Aicon\n" ..
            "IconHandle=h#0003\n" ..
            "Note#=66\n" ..
    
            "[ID#0004]\n" ..
            "Type=Singer\n" ..
            "IconHandle=h#0004\n" ..
    
            "[ID#0005]\n" ..
            "Type=Anote\n" ..
            "Length=480\n" ..
            "Note#=67\n" ..
            "Dynamics=68\n" ..
            "PMB}Depth=69\n" ..
            "PMB}Length=70\n" ..
            "PMbPortamentoUse=3\n" ..
            "DEMdecGainRate=71\n" ..
            "DEMaccent=72\n" ..
            "LyricHandle=h#0005\n" ..
            "VibratoHandle=h#0006\n" ..
            "VibratoDelay=73\n" ..
            "NoteHeadHandle=h#0007\n" ..
    
            "[h#0000]\n" ..
            "IconID=$07010002\n" ..
            "IDS=Miku\n" ..
            "Original=1\n" ..
            "Caption=caption for miku\n" ..
            "Length=0\n" ..
            "Language=1\n" ..
            "Program=2\n" ..
    
            "[h#0001]\n" ..
            "IconID=$05020001\n" ..
            "IDS=cresc}o\n" ..
            "Original=1\n" ..
            "Caption=caption for cresc}o\n" ..
            "StartDyn=4\n" ..
            "EndDyn=7\n" ..
            "Length=10\n" ..
            "DynBPNum=0\n" ..
    
            "[h#0002]\n" ..
            "IconID=$05010001\n" ..
            "IDS=dynaff\n" ..
            "Original=2\n" ..
            "Caption=caption for dynaff\n" ..
            "StartDyn=5\n" ..
            "EndDyn=8\n" ..
            "Length=11\n" ..
            "DynBPNum=0\n" ..
    
            "[h#0003]\n" ..
            "IconID=$05030001\n" ..
            "IDS=decresc}o\n" ..
            "Original=3\n" ..
            "Caption=caption for decresc}o\n" ..
            "StartDyn=6\n" ..
            "EndDyn=9\n" ..
            "Length=12\n" ..
            "DynBPNum=0\n" ..
    
            "[h#0004]\n" ..
            "IconID=$07020003\n" ..
            "IDS=Luka_EN\n" ..
            "Original=130\n" ..
            "Caption=caption for luka\n" ..
            "Length=0\n" ..
            "Language=2\n" ..
            "Program=3\n" ..
    
            "[h#0005]\n" ..
            "L0=\"ら\",\"4 a\",1,64,0,0\n" ..
    
            "[h#0006]\n" ..
            "IconID=$04040004\n" ..
            "IDS=vibrato\n" ..
            "Original=1\n" ..
            "Caption=caption for vibrato\n" ..
            "Length=407\n" ..
            "StartDepth=13\n" ..
            "DepthBPNum=0\n" ..
            "StartRate=14\n" ..
            "RateBPNum=0\n" ..
    
            "[h#0007]\n" ..
            "IconID=$05030000\n" ..
            "IDS=attack\n" ..
            "Original=15\n" ..
            "Caption=caption for attack\n" ..
            "Length=120\n" ..
            "Duration=62\n" ..
            "Depth=65\n" ..
    
            "[PitchB}BPList]\n" ..
            "481=1\n" ..
    
            "[PitchB}SensBPList]\n" ..
            "482=2\n" ..
    
            "[DynamicsBPList]\n" ..
            "483=3\n" ..
    
            "[EpRResidualBPList]\n" ..
            "484=4\n" ..
    
            "[EpRESlopeBPList]\n" ..
            "485=5\n" ..
    
            "[EpRESlopeDepthBPList]\n" ..
            "486=6\n" ..
    
            "[G}erFactorBPList]\n" ..
            "501=21\n" ..
    
            "[PortamentoTimingBPList]\n" ..
            "502=22\n" ..
    
            "[OpeningBPList]\n" ..
            "503=23\n";
    
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }
    
    void testChangeR}erer(){
    --    fail();
    }
    
    void testGetterAndSetterCurve(){
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
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
    }
    
    void testClone(){
        local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
        local event = luavsq.Event.new( 480, luavsq.EventTypeEnum.NOTE );
        track.events.add( event );
        track.getCurve( "pit" ).add( 480, 100 );
        track.tag = "valueOfTag";
    
        local copy = track.clone();
        CPPUNIT_ASSERT_EQUAL( 2, copy.events.size() );
        CPPUNIT_ASSERT_EQUAL( 0, copy.events.get( 0 ).clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.EventTypeEnum.SINGER, copy.events.get( 0 ).type );
        CPPUNIT_ASSERT_EQUAL( "DummySingerName", copy.events.get( 0 ).singerHandle.ids );
        CPPUNIT_ASSERT_EQUAL( 480, copy.events.get( 1 ).clock );
        CPPUNIT_ASSERT_EQUAL( luavsq.EventTypeEnum.NOTE, copy.events.get( 1 ).type );
        CPPUNIT_ASSERT_EQUAL( 1, copy.getCurve( "pit" ).size() );
        CPPUNIT_ASSERT_EQUAL( 480, copy.getCurve( "pit" ).getKeyClock( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 100, copy.getCurve( "pit" ).get( 0 ).value );
        CPPUNIT_ASSERT_EQUAL( "DummyTrackName", copy.getName() );
        CPPUNIT_ASSERT_EQUAL( "valueOfTag", copy.tag );
    }
    
    void testGetLyricLength(){
    --    fail();
    }

    CPPUNIT_TEST_SUITE( TrackTest );
    CPPUNIT_TEST( testConstructMasterTrack );
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

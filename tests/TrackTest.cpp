#include "Util.hpp"
#include "../include/libvsq/Track.hpp"

using namespace std;
using namespace vsq;

class TrackStub : public Track
{
public:
	explicit TrackStub(std::string name, std::string singer)
		: Track(name, singer)
	{
	}
};

TEST(TrackTest, testConstructNormalTrack)
{
	Track track("DummyTrackName", "DummySingerName");
	EXPECT_EQ(string("DummyTrackName"), track.name());
	Event::List& events = track.events();
	EXPECT_EQ(1, events.size());
	EXPECT_EQ(EventType::SINGER, events.get(0)->type());
	EXPECT_EQ(string("DummySingerName"), events.get(0)->singerHandle.ids);

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

	EXPECT_EQ(string("pit"), track.curve("pit")->name());
	EXPECT_EQ(0, track.curve("pit")->defaultValue());
	EXPECT_EQ(-8192, track.curve("pit")->minimum());
	EXPECT_EQ(8191, track.curve("pit")->maximum());
	EXPECT_EQ(string("pbs"), track.curve("pbs")->name());
	EXPECT_EQ(2, track.curve("pbs")->defaultValue());
	EXPECT_EQ(0, track.curve("pbs")->minimum());
	EXPECT_EQ(24, track.curve("pbs")->maximum());
	EXPECT_EQ(string("dyn"), track.curve("dyn")->name());
	EXPECT_EQ(0, track.curve("dyn")->minimum());
	EXPECT_EQ(127, track.curve("dyn")->maximum());
	EXPECT_EQ(string("bre"), track.curve("bre")->name());
	EXPECT_EQ(0, track.curve("bre")->defaultValue());
	EXPECT_EQ(0, track.curve("bre")->minimum());
	EXPECT_EQ(127, track.curve("bre")->maximum());
	EXPECT_EQ(string("bri"), track.curve("bri")->name());
	EXPECT_EQ(64, track.curve("bri")->defaultValue());
	EXPECT_EQ(0, track.curve("bri")->minimum());
	EXPECT_EQ(127, track.curve("bri")->maximum());
	EXPECT_EQ(string("cle"), track.curve("cle")->name());
	EXPECT_EQ(0, track.curve("cle")->defaultValue());
	EXPECT_EQ(0, track.curve("cle")->minimum());
	EXPECT_EQ(127, track.curve("cle")->maximum());
	EXPECT_EQ(string("reso1freq"), track.curve("reso1Freq")->name());
	EXPECT_EQ(64, track.curve("reso1Freq")->defaultValue());
	EXPECT_EQ(0, track.curve("reso1Freq")->minimum());
	EXPECT_EQ(127, track.curve("reso1Freq")->maximum());
	EXPECT_EQ(string("reso2freq"), track.curve("reso2Freq")->name());
	EXPECT_EQ(64, track.curve("reso2Freq")->defaultValue());
	EXPECT_EQ(0, track.curve("reso2Freq")->minimum());
	EXPECT_EQ(127, track.curve("reso2Freq")->maximum());
	EXPECT_EQ(string("reso3freq"), track.curve("reso3Freq")->name());
	EXPECT_EQ(64, track.curve("reso3Freq")->defaultValue());
	EXPECT_EQ(0, track.curve("reso3Freq")->minimum());
	EXPECT_EQ(127, track.curve("reso3Freq")->maximum());
	EXPECT_EQ(string("reso4freq"), track.curve("reso4Freq")->name());
	EXPECT_EQ(64, track.curve("reso4Freq")->defaultValue());
	EXPECT_EQ(0, track.curve("reso4Freq")->minimum());
	EXPECT_EQ(127, track.curve("reso4Freq")->maximum());
	EXPECT_EQ(string("reso1bw"), track.curve("reso1BW")->name());
	EXPECT_EQ(64, track.curve("reso1BW")->defaultValue());
	EXPECT_EQ(0, track.curve("reso1BW")->minimum());
	EXPECT_EQ(127, track.curve("reso1BW")->maximum());
	EXPECT_EQ(string("reso2bw"), track.curve("reso2BW")->name());
	EXPECT_EQ(64, track.curve("reso2BW")->defaultValue());
	EXPECT_EQ(0, track.curve("reso2BW")->minimum());
	EXPECT_EQ(127, track.curve("reso2BW")->maximum());
	EXPECT_EQ(string("reso3bw"), track.curve("reso3BW")->name());
	EXPECT_EQ(64, track.curve("reso3BW")->defaultValue());
	EXPECT_EQ(0, track.curve("reso3BW")->minimum());
	EXPECT_EQ(127, track.curve("reso3BW")->maximum());
	EXPECT_EQ(string("reso4bw"), track.curve("reso4BW")->name());
	EXPECT_EQ(64, track.curve("reso4BW")->defaultValue());
	EXPECT_EQ(0, track.curve("reso4BW")->minimum());
	EXPECT_EQ(127, track.curve("reso4BW")->maximum());
	EXPECT_EQ(string("reso1amp"), track.curve("reso1Amp")->name());
	EXPECT_EQ(64, track.curve("reso1Amp")->defaultValue());
	EXPECT_EQ(0, track.curve("reso1Amp")->minimum());
	EXPECT_EQ(127, track.curve("reso1Amp")->maximum());
	EXPECT_EQ(string("reso2amp"), track.curve("reso2Amp")->name());
	EXPECT_EQ(64, track.curve("reso2Amp")->defaultValue());
	EXPECT_EQ(0, track.curve("reso2Amp")->minimum());
	EXPECT_EQ(127, track.curve("reso2Amp")->maximum());
	EXPECT_EQ(string("reso3amp"), track.curve("reso3Amp")->name());
	EXPECT_EQ(64, track.curve("reso3Amp")->defaultValue());
	EXPECT_EQ(0, track.curve("reso3Amp")->minimum());
	EXPECT_EQ(127, track.curve("reso3Amp")->maximum());
	EXPECT_EQ(string("reso4amp"), track.curve("reso4Amp")->name());
	EXPECT_EQ(64, track.curve("reso4Amp")->defaultValue());
	EXPECT_EQ(0, track.curve("reso4Amp")->minimum());
	EXPECT_EQ(127, track.curve("reso4Amp")->maximum());
	EXPECT_EQ(string("harmonics"), track.curve("harmonics")->name());
	EXPECT_EQ(64, track.curve("harmonics")->defaultValue());
	EXPECT_EQ(0, track.curve("harmonics")->minimum());
	EXPECT_EQ(127, track.curve("harmonics")->maximum());
	EXPECT_EQ(string("fx2depth"), track.curve("fx2depth")->name());
	EXPECT_EQ(64, track.curve("fx2depth")->defaultValue());
	EXPECT_EQ(0, track.curve("fx2depth")->minimum());
	EXPECT_EQ(127, track.curve("fx2depth")->maximum());
	EXPECT_EQ(string("gen"), track.curve("gen")->name());
	EXPECT_EQ(64, track.curve("gen")->defaultValue());
	EXPECT_EQ(0, track.curve("gen")->minimum());
	EXPECT_EQ(127, track.curve("gen")->maximum());
	EXPECT_EQ(string("por"), track.curve("por")->name());
	EXPECT_EQ(64, track.curve("por")->defaultValue());
	EXPECT_EQ(0, track.curve("por")->minimum());
	EXPECT_EQ(127, track.curve("por")->maximum());
	EXPECT_EQ(string("ope"), track.curve("ope")->name());
	EXPECT_EQ(127, track.curve("ope")->defaultValue());
	EXPECT_EQ(0, track.curve("ope")->minimum());
	EXPECT_EQ(127, track.curve("ope")->maximum());
}

TEST(TrackTest, testConstructFromMidiEvents)
{
	//fail();
}

TEST(TrackTest, testGetIndexIterator)
{
	//TODO
	/*
	local track = luavsq.Track.new( "DummyTrackName", "DummySingerName" );
	local iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.NOTE );
	EXPECT_TRUE( false == iterator.hasNext() );

	iterator = track.getIndexIterator( luavsq.EventList.IndexIteratorKindEnum.SINGER );
	EXPECT_TRUE( iterator.hasNext() );
	EXPECT_EQ( 0, iterator.next() );
	EXPECT_TRUE( false == iterator.hasNext() );
	*/
}

/**
	* @todo
	*/
TEST(TrackTest, testGetterAndSetterPlayMode)
{
	//fail();
}

/**
	* @todo
	*/
TEST(TrackTest, testGetterAndSetterTrackOn)
{
	//fail();
}

TEST(TrackTest, testGetterAndSetterName)
{
	Track track("DummyTrackName", "DummySingerName");
	EXPECT_EQ(string("DummyTrackName"), track.name());
	track.name("foo");
	EXPECT_EQ(string("foo"), track.name());
}

/**
	* @todo
	*/
TEST(TrackTest, testGetPitchAt)
{
	//fail();
}

/**
	* @todo
	*/
TEST(TrackTest, testReflectDynamics)
{
	//fail();
}

TEST(TrackTest, testGetSingerEventAt)
{
	Track track("", "");
	track.events().clear();
	{
		const Event* actual = track.singerEventAt(0);
		EXPECT_TRUE(0 == actual);
	}

	Event singer1(0, EventType::SINGER);
	track.events().add(singer1, 1);

	Event note1(480, EventType::NOTE);
	track.events().add(note1, 2);

	Event singer2(480, EventType::SINGER);
	track.events().add(singer2, 3);

	{
		const Event* actual = track.singerEventAt(0);
		EXPECT_EQ(1, actual->id);
	}
	{
		const Event* actual = track.singerEventAt(479);
		EXPECT_EQ(1, actual->id);
	}
	{
		const Event* actual = track.singerEventAt(480);
		EXPECT_EQ(3, actual->id);
	}
	{
		const Event* actual = track.singerEventAt(10000);
		EXPECT_EQ(3, actual->id);
	}
	{
		const Event* actual = track.singerEventAt(-100);
		EXPECT_TRUE(0 == actual);
	}
}

/**
	* @todo
	*/
TEST(TrackTest, testSortEvent)
{
	//fail();
}

TEST(TrackTest, testGetIndexIteratorSinger)
{
	Track track("DummyTrackName", "DummySingerName");
	EventListIndexIterator iterator = track.getIndexIterator(EventListIndexIteratorKind::SINGER);
	EXPECT_TRUE(iterator.hasNext());
	const Event* event = track.events().get(iterator.next());
	EXPECT_EQ(string("DummySingerName"), event->singerHandle.ids);
	EXPECT_TRUE(false == iterator.hasNext());
}

TEST(TrackTest, testGetIndexIteratorNote)
{
	Track track("DummyTrackName", "DummySingerName");
	EventListIndexIterator iterator = track.getIndexIterator(EventListIndexIteratorKind::NOTE);
	EXPECT_TRUE(false == iterator.hasNext());

	Event event(480, EventType::NOTE);
	track.events().add(event, 10);
	iterator = track.getIndexIterator(EventListIndexIteratorKind::NOTE);
	EXPECT_TRUE(iterator.hasNext());
	const Event* obtained = track.events().get(iterator.next());
	//TODO: Event::equalが実装されたらコメントアウトを元に戻す
	//EXPECT_EQ( event, obtained );
	EXPECT_EQ(10, obtained->id);
	EXPECT_TRUE(false == iterator.hasNext());
}

TEST(TrackTest, testGetIndexIteratorDynamics)
{
	Track track("DummyTrackName", "DummySingerName");
	EventListIndexIterator iterator = track.getIndexIterator(EventListIndexIteratorKind::DYNAFF);
	EXPECT_TRUE(false == iterator.hasNext());

	Event event(480, EventType::ICON);
	event.iconDynamicsHandle = Handle(HandleType::DYNAMICS);
	event.iconDynamicsHandle.iconId = "$05019999";
	track.events().add(event, 10);
	iterator = track.getIndexIterator(EventListIndexIteratorKind::DYNAFF);
	EXPECT_TRUE(iterator.hasNext());
	const Event* obtained = track.events().get(iterator.next());
	//TODO: Event::equalが実装されたらコメントアウトを元に戻す
	//EXPECT_EQ( event, obtained );
	EXPECT_EQ(10, obtained->id);
	EXPECT_EQ(string("$05019999"), obtained->iconDynamicsHandle.iconId);
	EXPECT_TRUE(false == iterator.hasNext());
}

/**
	* @todo
	*/
TEST(TrackTest, testChangeRenderer)
{
	//fail();
}

/**
	* @todo
	*/
TEST(TrackTest, testGetterAndSetterCurve)
{
	/*
		    Track track( "DummyTrackName", "DummySingerName" );
		    EXPECT_EQ( track._pit, track.getCurve( "PiT" ) );
		    EXPECT_EQ( track._pbs, track.getCurve( "PBS" ) );
		    EXPECT_EQ( track._dyn, track.getCurve( "DYN" ) );
		    EXPECT_EQ( track._bre, track.getCurve( "bre" ) );
		    EXPECT_EQ( track._bri, track.getCurve( "BRI" ) )
		    EXPECT_EQ( track._cle, track.getCurve( "CLE" ) );
		    EXPECT_EQ( track._reso1FreqBPList, track.getCurve( "RESO1FREQ" ) );
		    EXPECT_EQ( track._reso2FreqBPList, track.getCurve( "RESO2FREQ" ) );
		    EXPECT_EQ( track._reso3FreqBPList, track.getCurve( "RESO3FREQ" ) );
		    EXPECT_EQ( track._reso4FreqBPList, track.getCurve( "RESO4FREQ" ) );
		    EXPECT_EQ( track._reso1BWBPList, track.getCurve( "RESO1BW" ) );
		    EXPECT_EQ( track._reso2BWBPList, track.getCurve( "RESO2BW" ) );
		    EXPECT_EQ( track._reso3BWBPList, track.getCurve( "RESO3BW" ) );
		    EXPECT_EQ( track._reso4BWBPList, track.getCurve( "RESO4BW" ) );
		    EXPECT_EQ( track._reso1AmpBPList, track.getCurve( "RESO1amp" ) );
		    EXPECT_EQ( track._reso2AmpBPList, track.getCurve( "RESO2amp" ) );
		    EXPECT_EQ( track._reso3AmpBPList, track.getCurve( "RESO3amp" ) );
		    EXPECT_EQ( track._reso4AmpBPList, track.getCurve( "RESO4amp" ) );
		    EXPECT_EQ( track._harmonics, track.getCurve( "HARMONICS" ) );
		    EXPECT_EQ( track._fx2depth, track.getCurve( "fx2depth" ) );
		    EXPECT_EQ( track._gen, track.getCurve( "GEN" ) );
		    EXPECT_EQ( track._por, track.getCurve( "pOr" ) );
		    EXPECT_EQ( track._ope, track.getCurve( "OPE" ) );
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
		                EXPECT_EQ( dummyList, track.getCurve( curves[j] ) );
		            }else{
		                assert_nil( track.getCurve( curves[j] ) );
		            }
		        }
		    }
	*/
}

TEST(TrackTest, testClone)
{
	Track track("DummyTrackName", "DummySingerName");
	Event event(480, EventType::NOTE);
	track.events().add(event);
	track.curve("pit")->add(480, 100);

	Track copy = track.clone();
	EXPECT_EQ(2, copy.events().size());
	EXPECT_EQ((tick_t)0, copy.events().get(0)->tick);
	EXPECT_EQ(EventType::SINGER, copy.events().get(0)->type());
	EXPECT_EQ(string("DummySingerName"), copy.events().get(0)->singerHandle.ids);
	EXPECT_EQ((tick_t)480, copy.events().get(1)->tick);
	EXPECT_EQ(EventType::NOTE, copy.events().get(1)->type());
	EXPECT_EQ(1, copy.curve("pit")->size());
	EXPECT_EQ((tick_t)480, copy.curve("pit")->keyTickAt(0));
	EXPECT_EQ(100, copy.curve("pit")->get(0).value);
	EXPECT_EQ(string("DummyTrackName"), copy.name());
}

/**
	* @todo
	*/
TEST(TrackTest, testGetLyricLength)
{
	//fail();
}

TEST(TrackTest, testCurveNameList)
{
	{
		Track track;
		track.common().version = "DSB200";
		const std::vector<std::string>* list = track.curveNameList();
		EXPECT_EQ((size_t)22, list->size());
		EXPECT_EQ(string("PIT"), list->at(0));
		EXPECT_EQ(string("PBS"), list->at(1));
		EXPECT_EQ(string("DYN"), list->at(2));
		EXPECT_EQ(string("BRE"), list->at(3));
		EXPECT_EQ(string("BRI"), list->at(4));
		EXPECT_EQ(string("CLE"), list->at(5));
		EXPECT_EQ(string("harmonics"), list->at(6));
		EXPECT_EQ(string("fx2depth"), list->at(7));
		EXPECT_EQ(string("reso1Freq"), list->at(8));
		EXPECT_EQ(string("reso2Freq"), list->at(9));
		EXPECT_EQ(string("reso3Freq"), list->at(10));
		EXPECT_EQ(string("reso4Freq"), list->at(11));
		EXPECT_EQ(string("reso1BW"), list->at(12));
		EXPECT_EQ(string("reso2BW"), list->at(13));
		EXPECT_EQ(string("reso3BW"), list->at(14));
		EXPECT_EQ(string("reso4BW"), list->at(15));
		EXPECT_EQ(string("reso1Amp"), list->at(16));
		EXPECT_EQ(string("reso2Amp"), list->at(17));
		EXPECT_EQ(string("reso3Amp"), list->at(18));
		EXPECT_EQ(string("reso4Amp"), list->at(19));
		EXPECT_EQ(string("GEN"), list->at(20));
		EXPECT_EQ(string("POR"), list->at(21));
	}

	{
		Track track;
		track.common().version = "DSB302";
		const std::vector<std::string>* list = track.curveNameList();
		EXPECT_EQ((size_t)9, list->size());
		EXPECT_EQ(string("PIT"), list->at(0));
		EXPECT_EQ(string("PBS"), list->at(1));
		EXPECT_EQ(string("DYN"), list->at(2));
		EXPECT_EQ(string("BRE"), list->at(3));
		EXPECT_EQ(string("BRI"), list->at(4));
		EXPECT_EQ(string("CLE"), list->at(5));
		EXPECT_EQ(string("GEN"), list->at(6));
		EXPECT_EQ(string("POR"), list->at(7));
		EXPECT_EQ(string("OPE"), list->at(8));
	}

	{
		Track track;
		track.common().version = "";
		const std::vector<std::string>* list = track.curveNameList();
		EXPECT_EQ((size_t)9, list->size());
		EXPECT_EQ(string("PIT"), list->at(0));
		EXPECT_EQ(string("PBS"), list->at(1));
		EXPECT_EQ(string("DYN"), list->at(2));
		EXPECT_EQ(string("BRE"), list->at(3));
		EXPECT_EQ(string("BRI"), list->at(4));
		EXPECT_EQ(string("CLE"), list->at(5));
		EXPECT_EQ(string("GEN"), list->at(6));
		EXPECT_EQ(string("POR"), list->at(7));
		EXPECT_EQ(string("OPE"), list->at(8));
	}
}

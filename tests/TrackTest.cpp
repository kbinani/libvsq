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

class TrackTest : public CppUnit::TestCase
{
public:
	void testConstructNormalTrack()
	{
		Track track("DummyTrackName", "DummySingerName");
		CPPUNIT_ASSERT_EQUAL(string("DummyTrackName"), track.name());
		Event::List& events = track.events();
		CPPUNIT_ASSERT_EQUAL(1, events.size());
		CPPUNIT_ASSERT_EQUAL(EventType::SINGER, events.get(0)->type());
		CPPUNIT_ASSERT_EQUAL(string("DummySingerName"), events.get(0)->singerHandle.ids);

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

		CPPUNIT_ASSERT_EQUAL(string("pit"), track.curve("pit")->name());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("pit")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(-8192, track.curve("pit")->minimum());
		CPPUNIT_ASSERT_EQUAL(8191, track.curve("pit")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("pbs"), track.curve("pbs")->name());
		CPPUNIT_ASSERT_EQUAL(2, track.curve("pbs")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("pbs")->minimum());
		CPPUNIT_ASSERT_EQUAL(24, track.curve("pbs")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("dyn"), track.curve("dyn")->name());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("dyn")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("dyn")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("bre"), track.curve("bre")->name());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("bre")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("bre")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("bre")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("bri"), track.curve("bri")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("bri")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("bri")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("bri")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("cle"), track.curve("cle")->name());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("cle")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("cle")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("cle")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso1freq"), track.curve("reso1Freq")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso1Freq")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso1Freq")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso1Freq")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso2freq"), track.curve("reso2Freq")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso2Freq")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso2Freq")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso2Freq")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso3freq"), track.curve("reso3Freq")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso3Freq")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso3Freq")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso3Freq")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso4freq"), track.curve("reso4Freq")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso4Freq")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso4Freq")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso4Freq")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso1bw"), track.curve("reso1BW")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso1BW")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso1BW")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso1BW")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso2bw"), track.curve("reso2BW")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso2BW")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso2BW")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso2BW")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso3bw"), track.curve("reso3BW")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso3BW")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso3BW")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso3BW")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso4bw"), track.curve("reso4BW")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso4BW")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso4BW")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso4BW")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso1amp"), track.curve("reso1Amp")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso1Amp")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso1Amp")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso1Amp")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso2amp"), track.curve("reso2Amp")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso2Amp")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso2Amp")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso2Amp")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso3amp"), track.curve("reso3Amp")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso3Amp")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso3Amp")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso3Amp")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("reso4amp"), track.curve("reso4Amp")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("reso4Amp")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("reso4Amp")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("reso4Amp")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("harmonics"), track.curve("harmonics")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("harmonics")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("harmonics")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("harmonics")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("fx2depth"), track.curve("fx2depth")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("fx2depth")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("fx2depth")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("fx2depth")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("gen"), track.curve("gen")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("gen")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("gen")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("gen")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("por"), track.curve("por")->name());
		CPPUNIT_ASSERT_EQUAL(64, track.curve("por")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("por")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("por")->maximum());
		CPPUNIT_ASSERT_EQUAL(string("ope"), track.curve("ope")->name());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("ope")->defaultValue());
		CPPUNIT_ASSERT_EQUAL(0, track.curve("ope")->minimum());
		CPPUNIT_ASSERT_EQUAL(127, track.curve("ope")->maximum());
	}

	void testConstructFromMidiEvents()
	{
		//fail();
	}

	void testGetIndexIterator()
	{
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
	void testGetterAndSetterPlayMode()
	{
		//fail();
	}

	/**
	 * @todo
	 */
	void testGetterAndSetterTrackOn()
	{
		//fail();
	}

	void testGetterAndSetterName()
	{
		Track track("DummyTrackName", "DummySingerName");
		CPPUNIT_ASSERT_EQUAL(string("DummyTrackName"), track.name());
		track.name("foo");
		CPPUNIT_ASSERT_EQUAL(string("foo"), track.name());
	}

	/**
	 * @todo
	 */
	void testGetPitchAt()
	{
		//fail();
	}

	/**
	 * @todo
	 */
	void testReflectDynamics()
	{
		//fail();
	}

	void testGetSingerEventAt()
	{
		Track track("", "");
		track.events().clear();
		{
			const Event* actual = track.singerEventAt(0);
			CPPUNIT_ASSERT(0 == actual);
		}

		Event singer1(0, EventType::SINGER);
		track.events().add(singer1, 1);

		Event note1(480, EventType::NOTE);
		track.events().add(note1, 2);

		Event singer2(480, EventType::SINGER);
		track.events().add(singer2, 3);

		{
			const Event* actual = track.singerEventAt(0);
			CPPUNIT_ASSERT_EQUAL(1, actual->id);
		}
		{
			const Event* actual = track.singerEventAt(479);
			CPPUNIT_ASSERT_EQUAL(1, actual->id);
		}
		{
			const Event* actual = track.singerEventAt(480);
			CPPUNIT_ASSERT_EQUAL(3, actual->id);
		}
		{
			const Event* actual = track.singerEventAt(10000);
			CPPUNIT_ASSERT_EQUAL(3, actual->id);
		}
		{
			const Event* actual = track.singerEventAt(-100);
			CPPUNIT_ASSERT(0 == actual);
		}
	}

	/**
	 * @todo
	 */
	void testSortEvent()
	{
		//fail();
	}

	void testGetIndexIteratorSinger()
	{
		Track track("DummyTrackName", "DummySingerName");
		EventListIndexIterator iterator = track.getIndexIterator(EventListIndexIteratorKind::SINGER);
		CPPUNIT_ASSERT(iterator.hasNext());
		const Event* event = track.events().get(iterator.next());
		CPPUNIT_ASSERT_EQUAL(string("DummySingerName"), event->singerHandle.ids);
		CPPUNIT_ASSERT(false == iterator.hasNext());
	}

	void testGetIndexIteratorNote()
	{
		Track track("DummyTrackName", "DummySingerName");
		EventListIndexIterator iterator = track.getIndexIterator(EventListIndexIteratorKind::NOTE);
		CPPUNIT_ASSERT(false == iterator.hasNext());

		Event event(480, EventType::NOTE);
		track.events().add(event, 10);
		iterator = track.getIndexIterator(EventListIndexIteratorKind::NOTE);
		CPPUNIT_ASSERT(iterator.hasNext());
		const Event* obtained = track.events().get(iterator.next());
		//TODO: Event::equalが実装されたらコメントアウトを元に戻す
		//CPPUNIT_ASSERT_EQUAL( event, obtained );
		CPPUNIT_ASSERT_EQUAL(10, obtained->id);
		CPPUNIT_ASSERT(false == iterator.hasNext());
	}

	void testGetIndexIteratorDynamics()
	{
		Track track("DummyTrackName", "DummySingerName");
		EventListIndexIterator iterator = track.getIndexIterator(EventListIndexIteratorKind::DYNAFF);
		CPPUNIT_ASSERT(false == iterator.hasNext());

		Event event(480, EventType::ICON);
		event.iconDynamicsHandle = Handle(HandleType::DYNAMICS);
		event.iconDynamicsHandle.iconId = "$05019999";
		track.events().add(event, 10);
		iterator = track.getIndexIterator(EventListIndexIteratorKind::DYNAFF);
		CPPUNIT_ASSERT(iterator.hasNext());
		const Event* obtained = track.events().get(iterator.next());
		//TODO: Event::equalが実装されたらコメントアウトを元に戻す
		//CPPUNIT_ASSERT_EQUAL( event, obtained );
		CPPUNIT_ASSERT_EQUAL(10, obtained->id);
		CPPUNIT_ASSERT_EQUAL(string("$05019999"), obtained->iconDynamicsHandle.iconId);
		CPPUNIT_ASSERT(false == iterator.hasNext());
	}

	/**
	 * @todo
	 */
	void testChangeRenderer()
	{
		//fail();
	}

	/**
	 * @todo
	 */
	void testGetterAndSetterCurve()
	{
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

	void testClone()
	{
		Track track("DummyTrackName", "DummySingerName");
		Event event(480, EventType::NOTE);
		track.events().add(event);
		track.curve("pit")->add(480, 100);

		Track copy = track.clone();
		CPPUNIT_ASSERT_EQUAL(2, copy.events().size());
		CPPUNIT_ASSERT_EQUAL((tick_t)0, copy.events().get(0)->tick);
		CPPUNIT_ASSERT_EQUAL(EventType::SINGER, copy.events().get(0)->type());
		CPPUNIT_ASSERT_EQUAL(string("DummySingerName"), copy.events().get(0)->singerHandle.ids);
		CPPUNIT_ASSERT_EQUAL((tick_t)480, copy.events().get(1)->tick);
		CPPUNIT_ASSERT_EQUAL(EventType::NOTE, copy.events().get(1)->type());
		CPPUNIT_ASSERT_EQUAL(1, copy.curve("pit")->size());
		CPPUNIT_ASSERT_EQUAL((tick_t)480, copy.curve("pit")->keyTickAt(0));
		CPPUNIT_ASSERT_EQUAL(100, copy.curve("pit")->get(0).value);
		CPPUNIT_ASSERT_EQUAL(string("DummyTrackName"), copy.name());
	}

	/**
	 * @todo
	 */
	void testGetLyricLength()
	{
		//fail();
	}

	void testCurveNameList()
	{
		{
			Track track;
			track.common().version = "DSB200";
			const std::vector<std::string>* list = track.curveNameList();
			CPPUNIT_ASSERT_EQUAL((size_t)22, list->size());
			CPPUNIT_ASSERT_EQUAL(string("PIT"), list->at(0));
			CPPUNIT_ASSERT_EQUAL(string("PBS"), list->at(1));
			CPPUNIT_ASSERT_EQUAL(string("DYN"), list->at(2));
			CPPUNIT_ASSERT_EQUAL(string("BRE"), list->at(3));
			CPPUNIT_ASSERT_EQUAL(string("BRI"), list->at(4));
			CPPUNIT_ASSERT_EQUAL(string("CLE"), list->at(5));
			CPPUNIT_ASSERT_EQUAL(string("harmonics"), list->at(6));
			CPPUNIT_ASSERT_EQUAL(string("fx2depth"), list->at(7));
			CPPUNIT_ASSERT_EQUAL(string("reso1Freq"), list->at(8));
			CPPUNIT_ASSERT_EQUAL(string("reso2Freq"), list->at(9));
			CPPUNIT_ASSERT_EQUAL(string("reso3Freq"), list->at(10));
			CPPUNIT_ASSERT_EQUAL(string("reso4Freq"), list->at(11));
			CPPUNIT_ASSERT_EQUAL(string("reso1BW"), list->at(12));
			CPPUNIT_ASSERT_EQUAL(string("reso2BW"), list->at(13));
			CPPUNIT_ASSERT_EQUAL(string("reso3BW"), list->at(14));
			CPPUNIT_ASSERT_EQUAL(string("reso4BW"), list->at(15));
			CPPUNIT_ASSERT_EQUAL(string("reso1Amp"), list->at(16));
			CPPUNIT_ASSERT_EQUAL(string("reso2Amp"), list->at(17));
			CPPUNIT_ASSERT_EQUAL(string("reso3Amp"), list->at(18));
			CPPUNIT_ASSERT_EQUAL(string("reso4Amp"), list->at(19));
			CPPUNIT_ASSERT_EQUAL(string("GEN"), list->at(20));
			CPPUNIT_ASSERT_EQUAL(string("POR"), list->at(21));
		}

		{
			Track track;
			track.common().version = "DSB302";
			const std::vector<std::string>* list = track.curveNameList();
			CPPUNIT_ASSERT_EQUAL((size_t)9, list->size());
			CPPUNIT_ASSERT_EQUAL(string("PIT"), list->at(0));
			CPPUNIT_ASSERT_EQUAL(string("PBS"), list->at(1));
			CPPUNIT_ASSERT_EQUAL(string("DYN"), list->at(2));
			CPPUNIT_ASSERT_EQUAL(string("BRE"), list->at(3));
			CPPUNIT_ASSERT_EQUAL(string("BRI"), list->at(4));
			CPPUNIT_ASSERT_EQUAL(string("CLE"), list->at(5));
			CPPUNIT_ASSERT_EQUAL(string("GEN"), list->at(6));
			CPPUNIT_ASSERT_EQUAL(string("POR"), list->at(7));
			CPPUNIT_ASSERT_EQUAL(string("OPE"), list->at(8));
		}

		{
			Track track;
			track.common().version = "";
			const std::vector<std::string>* list = track.curveNameList();
			CPPUNIT_ASSERT_EQUAL((size_t)9, list->size());
			CPPUNIT_ASSERT_EQUAL(string("PIT"), list->at(0));
			CPPUNIT_ASSERT_EQUAL(string("PBS"), list->at(1));
			CPPUNIT_ASSERT_EQUAL(string("DYN"), list->at(2));
			CPPUNIT_ASSERT_EQUAL(string("BRE"), list->at(3));
			CPPUNIT_ASSERT_EQUAL(string("BRI"), list->at(4));
			CPPUNIT_ASSERT_EQUAL(string("CLE"), list->at(5));
			CPPUNIT_ASSERT_EQUAL(string("GEN"), list->at(6));
			CPPUNIT_ASSERT_EQUAL(string("POR"), list->at(7));
			CPPUNIT_ASSERT_EQUAL(string("OPE"), list->at(8));
		}
	}

	CPPUNIT_TEST_SUITE(TrackTest);
	CPPUNIT_TEST(testConstructNormalTrack);
	CPPUNIT_TEST(testConstructFromMidiEvents);
	CPPUNIT_TEST(testGetIndexIterator);
	CPPUNIT_TEST(testGetterAndSetterPlayMode);
	CPPUNIT_TEST(testGetterAndSetterTrackOn);
	CPPUNIT_TEST(testGetterAndSetterName);
	CPPUNIT_TEST(testGetPitchAt);
	CPPUNIT_TEST(testReflectDynamics);
	CPPUNIT_TEST(testGetSingerEventAt);
	CPPUNIT_TEST(testSortEvent);
	CPPUNIT_TEST(testGetIndexIteratorSinger);
	CPPUNIT_TEST(testGetIndexIteratorNote);
	CPPUNIT_TEST(testGetIndexIteratorDynamics);
	CPPUNIT_TEST(testChangeRenderer);
	CPPUNIT_TEST(testGetterAndSetterCurve);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST(testGetLyricLength);
	CPPUNIT_TEST(testCurveNameList);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(TrackTest);

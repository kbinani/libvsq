#include "Util.hpp"
#include "../include/libvsq/Event.hpp"

using namespace std;
using namespace vsq;

static Event getNoteEvent()
{
	Event noteEvent(0, EventType::NOTE);
	noteEvent.length(2);
	noteEvent.note = 6;
	noteEvent.dynamics = 21;
	noteEvent.pmBendDepth = 4;
	noteEvent.pmBendLength = 5;
	noteEvent.pmbPortamentoUse = 3;
	noteEvent.demDecGainRate = 7;
	noteEvent.demAccent = 8;
	noteEvent.vibratoDelay = 13;
	noteEvent.lyricHandle.index = 1;
	return noteEvent;
}

static Event getSingerEvent()
{
	Event singerEvent(0, EventType::SINGER);
	singerEvent.singerHandle = Handle(HandleType::SINGER);
	singerEvent.singerHandle.index = 16;
	return singerEvent;
}

static Event getIconEvent()
{
	Event iconEvent(0, EventType::ICON);
	iconEvent.note = 19;
	return iconEvent;
}

TEST(EventTest, testConstruct)
{
	Event event = Event::eos();
	EXPECT_EQ((tick_t)0, event.tick);
	EXPECT_EQ(0, event.id);
	EXPECT_TRUE(event.isEOS());
}

TEST(EventTest, testConstructWithTickAndId)
{
	Event event(1, EventType::NOTE);

	EXPECT_EQ((tick_t)1, event.tick);
	EXPECT_EQ(EventType::NOTE, event.type());
	EXPECT_TRUE(false == event.isEOS());
}

TEST(EventTest, testEquals)
{
	//    fail();
}

TEST(EventTest, testCompareTo)
{
	Event singerEvent(1920, EventType::SINGER);
	Event noteEvent(1920, EventType::NOTE);
	EXPECT_EQ(0, singerEvent.compareTo(singerEvent));
	EXPECT_TRUE(0 > singerEvent.compareTo(noteEvent));
	EXPECT_TRUE(0 < noteEvent.compareTo(singerEvent));

	singerEvent.tick = 2000;
	noteEvent.tick = 1920;
	EXPECT_TRUE(0 < singerEvent.compareTo(noteEvent));

	singerEvent.tick = 2000;
	noteEvent.tick = 2001;
	EXPECT_EQ(1, noteEvent.compareTo(singerEvent));
}

TEST(EventTest, testCompare)
{
	Event singerEvent = getSingerEvent();
	singerEvent.tick = 1920;
	Event noteEvent = getNoteEvent();
	noteEvent.tick = 1920;
	EXPECT_TRUE(false == Event::compare(singerEvent, singerEvent));
	EXPECT_TRUE(Event::compare(singerEvent, noteEvent));
	EXPECT_TRUE(false == Event::compare(noteEvent, singerEvent));

	singerEvent.tick = 2000;
	noteEvent.tick = 1920;
	EXPECT_TRUE(false == Event::compare(singerEvent, noteEvent));

	singerEvent.tick = 2000;
	noteEvent.tick = 2001;
	EXPECT_TRUE(false == Event::compare(noteEvent, singerEvent));
}

TEST(EventTest, testClone)
{
	Event event = getSingerEvent();
	event.tick = 40;
	event.id = 4;
	event.singerHandle = Handle(HandleType::SINGER);
	event.singerHandle.index = 12;
	Event copy = event.clone();
	EXPECT_EQ((tick_t)40, copy.tick);
	EXPECT_EQ(4, copy.id);
	EXPECT_EQ(12, copy.singerHandle.index);

	Event id(0, EventType::NOTE);
	id.note = 6;
	id.dynamics = 7;
	id.pmBendDepth = 8;
	id.pmBendLength = 9;
	id.pmbPortamentoUse = 10;
	id.demDecGainRate = 11;
	id.demAccent = 12;
	id.vibratoDelay = 13;
	id.pMeanOnsetFirstNote = 14;
	id.vMeanNoteTransition = 15;
	id.d4mean = 16;
	id.pMeanEndingNote = 17;
	//assert_nil( id.singerHandle );
	//assert_not_nil( id.lyricHandle );
	//assert_nil( id.vibratoHandle );
	//assert_nil( id.noteHeadHandle );
	//assert_nil( id.iconDynamicsHandle );

	copy = id.clone();
	EXPECT_EQ(EventType::NOTE, copy.type());
	EXPECT_EQ(6, copy.note);
	EXPECT_EQ(7, copy.dynamics);
	EXPECT_EQ(8, copy.pmBendDepth);
	EXPECT_EQ(9, copy.pmBendLength);
	EXPECT_EQ(10, copy.pmbPortamentoUse);
	EXPECT_EQ(11, copy.demDecGainRate);
	EXPECT_EQ(12, copy.demAccent);
	EXPECT_EQ(13, copy.vibratoDelay);
	EXPECT_EQ(14, copy.pMeanOnsetFirstNote);
	EXPECT_EQ(15, copy.vMeanNoteTransition);
	EXPECT_EQ(16, copy.d4mean);
	EXPECT_EQ(17, copy.pMeanEndingNote);

	Handle iconHandle(HandleType::SINGER);
	iconHandle.caption = "foo";
	id.singerHandle = iconHandle;
	Handle lyricHandle(HandleType::LYRIC);
	lyricHandle.index = 102;
	id.lyricHandle = lyricHandle;
	Handle vibratoHandle(HandleType::VIBRATO);
	vibratoHandle.iconId = "aho";
	id.vibratoHandle = vibratoHandle;
	Handle noteHeadHandle(HandleType::NOTE_HEAD);
	noteHeadHandle.ids = "baka";
	id.noteHeadHandle = noteHeadHandle;
	Handle iconDynamicsHandle(HandleType::DYNAMICS);
	iconDynamicsHandle.startDyn = 183635;
	id.iconDynamicsHandle = iconDynamicsHandle;

	copy = id.clone();
	EXPECT_EQ(string("foo"), copy.singerHandle.caption);
	EXPECT_EQ(102, copy.lyricHandle.index);
	EXPECT_EQ(string("aho"), copy.vibratoHandle.iconId);
	EXPECT_EQ(string("baka"), copy.noteHeadHandle.ids);
	EXPECT_EQ(183635, copy.iconDynamicsHandle.startDyn);
}

TEST(EventTest, testIsEos)
{
	Event eos = Event::eos();
	EXPECT_TRUE(eos.isEOS());
}

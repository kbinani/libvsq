#include "Util.hpp"
#include "../include/libvsq/Handle.hpp"

using namespace std;
using namespace vsq;

TEST(HandleTest, testConstructIconDynamicsHandle)
{
	Handle handle(HandleType::DYNAMICS);
	EXPECT_EQ(ArticulationType::DYNAFF, handle.articulation());
}

TEST(HandleTest, testConstructNoteHeadHandle)
{
	Handle handle(HandleType::NOTE_HEAD);
	EXPECT_EQ(ArticulationType::NOTE_ATTACK, handle.articulation());
}

TEST(HandleTest, testIsDynaffType)
{
	Handle handle(HandleType::DYNAMICS);
	handle.iconId = "";
	EXPECT_TRUE(false == handle.isDynaffType());
	handle.iconId = "$05000000";
	EXPECT_TRUE(false == handle.isDynaffType());
	handle.iconId = "$05010000";
	EXPECT_TRUE(handle.isDynaffType());
}

TEST(HandleTest, testIsCrescendType)
{
	Handle handle(HandleType::DYNAMICS);
	handle.iconId = "";
	EXPECT_TRUE(false == handle.isCrescendType());
	handle.iconId = "$05000000";
	EXPECT_TRUE(false == handle.isCrescendType());
	handle.iconId = "$05020000";
	EXPECT_TRUE(handle.isCrescendType());
}

TEST(HandleTest, testIsDecrescendType)
{
	Handle handle(HandleType::DYNAMICS);
	handle.iconId = "";
	EXPECT_TRUE(false == handle.isDecrescendType());
	handle.iconId = "$05000000";
	EXPECT_TRUE(false == handle.isDecrescendType());
	handle.iconId = "$05030000";
	EXPECT_TRUE(handle.isDecrescendType());
}

TEST(HandleTest, testGetterAndSetterLength)
{
	Handle handle(HandleType::VIBRATO);
	tick_t expected = 847;
	EXPECT_TRUE(expected != handle.length());
	handle.length(expected);
	EXPECT_EQ(expected, handle.length());
}

TEST(HandleTest, testGetHandleIndexFromString)
{
	EXPECT_EQ(2, Handle::getHandleIndexFromString("h#0002"));
}

TEST(HandleTest, testGetDisplayString)
{
	Handle handle(HandleType::NOTE_HEAD);
	handle.ids = "goo";
	handle.caption = "gle";
	EXPECT_EQ(string("google"), handle.displayString());
}

TEST(HandleTest, testGetterAndSetterLyric)
{
	Handle handle(HandleType::LYRIC);
	handle.set(0, Lyric("は", "h a"));
	Lyric lyric("ら", "4 a");
	handle.set(1, lyric);
	EXPECT_EQ(2, handle.size());
	EXPECT_TRUE(handle.get(1).equals(lyric));
}

TEST(HandleTest, testCloneIconDynamicsHandle)
{
	Handle handle(HandleType::DYNAMICS);
	handle.iconId = "$05010000";
	handle.ids = "foo";
	handle.original = 1;
	handle.caption = "bar";
	handle.startDyn = 2;
	handle.endDyn = 3;
	handle.length(4);
	Handle copy = handle.clone();
	EXPECT_EQ(string("$05010000"), copy.iconId);
	EXPECT_EQ(string("foo"), copy.ids);
	EXPECT_EQ(1, copy.original);
	EXPECT_EQ(string("bar"), copy.caption);
	EXPECT_EQ(2, copy.startDyn);
	EXPECT_EQ(3, copy.endDyn);
	EXPECT_EQ((tick_t)4, copy.length());
	EXPECT_EQ(0, copy.dynBP.size());

	handle.dynBP = VibratoBPList("2", "0.0,1.0", "1,64");
	copy = handle.clone();
	EXPECT_EQ(string("0=1,1=64"), copy.dynBP.data());
}

TEST(HandleTest, testCloneLyricHandle)
{
	Handle handle(HandleType::LYRIC);
	handle.set(0, Lyric("ら", "4 a"));
	handle.index = 10;
	Handle copy = handle.clone();
	EXPECT_EQ(handle.index, copy.index);
	Lyric original = handle.get(0);
	Lyric copied = copy.get(0);
	EXPECT_TRUE(original.equals(copied));
}

TEST(HandleTest, testCloneNoteHeadHandle)
{
	Handle handle(HandleType::NOTE_HEAD);
	handle.index = 1;
	handle.iconId = "$05010000";
	handle.ids = "dwango";
	handle.original = 2;
	handle.caption = "niwango";
	handle.length(3);
	handle.duration = 4;
	handle.depth = 5;

	Handle copy = handle.clone();
	EXPECT_EQ(1, copy.index);
	EXPECT_EQ(string("$05010000"), copy.iconId);
	EXPECT_EQ(string("dwango"), copy.ids);
	EXPECT_EQ(2, copy.original);
	EXPECT_EQ(string("niwango"), copy.caption);
	EXPECT_EQ((tick_t)3, copy.length());
	EXPECT_EQ(4, copy.duration);
	EXPECT_EQ(5, copy.depth);
}

TEST(HandleTest, testCloneVibratoHandle)
{
	Handle handle(HandleType::VIBRATO);
	handle.index = 1;
	handle.iconId = "hahaha";
	handle.ids = "baka";
	handle.original = 2;
	handle.caption = "aho";
	handle.length(3);
	handle.startDepth = 4;
	handle.depthBP = VibratoBPList("2", "0.0,1.0", "32,56");
	handle.startRate = 5;
	handle.rateBP = VibratoBPList("2", "0.0,1.0", "64,128");
	Handle copy = handle.clone();
	EXPECT_EQ(1, copy.index);
	EXPECT_EQ(string("hahaha"), copy.iconId);
	EXPECT_EQ(string("baka"), copy.ids);
	EXPECT_EQ(2, copy.original);
	EXPECT_EQ(string("aho"), copy.caption);
	EXPECT_EQ((tick_t)3, copy.length());
	EXPECT_EQ(4, copy.startDepth);
	EXPECT_EQ(string("0=32,1=56"), copy.depthBP.data());
	EXPECT_EQ(5, copy.startRate);
	EXPECT_EQ(string("0=64,1=128"), copy.rateBP.data());
}

TEST(HandleTest, testCloneSingerHandle)
{
	Handle handle(HandleType::SINGER);
	handle.caption = "bar";
	handle.iconId = "$07010001";
	handle.ids = "foo";
	handle.index = 1;
	handle.length(2);
	handle.original = 3;
	handle.program = 4;
	handle.language = 5;

	Handle copy = handle.clone();
	EXPECT_EQ(handle.caption, copy.caption);
	EXPECT_EQ(handle.iconId, copy.iconId);
	EXPECT_EQ(handle.ids, copy.ids);
	EXPECT_EQ(handle.index, copy.index);
	EXPECT_EQ(handle.language, copy.language);
	EXPECT_EQ(handle.length(), copy.length());
	EXPECT_EQ(handle.original, copy.original);
	EXPECT_EQ(handle.program, copy.program);
}

TEST(HandleTest, testAddLyric)
{
	Handle handle(HandleType::LYRIC);
	handle.add(Lyric("ら", "4 a"));

	EXPECT_EQ(1, handle.size());
	EXPECT_EQ(string("ら"), handle.get(0).phrase);
	EXPECT_EQ(string("4 a"), handle.get(0).phoneticSymbol());
}

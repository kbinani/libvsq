#include "../include/libvsq/Lyric.hpp"
#include "Util.hpp"

using namespace std;
using namespace vsq;

TEST(LyricTest, testConstructWithPhrase)
{
	Lyric lyric = Lyric("は", "h a");
	EXPECT_EQ(string("は"), lyric.phrase);
	EXPECT_EQ(string("h a"), lyric.phoneticSymbol());
	EXPECT_EQ(string("64,0"), lyric.consonantAdjustment());
	EXPECT_TRUE(false == lyric.isProtected);
}

TEST(LyricTest, testGetPhoneticSymbol)
{
	Lyric lyric = Lyric("は", "h a");
	string actual = lyric.phoneticSymbol();
	EXPECT_EQ(string("h a"), actual);
}

TEST(LyricTest, testGetPhoneticSymbolList)
{
	Lyric lyric = Lyric("は", "h a");
	vector<string> actual = lyric.phoneticSymbolList();
	EXPECT_EQ((vector<string>::size_type)2, actual.size());
	EXPECT_EQ(string("h"), actual[0]);
	EXPECT_EQ(string("a"), actual[1]);
}

TEST(LyricTest, testSetPhoneticSymbol)
{
	Lyric lyric = Lyric("あ", "a");
	lyric.phoneticSymbol("h a");
	vector<string> actual = lyric.phoneticSymbolList();
	EXPECT_EQ(2, (int)actual.size());
	EXPECT_EQ(string("h"), actual[0]);
	EXPECT_EQ(string("a"), actual[1]);
}

TEST(LyricTest, testGetConsonantAdjustmentList)
{
	Lyric lyric = Lyric("は", "h a");
	lyric.lengthRatio = 1;
	lyric.consonantAdjustmentList({64, 0});
	lyric.isProtected = false;
	const vector<int> actual = lyric.consonantAdjustmentList();
	EXPECT_EQ(2, (int)actual.size());
	EXPECT_EQ(64, actual[0]);
	EXPECT_EQ(0, actual[1]);
}

TEST(LyricTest, testGetConsonantAdjustmentListWithNil)
{
	Lyric lyric = Lyric("は", "h a");
	lyric.lengthRatio = 1;
	lyric.consonantAdjustmentList({32, 16});
	lyric.isProtected = false;
	vector<int> actual = lyric.consonantAdjustmentList();
	EXPECT_EQ(2, (int)actual.size());
	EXPECT_EQ(32, actual[0]);
	EXPECT_EQ(16, actual[1]);

	lyric.consonantAdjustmentList(std::vector<int>());
	actual = lyric.consonantAdjustmentList();
	EXPECT_EQ(2, (int)actual.size());
	EXPECT_EQ(64, actual[0]);
	EXPECT_EQ(0, actual[1]);
}

TEST(LyricTest, testGetConsonantAdjustment)
{
	Lyric lyric = Lyric("は", "h a");
	lyric.lengthRatio = 1;
	lyric.consonantAdjustmentList({64, 0});
	lyric.isProtected = false;
	string actual = lyric.consonantAdjustment();
	EXPECT_EQ(string("64,0"), actual);
}

TEST(LyricTest, testSetConsonantAdjustment)
{
	Lyric lyric = Lyric("は", "h a");
	lyric.consonantAdjustment("61,0");
	EXPECT_EQ(string("61,0"), lyric.consonantAdjustment());
}

TEST(LyricTest, testToString)
{
	string expected = "は,h a,1,64,0,0";
	Lyric lyric = Lyric("は", "h a");
	lyric.lengthRatio = 1.0;
	lyric.consonantAdjustment("64,0");
	lyric.isProtected = false;
	EXPECT_EQ(expected, lyric.toString());
}

TEST(LyricTest, testEquals)
{
	Lyric a = Lyric("は", "h a");//,1,64,0,0");
	Lyric b = Lyric("は", "h a");//,1,64,0,0");
	EXPECT_TRUE(a.equals(b));
	Lyric c = Lyric("あ", "a");//,1.0,0,0");
	EXPECT_TRUE(false == a.equals(c));
}

TEST(LyricTest, testEqualsForSynth)
{
	Lyric a = Lyric("は", "h a");
	a.lengthRatio = 1;
	a.consonantAdjustmentList({64, 0});
	a.isProtected = false;
	Lyric b = Lyric("あ", "h a");
	b.lengthRatio = 0.5;
	b.consonantAdjustmentList({64, 0});
	b.isProtected = true;
	EXPECT_TRUE(a.equalsForSynth(b));

	// consonantAdjustmentだけ違う場合
	Lyric c = Lyric("は", "h a");
	c.lengthRatio = 1;
	c.consonantAdjustmentList({64, 1});
	c.isProtected = false;
	EXPECT_TRUE(false == a.equalsForSynth(c));

	// 発音記号が違う場合
	Lyric d = Lyric("は", "h e");
	d.lengthRatio = 1;
	d.consonantAdjustmentList({64, 0});
	d.isProtected = false;
	EXPECT_TRUE(false == a.equalsForSynth(d));
}

TEST(LyricTest, testClone)
{
	Lyric lyric("は", "h a");
	lyric.lengthRatio = 1;
	lyric.consonantAdjustmentList({64, 0});
	lyric.isProtected = false;
	Lyric copy = lyric.clone();
	EXPECT_EQ(lyric.toString(), copy.toString());
}

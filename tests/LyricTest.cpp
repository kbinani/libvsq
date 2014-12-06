#include "../include/libvsq/Lyric.hpp"
#include "Util.hpp"

using namespace std;
using namespace vsq;

class LyricTest : public CppUnit::TestCase
{
public:
	void testConstructWithPhrase()
	{
		Lyric lyric = Lyric("は", "h a");
		CPPUNIT_ASSERT_EQUAL(string("は"), lyric.phrase);
		CPPUNIT_ASSERT_EQUAL(string("h a"), lyric.phoneticSymbol());
		CPPUNIT_ASSERT_EQUAL(string("64,0"), lyric.consonantAdjustment());
		CPPUNIT_ASSERT(false == lyric.isProtected);
	}

	void testGetPhoneticSymbol()
	{
		Lyric lyric = Lyric("は", "h a");
		string actual = lyric.phoneticSymbol();
		CPPUNIT_ASSERT_EQUAL(string("h a"), actual);
	}

	void testGetPhoneticSymbolList()
	{
		Lyric lyric = Lyric("は", "h a");
		vector<string> actual = lyric.phoneticSymbolList();
		CPPUNIT_ASSERT_EQUAL((vector<string>::size_type)2, actual.size());
		CPPUNIT_ASSERT_EQUAL(string("h"), actual[0]);
		CPPUNIT_ASSERT_EQUAL(string("a"), actual[1]);
	}

	void testSetPhoneticSymbol()
	{
		Lyric lyric = Lyric("あ", "a");
		lyric.phoneticSymbol("h a");
		vector<string> actual = lyric.phoneticSymbolList();
		CPPUNIT_ASSERT_EQUAL(2, (int)actual.size());
		CPPUNIT_ASSERT_EQUAL(string("h"), actual[0]);
		CPPUNIT_ASSERT_EQUAL(string("a"), actual[1]);
	}

	void testGetConsonantAdjustmentList()
	{
		Lyric lyric = Lyric("は", "h a");
		lyric.lengthRatio = 1;
		lyric.consonantAdjustmentList({64, 0});
		lyric.isProtected = false;
		const vector<int> actual = lyric.consonantAdjustmentList();
		CPPUNIT_ASSERT_EQUAL(2, (int)actual.size());
		CPPUNIT_ASSERT_EQUAL(64, actual[0]);
		CPPUNIT_ASSERT_EQUAL(0, actual[1]);
	}

	void testGetConsonantAdjustmentListWithNil()
	{
		Lyric lyric = Lyric("は", "h a");
		lyric.lengthRatio = 1;
		lyric.consonantAdjustmentList({32, 16});
		lyric.isProtected = false;
		vector<int> actual = lyric.consonantAdjustmentList();
		CPPUNIT_ASSERT_EQUAL(2, (int)actual.size());
		CPPUNIT_ASSERT_EQUAL(32, actual[0]);
		CPPUNIT_ASSERT_EQUAL(16, actual[1]);

		lyric.consonantAdjustmentList(std::vector<int>());
		actual = lyric.consonantAdjustmentList();
		CPPUNIT_ASSERT_EQUAL(2, (int)actual.size());
		CPPUNIT_ASSERT_EQUAL(64, actual[0]);
		CPPUNIT_ASSERT_EQUAL(0, actual[1]);
	}

	void testGetConsonantAdjustment()
	{
		Lyric lyric = Lyric("は", "h a");
		lyric.lengthRatio = 1;
		lyric.consonantAdjustmentList({64, 0});
		lyric.isProtected = false;
		string actual = lyric.consonantAdjustment();
		CPPUNIT_ASSERT_EQUAL(string("64,0"), actual);
	}

	void testSetConsonantAdjustment()
	{
		Lyric lyric = Lyric("は", "h a");
		lyric.consonantAdjustment("61,0");
		CPPUNIT_ASSERT_EQUAL(string("61,0"), lyric.consonantAdjustment());
	}

	void testToString()
	{
		string expected = "は,h a,1,64,0,0";
		Lyric lyric = Lyric("は", "h a");
		lyric.lengthRatio = 1.0;
		lyric.consonantAdjustment("64,0");
		lyric.isProtected = false;
		CPPUNIT_ASSERT_EQUAL(expected, lyric.toString());
	}

	void testEquals()
	{
		Lyric a = Lyric("は", "h a");//,1,64,0,0");
		Lyric b = Lyric("は", "h a");//,1,64,0,0");
		CPPUNIT_ASSERT(a.equals(b));
		Lyric c = Lyric("あ", "a");//,1.0,0,0");
		CPPUNIT_ASSERT(false == a.equals(c));
	}

	void testEqualsForSynth()
	{
		Lyric a = Lyric("は", "h a");
		a.lengthRatio = 1;
		a.consonantAdjustmentList({64, 0});
		a.isProtected = false;
		Lyric b = Lyric("あ", "h a");
		b.lengthRatio = 0.5;
		b.consonantAdjustmentList({64, 0});
		b.isProtected = true;
		CPPUNIT_ASSERT(a.equalsForSynth(b));

		// consonantAdjustmentだけ違う場合
		Lyric c = Lyric("は", "h a");
		c.lengthRatio = 1;
		c.consonantAdjustmentList({64, 1});
		c.isProtected = false;
		CPPUNIT_ASSERT(false == a.equalsForSynth(c));

		// 発音記号が違う場合
		Lyric d = Lyric("は", "h e");
		d.lengthRatio = 1;
		d.consonantAdjustmentList({64, 0});
		d.isProtected = false;
		CPPUNIT_ASSERT(false == a.equalsForSynth(d));
	}

	void testClone()
	{
		Lyric lyric("は", "h a");
		lyric.lengthRatio = 1;
		lyric.consonantAdjustmentList({64, 0});
		lyric.isProtected = false;
		Lyric copy = lyric.clone();
		CPPUNIT_ASSERT_EQUAL(lyric.toString(), copy.toString());
	}

	CPPUNIT_TEST_SUITE(LyricTest);
	CPPUNIT_TEST(testConstructWithPhrase);
	CPPUNIT_TEST(testGetPhoneticSymbol);
	CPPUNIT_TEST(testGetPhoneticSymbolList);
	CPPUNIT_TEST(testSetPhoneticSymbol);
	CPPUNIT_TEST(testGetConsonantAdjustmentList);
	CPPUNIT_TEST(testGetConsonantAdjustmentListWithNil);
	CPPUNIT_TEST(testGetConsonantAdjustment);
	CPPUNIT_TEST(testSetConsonantAdjustment);
	CPPUNIT_TEST(testToString);
	CPPUNIT_TEST(testEquals);
	CPPUNIT_TEST(testEqualsForSynth);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(LyricTest);

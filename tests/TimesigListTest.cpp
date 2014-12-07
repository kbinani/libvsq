#include "Util.hpp"
#include "../include/libvsq/TimesigList.hpp"

using namespace vsq;

TEST(TimesigListTest, testUpdateTimesigInfo)
{
	TimesigList table;
	table.push(Timesig(4, 4, 2));
	table.push(Timesig(4, 4, 0));
	table.push(Timesig(3, 4, 1));

	EXPECT_EQ((tick_t)0, table.get(0).tick());
	EXPECT_EQ(0, table.get(0).barCount);
	EXPECT_EQ((tick_t)1920, table.get(1).tick());
	EXPECT_EQ(1, table.get(1).barCount);
	EXPECT_EQ((tick_t)3360, table.get(2).tick());
	EXPECT_EQ(2, table.get(2).barCount);
}

TEST(TimesigListTest, testGetTimesigAt)
{
	TimesigList table;
	table.push(Timesig(4, 8, 2));
	table.push(Timesig(4, 4, 0));
	table.push(Timesig(3, 4, 1));

	/*
		0                   1               2            3           4           5           6           7           8           9
		‖4   |    |    |    ‖3    |    |    ‖4  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖
		‖4   |    |    |    ‖4    |    |    ‖8  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖  |  |  |  ‖
		^                   ^               ^            ^           ^           ^           ^           ^           ^           ^
		|                   |               |            |           |           |           |           |           |           |
		0                   1920            3360         4320        5280        6240        7200        8160        9120        10080
		*/

	Timesig timesig = table.timesigAt(480);
	EXPECT_EQ(4, timesig.numerator);
	EXPECT_EQ(4, timesig.denominator);
	EXPECT_EQ(0, timesig.barCount);

	timesig = table.timesigAt(1920);
	EXPECT_EQ(3, timesig.numerator);
	EXPECT_EQ(4, timesig.denominator);
	EXPECT_EQ(1, timesig.barCount);

	timesig = table.timesigAt(10000);
	EXPECT_EQ(4, timesig.numerator);
	EXPECT_EQ(8, timesig.denominator);
	EXPECT_EQ(8, timesig.barCount);
}

TEST(TimesigListTest, testPushDuplicateKey)
{
	TimesigList table;
	table.push(Timesig(3, 4, 0));
	table.push(Timesig(4, 8, 0));

	EXPECT_EQ(1, table.size());
	EXPECT_EQ(4, table.get(0).numerator);
	EXPECT_EQ(8, table.get(0).denominator);
}

TEST(TimesigListTest, testTickFromBarCount)
{
	TimesigList table;
	table.push(Timesig(4, 6, 2));     // 3360 tick開始
	table.push(Timesig(4, 4, 0));     //    0 tick開始
	table.push(Timesig(3, 4, 1));     // 1920 tick開始

	EXPECT_EQ((tick_t)0, table.tickFromBarCount(0));
	EXPECT_EQ((tick_t)1920, table.tickFromBarCount(1));
	EXPECT_EQ((tick_t)3360, table.tickFromBarCount(2));
	EXPECT_EQ((tick_t)9760, table.tickFromBarCount(7));
}

TEST(TimesigListTest, testClear)
{
	TimesigList table;
	table.push(Timesig(4, 6, 2));
	EXPECT_EQ(1, table.size());
	table.clear();
	EXPECT_EQ(0, table.size());
	table.push(Timesig(4, 6, 2));
	EXPECT_EQ(1, table.size());
}

TEST(TimesigListTest, testCopy)
{
	TimesigList a;
	a.push(Timesig(4, 4, 0));
	TimesigList b = a;
	a.push(Timesig(3, 4, 1));

	EXPECT_EQ(2, a.size());
	EXPECT_EQ(1, b.size());
}

TEST(TimesigListTest, testBarCountFromTick)
{
	TimesigList a;
	a.push(Timesig(4, 6, 2));     // 3360 tick開始
	a.push(Timesig(4, 4, 0));     //    0 tick開始
	a.push(Timesig(3, 4, 1));     // 1920 tick開始

	EXPECT_EQ(0, a.barCountFromTick(0));
	EXPECT_EQ(0, a.barCountFromTick(1919));
	EXPECT_EQ(1, a.barCountFromTick(1920));
	EXPECT_EQ(2, a.barCountFromTick(3360));
	EXPECT_EQ(7, a.barCountFromTick(9760));
}

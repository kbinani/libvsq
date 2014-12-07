#include "Util.hpp"
#include "../include/libvsq/Tempo.hpp"

using namespace std;
using namespace vsq;

class TempoStub : public Tempo
{
public:
	TempoStub(tick_t tick, int tempo) :
		Tempo(tick, tempo)
	{
	}

	void setTime(double time)
	{
		_time = time;
	}
};

TEST(TempoTest, testtestConstructor)
{
	Tempo entry = Tempo();
	EXPECT_EQ((tick_t)0, entry.tick);
	EXPECT_EQ(0, entry.tempo);
	EXPECT_EQ(0.0, entry.time());

	entry = Tempo(480, 500000);
	EXPECT_EQ((tick_t)480, entry.tick);
	EXPECT_EQ(500000, entry.tempo);
}

TEST(TempoTest, testtestToString)
{
	Tempo entry = Tempo(480, 500000);
	EXPECT_EQ(string("{Tick=480, Tempo=500000, Time=0}"), entry.toString());
}

TEST(TempoTest, testtestCompareTo)
{
	Tempo a = Tempo();
	Tempo b = Tempo(480, 500000);
	EXPECT_TRUE(0 < b.compareTo(a));
	EXPECT_EQ(0, a.compareTo(a));
	EXPECT_TRUE(0 > a.compareTo(b));
}

TEST(TempoTest, testtestEquals)
{
	TempoStub a = TempoStub(480, 500000);
	TempoStub b = TempoStub(480, 500000);
	a.setTime(0.5);
	b.setTime(0.5);
	EXPECT_TRUE(a.equals(b));
	// クロックは同じだがtimeが違う
	b.setTime(1);
	EXPECT_TRUE(a.equals(b));
	b.tick = 1;
	b.setTime(0.5);
	EXPECT_TRUE(false == a.equals(b));
}

TEST(TempoTest, testtestCompare)
{
	Tempo a = Tempo();
	Tempo b = Tempo(480, 500000);
	EXPECT_TRUE(false == Tempo::compare(b, a));
	EXPECT_TRUE(false == Tempo::compare(a, a));
	EXPECT_TRUE(Tempo::compare(a, b));
}

TEST(TempoTest, testtestClone)
{
	Tempo a(1920, 500000);
	Tempo b = a;
	EXPECT_EQ((tick_t)1920, b.tick);
	EXPECT_EQ(500000, b.tempo);
	EXPECT_EQ(0.0, b.time());
}

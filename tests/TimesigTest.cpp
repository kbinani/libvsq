#include "Util.hpp"
#include "../include/libvsq/Timesig.hpp"

using namespace std;
using namespace vsq;

class TimesigStub : public Timesig
{
public:
	explicit TimesigStub(const Timesig& value) :
		Timesig(value)
	{
	}

	void setTick(tick_t tick)
	{
		this->tick_ = tick;
	}
};

TEST(TimesigTest, testConstruct)
{
	Timesig itemA;
	EXPECT_EQ(itemA.numerator, 4);
	EXPECT_EQ(itemA.denominator, 4);
	EXPECT_EQ(itemA.barCount, 0);

	Timesig itemB(3, 4, 1);
	EXPECT_EQ(itemB.numerator, 3);
	EXPECT_EQ(itemB.denominator, 4);
	EXPECT_EQ(itemB.barCount, 1);
}

TEST(TimesigTest, testToString)
{
	Timesig item(3, 4, 1);
	string expected = "{Tick=0, Numerator=3, Denominator=4, BarCount=1}";
	EXPECT_EQ(expected, item.toString());
}

TEST(TimesigTest, testCompareTo)
{
	Timesig a(4, 4, 0);
	Timesig b(4, 4, 1);
	EXPECT_EQ(true, a.compareTo(b));
	EXPECT_EQ(false, b.compareTo(a));
	EXPECT_EQ(false, a.compareTo(a));
}

TEST(TimesigTest, testClone)
{
	TimesigStub a(Timesig(3, 4, 1));
	a.setTick(10);
	Timesig b = a.clone();
	EXPECT_EQ(3, b.numerator);
	EXPECT_EQ(4, b.denominator);
	EXPECT_EQ(1, b.barCount);
	EXPECT_EQ((tick_t)10, b.tick());
}

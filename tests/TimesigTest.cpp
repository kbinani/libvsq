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

class TimesigTest : public CppUnit::TestFixture
{
public:
	void testConstruct()
	{
		Timesig itemA;
		CPPUNIT_ASSERT_EQUAL(itemA.numerator, 4);
		CPPUNIT_ASSERT_EQUAL(itemA.denominator, 4);
		CPPUNIT_ASSERT_EQUAL(itemA.barCount, 0);

		Timesig itemB(3, 4, 1);
		CPPUNIT_ASSERT_EQUAL(itemB.numerator, 3);
		CPPUNIT_ASSERT_EQUAL(itemB.denominator, 4);
		CPPUNIT_ASSERT_EQUAL(itemB.barCount, 1);
	}

	void testToString()
	{
		Timesig item(3, 4, 1);
		string expected = "{Tick=0, Numerator=3, Denominator=4, BarCount=1}";
		CPPUNIT_ASSERT_EQUAL(expected, item.toString());
	}

	void testCompareTo()
	{
		Timesig a(4, 4, 0);
		Timesig b(4, 4, 1);
		CPPUNIT_ASSERT_EQUAL(true, a.compareTo(b));
		CPPUNIT_ASSERT_EQUAL(false, b.compareTo(a));
		CPPUNIT_ASSERT_EQUAL(false, a.compareTo(a));
	}

	void testClone()
	{
		TimesigStub a(Timesig(3, 4, 1));
		a.setTick(10);
		Timesig b = a.clone();
		CPPUNIT_ASSERT_EQUAL(3, b.numerator);
		CPPUNIT_ASSERT_EQUAL(4, b.denominator);
		CPPUNIT_ASSERT_EQUAL(1, b.barCount);
		CPPUNIT_ASSERT_EQUAL((tick_t)10, b.tick());
	}

	CPPUNIT_TEST_SUITE(TimesigTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testToString);
	CPPUNIT_TEST(testCompareTo);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(TimesigTest);

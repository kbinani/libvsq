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

class TempoTest : public CppUnit::TestCase
{
public:
	void testConstructor()
	{
		Tempo entry = Tempo();
		CPPUNIT_ASSERT_EQUAL((tick_t)0, entry.tick);
		CPPUNIT_ASSERT_EQUAL(0, entry.tempo);
		CPPUNIT_ASSERT_EQUAL(0.0, entry.time());

		entry = Tempo(480, 500000);
		CPPUNIT_ASSERT_EQUAL((tick_t)480, entry.tick);
		CPPUNIT_ASSERT_EQUAL(500000, entry.tempo);
	}

	void testToString()
	{
		Tempo entry = Tempo(480, 500000);
		CPPUNIT_ASSERT_EQUAL(string("{Tick=480, Tempo=500000, Time=0}"), entry.toString());
	}

	void testCompareTo()
	{
		Tempo a = Tempo();
		Tempo b = Tempo(480, 500000);
		CPPUNIT_ASSERT(0 < b.compareTo(a));
		CPPUNIT_ASSERT_EQUAL(0, a.compareTo(a));
		CPPUNIT_ASSERT(0 > a.compareTo(b));
	}

	void testEquals()
	{
		TempoStub a = TempoStub(480, 500000);
		TempoStub b = TempoStub(480, 500000);
		a.setTime(0.5);
		b.setTime(0.5);
		CPPUNIT_ASSERT(a.equals(b));
		// クロックは同じだがtimeが違う
		b.setTime(1);
		CPPUNIT_ASSERT(a.equals(b));
		b.tick = 1;
		b.setTime(0.5);
		CPPUNIT_ASSERT(false == a.equals(b));
	}

	void testCompare()
	{
		Tempo a = Tempo();
		Tempo b = Tempo(480, 500000);
		CPPUNIT_ASSERT(false == Tempo::compare(b, a));
		CPPUNIT_ASSERT(false == Tempo::compare(a, a));
		CPPUNIT_ASSERT(Tempo::compare(a, b));
	}

	void testClone()
	{
		Tempo a(1920, 500000);
		Tempo b = a;
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, b.tick);
		CPPUNIT_ASSERT_EQUAL(500000, b.tempo);
		CPPUNIT_ASSERT_EQUAL(0.0, b.time());
	}

	CPPUNIT_TEST_SUITE(TempoTest);
	CPPUNIT_TEST(testConstructor);
	CPPUNIT_TEST(testToString);
	CPPUNIT_TEST(testCompareTo);
	CPPUNIT_TEST(testEquals);
	CPPUNIT_TEST(testCompare);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(TempoTest);

#include "Util.hpp"
#include "../include/libvsq/TempoList.hpp"

using namespace std;
using namespace vsq;

class TempoListTest : public CppUnit::TestCase
{
public:
	/**
	 * push, get, set, size のテスト
	 */
	void test()
	{
		TempoList list;
		CPPUNIT_ASSERT_EQUAL(0, list.size());
		list.push(Tempo(0, 500000));
		list.push(Tempo(480, 525000));
		list.updateTempoInfo();

		CPPUNIT_ASSERT_EQUAL(2, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)0, list.get(0).tick);
		CPPUNIT_ASSERT_EQUAL(500000, list.get(0).tempo);
		CPPUNIT_ASSERT_EQUAL(0.0, list.get(0).time());
		CPPUNIT_ASSERT_EQUAL((tick_t)480, list.get(1).tick);
		CPPUNIT_ASSERT_EQUAL(525000, list.get(1).tempo);
		CPPUNIT_ASSERT_EQUAL(0.5, list.get(1).time());
	}

	void testIterator()
	{
		TempoList list;
		CPPUNIT_ASSERT_EQUAL(0, list.size());
		list.push(Tempo(0, 500000));
		list.push(Tempo(480, 525000));
		list.updateTempoInfo();

		TempoList::Iterator i = list.iterator();
		CPPUNIT_ASSERT(i.hasNext());
		Tempo item = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)0, item.tick);
		CPPUNIT_ASSERT_EQUAL(500000, item.tempo);
		CPPUNIT_ASSERT_EQUAL(0.0, item.time());
		CPPUNIT_ASSERT(i.hasNext());
		item = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)480, item.tick);
		CPPUNIT_ASSERT_EQUAL(525000, item.tempo);
		CPPUNIT_ASSERT_EQUAL(0.5, item.time());
		CPPUNIT_ASSERT(i.hasNext() == false);
	}

	void testSort()
	{
		TempoList list;
		list.push(Tempo(480, 525000));
		list.push(Tempo(0, 500000));

		list.sort();

		CPPUNIT_ASSERT_EQUAL((tick_t)0, list.get(0).tick);
		CPPUNIT_ASSERT_EQUAL(500000, list.get(0).tempo);
		CPPUNIT_ASSERT_EQUAL((tick_t)480, list.get(1).tick);
		CPPUNIT_ASSERT_EQUAL(525000, list.get(1).tempo);
	}

	void testTickFromSec()
	{
		TempoList list;
		list.push(Tempo(480, 480000));
		list.push(Tempo(0, 500000));
		list.updateTempoInfo();

		CPPUNIT_ASSERT_EQUAL(0.0, list.tickFromTime(0.0));
		CPPUNIT_ASSERT_EQUAL(480.0, list.tickFromTime(0.5));
		CPPUNIT_ASSERT_EQUAL(384.0, list.tickFromTime(0.4));
		CPPUNIT_ASSERT_EQUAL(680.0, list.tickFromTime(0.7));
	}

	void testUpdateTempoInfo()
	{
		TempoList list;
		list.updateTempoInfo();
		CPPUNIT_ASSERT_EQUAL(1, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)0, list.get(0).tick);
		CPPUNIT_ASSERT_EQUAL(500000, list.get(0).tempo);
		CPPUNIT_ASSERT_EQUAL(0.0, list.get(0).time());

		list = TempoList();
		list.push(Tempo(480, 525000));
		list.push(Tempo(0, 500000));
		list.updateTempoInfo();
		CPPUNIT_ASSERT_EQUAL(2, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)0, list.get(0).tick);
		CPPUNIT_ASSERT_EQUAL(500000, list.get(0).tempo);
		CPPUNIT_ASSERT_EQUAL(0.0, list.get(0).time());
		CPPUNIT_ASSERT_EQUAL((tick_t)480, list.get(1).tick);
		CPPUNIT_ASSERT_EQUAL(525000, list.get(1).tempo);
		CPPUNIT_ASSERT_EQUAL(0.5, list.get(1).time());
	}

	void testTimeFromTick()
	{
		TempoList list;
		list.push(Tempo(480, 480000));
		list.push(Tempo(0, 500000));
		list.updateTempoInfo();

		CPPUNIT_ASSERT_EQUAL(0.0, list.timeFromTick(0));
		CPPUNIT_ASSERT_EQUAL(0.5, list.timeFromTick(480));
		CPPUNIT_ASSERT_EQUAL(0.4, list.timeFromTick(384));
		CPPUNIT_ASSERT_EQUAL(0.7, list.timeFromTick(680));
	}

	void testGetTempoAt()
	{
		TempoList list;
		list.push(Tempo(480, 480000));
		list.push(Tempo(0, 500000));
		list.updateTempoInfo();

		CPPUNIT_ASSERT_EQUAL(500000, list.tempoAt(0));
		CPPUNIT_ASSERT_EQUAL(500000, list.tempoAt(479));
		CPPUNIT_ASSERT_EQUAL(480000, list.tempoAt(480));
	}

	void testClear()
	{
		TempoList list;
		list.push(Tempo(480, 480000));
		CPPUNIT_ASSERT_EQUAL(1, list.size());
		list.clear();
		CPPUNIT_ASSERT_EQUAL(0, list.size());
	}

	CPPUNIT_TEST_SUITE(TempoListTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testIterator);
	CPPUNIT_TEST(testSort);
	CPPUNIT_TEST(testTickFromSec);
	CPPUNIT_TEST(testUpdateTempoInfo);
	CPPUNIT_TEST(testTimeFromTick);
	CPPUNIT_TEST(testGetTempoAt);
	CPPUNIT_TEST(testClear);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(TempoListTest);

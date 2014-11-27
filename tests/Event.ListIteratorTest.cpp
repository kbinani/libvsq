#include "Util.hpp"
#include "../Event.hpp"

using namespace std;
using namespace vsq;

class EventListIteratorTest : public CppUnit::TestCase
{
public:
	void test()
	{
		Event::List list;
		Event::ListIterator iterator(&list);
		CPPUNIT_ASSERT(false == iterator.hasNext());

		Event a(1920, EventType::NOTE);
		Event b(480, EventType::ICON);
		list.add(a, 1);
		list.add(b, 2);

		iterator = Event::ListIterator(&list);
		CPPUNIT_ASSERT(iterator.hasNext());
		Event* eventA = iterator.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)480, eventA->clock);
		CPPUNIT_ASSERT_EQUAL(2, eventA->id);
		CPPUNIT_ASSERT_EQUAL(EventType::ICON, eventA->type);
		CPPUNIT_ASSERT(iterator.hasNext());
		iterator.remove();
		CPPUNIT_ASSERT(iterator.hasNext());
		Event* eventB = iterator.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, eventB->clock);
		CPPUNIT_ASSERT_EQUAL(1, eventB->id);
		CPPUNIT_ASSERT_EQUAL(EventType::NOTE, eventB->type);
		CPPUNIT_ASSERT(false == iterator.hasNext());

		CPPUNIT_ASSERT_EQUAL(1, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list.get(0)->clock);
		CPPUNIT_ASSERT_EQUAL(1, list.get(0)->id);
	}

	CPPUNIT_TEST_SUITE(EventListIteratorTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(EventListIteratorTest);

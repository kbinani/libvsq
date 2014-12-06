#include "Util.hpp"
#include "../include/libvsq/EventListIndexIterator.hpp"
#include "../include/libvsq/EventListIndexIteratorKind.hpp"

using namespace vsq;

class EventListIndexIteratorTest : public CppUnit::TestCase
{
public:
	void test()
	{
		Event::List list;

		Event noteEvent(1920, EventType::NOTE);
		list.add(noteEvent, 5);

		Event singerEvent(0, EventType::SINGER);
		list.add(singerEvent, 1);

		Event dynaffEvent(480, EventType::ICON);
		dynaffEvent.iconDynamicsHandle = Handle(HandleType::DYNAMICS);
		dynaffEvent.iconDynamicsHandle.iconId = "$05010001";
		list.add(dynaffEvent, 3);

		Event crescendoEvent(240, EventType::ICON);
		crescendoEvent.iconDynamicsHandle = Handle(HandleType::DYNAMICS);
		crescendoEvent.iconDynamicsHandle.iconId = "$05020001";
		list.add(crescendoEvent, 2);

		Event decrescendoEvent(720, EventType::ICON);
		decrescendoEvent.iconDynamicsHandle = Handle(HandleType::DYNAMICS);
		decrescendoEvent.iconDynamicsHandle.iconId = "$05030001";
		list.add(decrescendoEvent, 4);

		// 音符イベントのみのイテレータ
		EventListIndexIterator noteIterator(&list, EventListIndexIteratorKind::NOTE);
		CPPUNIT_ASSERT(noteIterator.hasNext());
		CPPUNIT_ASSERT_EQUAL(4, noteIterator.next());
		CPPUNIT_ASSERT_EQUAL(false, noteIterator.hasNext());
		const Event* event = list.get(4);
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, event->tick);
		CPPUNIT_ASSERT_EQUAL(5, event->id);

		// 歌手変更イベントのみのイテレータ
		EventListIndexIterator singerIterator(&list, EventListIndexIteratorKind::SINGER);
		CPPUNIT_ASSERT(singerIterator.hasNext());
		CPPUNIT_ASSERT_EQUAL(0, singerIterator.next());
		CPPUNIT_ASSERT_EQUAL(false, singerIterator.hasNext());
		event = list.get(0);
		CPPUNIT_ASSERT_EQUAL((tick_t)0, event->tick);
		CPPUNIT_ASSERT_EQUAL(1, event->id);

		// 強弱記号のみのイテレータ
		EventListIndexIterator dynaffIterator(&list, EventListIndexIteratorKind::DYNAFF);
		CPPUNIT_ASSERT(dynaffIterator.hasNext());
		CPPUNIT_ASSERT_EQUAL(2, dynaffIterator.next());
		CPPUNIT_ASSERT_EQUAL(false, dynaffIterator.hasNext());
		event = list.get(2);
		CPPUNIT_ASSERT_EQUAL((tick_t)480, event->tick);
		CPPUNIT_ASSERT_EQUAL(3, event->id);

		// クレッシェンドのみのイテレータ
		EventListIndexIterator crescendoIterator(&list, EventListIndexIteratorKind::CRESCENDO);
		CPPUNIT_ASSERT(crescendoIterator.hasNext());
		CPPUNIT_ASSERT_EQUAL(1, crescendoIterator.next());
		CPPUNIT_ASSERT_EQUAL(false, crescendoIterator.hasNext());
		event = list.get(1);
		CPPUNIT_ASSERT_EQUAL((tick_t)240, event->tick);
		CPPUNIT_ASSERT_EQUAL(2, event->id);

		EventListIndexIterator decrescendoIterator(&list, EventListIndexIteratorKind::DECRESCENDO);
		CPPUNIT_ASSERT(decrescendoIterator.hasNext());
		CPPUNIT_ASSERT_EQUAL(3, decrescendoIterator.next());
		CPPUNIT_ASSERT_EQUAL(false, decrescendoIterator.hasNext());
		event = list.get(3);
		CPPUNIT_ASSERT_EQUAL((tick_t)720, event->tick);
		CPPUNIT_ASSERT_EQUAL(4, event->id);

		EventListIndexIteratorKind kindAll = EventListIndexIteratorKind::ALL;
		EventListIndexIterator iteratorAll(&list, kindAll);
		CPPUNIT_ASSERT(iteratorAll.hasNext());
		CPPUNIT_ASSERT_EQUAL(0, iteratorAll.next());
		CPPUNIT_ASSERT_EQUAL((tick_t)0, list.get(0)->tick);
		CPPUNIT_ASSERT_EQUAL(1, list.get(0)->id);

		CPPUNIT_ASSERT(iteratorAll.hasNext());
		CPPUNIT_ASSERT_EQUAL(1, iteratorAll.next());
		CPPUNIT_ASSERT_EQUAL((tick_t)240, list.get(1)->tick);
		CPPUNIT_ASSERT_EQUAL(2, list.get(1)->id);

		CPPUNIT_ASSERT(iteratorAll.hasNext());
		CPPUNIT_ASSERT_EQUAL(2, iteratorAll.next());
		CPPUNIT_ASSERT_EQUAL((tick_t)480, list.get(2)->tick);
		CPPUNIT_ASSERT_EQUAL(3, list.get(2)->id);

		CPPUNIT_ASSERT(iteratorAll.hasNext());
		CPPUNIT_ASSERT_EQUAL(3, iteratorAll.next());
		CPPUNIT_ASSERT_EQUAL((tick_t)720, list.get(3)->tick);
		CPPUNIT_ASSERT_EQUAL(4, list.get(3)->id);

		CPPUNIT_ASSERT(iteratorAll.hasNext());
		CPPUNIT_ASSERT_EQUAL(4, iteratorAll.next());
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list.get(4)->tick);
		CPPUNIT_ASSERT_EQUAL(5, list.get(4)->id);

		CPPUNIT_ASSERT_EQUAL(false, iteratorAll.hasNext());
	}

	CPPUNIT_TEST_SUITE(EventListIndexIteratorTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(EventListIndexIteratorTest);

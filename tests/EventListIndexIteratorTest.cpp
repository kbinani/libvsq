#include "Util.hpp"
#include "../include/libvsq/EventListIndexIterator.hpp"
#include "../include/libvsq/EventListIndexIteratorKind.hpp"

using namespace vsq;

TEST(EventListIndexIteratorTest, test)
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
	EXPECT_TRUE(noteIterator.hasNext());
	EXPECT_EQ(4, noteIterator.next());
	EXPECT_EQ(false, noteIterator.hasNext());
	const Event* event = list.get(4);
	EXPECT_EQ((tick_t)1920, event->tick);
	EXPECT_EQ(5, event->id);

	// 歌手変更イベントのみのイテレータ
	EventListIndexIterator singerIterator(&list, EventListIndexIteratorKind::SINGER);
	EXPECT_TRUE(singerIterator.hasNext());
	EXPECT_EQ(0, singerIterator.next());
	EXPECT_EQ(false, singerIterator.hasNext());
	event = list.get(0);
	EXPECT_EQ((tick_t)0, event->tick);
	EXPECT_EQ(1, event->id);

	// 強弱記号のみのイテレータ
	EventListIndexIterator dynaffIterator(&list, EventListIndexIteratorKind::DYNAFF);
	EXPECT_TRUE(dynaffIterator.hasNext());
	EXPECT_EQ(2, dynaffIterator.next());
	EXPECT_EQ(false, dynaffIterator.hasNext());
	event = list.get(2);
	EXPECT_EQ((tick_t)480, event->tick);
	EXPECT_EQ(3, event->id);

	// クレッシェンドのみのイテレータ
	EventListIndexIterator crescendoIterator(&list, EventListIndexIteratorKind::CRESCENDO);
	EXPECT_TRUE(crescendoIterator.hasNext());
	EXPECT_EQ(1, crescendoIterator.next());
	EXPECT_EQ(false, crescendoIterator.hasNext());
	event = list.get(1);
	EXPECT_EQ((tick_t)240, event->tick);
	EXPECT_EQ(2, event->id);

	EventListIndexIterator decrescendoIterator(&list, EventListIndexIteratorKind::DECRESCENDO);
	EXPECT_TRUE(decrescendoIterator.hasNext());
	EXPECT_EQ(3, decrescendoIterator.next());
	EXPECT_EQ(false, decrescendoIterator.hasNext());
	event = list.get(3);
	EXPECT_EQ((tick_t)720, event->tick);
	EXPECT_EQ(4, event->id);

	EventListIndexIteratorKind kindAll = EventListIndexIteratorKind::ALL;
	EventListIndexIterator iteratorAll(&list, kindAll);
	EXPECT_TRUE(iteratorAll.hasNext());
	EXPECT_EQ(0, iteratorAll.next());
	EXPECT_EQ((tick_t)0, list.get(0)->tick);
	EXPECT_EQ(1, list.get(0)->id);

	EXPECT_TRUE(iteratorAll.hasNext());
	EXPECT_EQ(1, iteratorAll.next());
	EXPECT_EQ((tick_t)240, list.get(1)->tick);
	EXPECT_EQ(2, list.get(1)->id);

	EXPECT_TRUE(iteratorAll.hasNext());
	EXPECT_EQ(2, iteratorAll.next());
	EXPECT_EQ((tick_t)480, list.get(2)->tick);
	EXPECT_EQ(3, list.get(2)->id);

	EXPECT_TRUE(iteratorAll.hasNext());
	EXPECT_EQ(3, iteratorAll.next());
	EXPECT_EQ((tick_t)720, list.get(3)->tick);
	EXPECT_EQ(4, list.get(3)->id);

	EXPECT_TRUE(iteratorAll.hasNext());
	EXPECT_EQ(4, iteratorAll.next());
	EXPECT_EQ((tick_t)1920, list.get(4)->tick);
	EXPECT_EQ(5, list.get(4)->id);

	EXPECT_EQ(false, iteratorAll.hasNext());
}

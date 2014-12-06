#include "Util.hpp"
#include "../include/libvsq/Event.hpp"

using namespace std;
using namespace vsq;

class EventListStub : public Event::List
{
public:
	void pushBackWithoutSort(Event const& item, int id)
	{
		auto add = std::unique_ptr<Event>(new Event(item));
		*add = item;
		_events.push_back(std::move(add));
		_ids.push_back(id);
	}
};

TEST(EventListTest, testConstruct)
{
	Event::List list;
	EXPECT_EQ(0, list.size());
}

TEST(EventListTest, testFindIndexFromId)
{
	Event::List list;
	Event a(1, EventType::NOTE);
	Event b(0, EventType::NOTE);
	list.add(a, 0);
	list.add(b, 1);

	// aのほうがtickが大きいので, 後ろに並び替えられる
	EXPECT_EQ(0, list.findIndexFromId(1));
	EXPECT_EQ(1, list.findIndexFromId(0));
}

TEST(EventListTest, testFindFromId)
{
	Event::List list;
	Event a(0, EventType::NOTE);
	Event b(0, EventType::SINGER);
	list.add(a, 0);
	list.add(b, 1);

	EXPECT_EQ(EventType::NOTE, list.findFromId(0)->type());
	EXPECT_EQ(EventType::SINGER, list.findFromId(1)->type());
	EXPECT_TRUE(NULL == list.findFromId(1000));
}

TEST(EventListTest, testSetForId)
{
	Event::List listA;
	Event event(0, EventType::NOTE);
	event.note = 60;
	event.id = 10;

	Event replace(0, EventType::NOTE);
	replace.note = 90;
	replace.id = 100;

	listA.add(event, 10);
	listA.setForId(10, replace);

	EXPECT_EQ(10, listA.get(0)->id);
	EXPECT_EQ(90, listA.get(0)->note);

	// 無効なinternalIdを渡すので, setが行われない場合
	Event::List listB;
	listB.add(event, 10);
	listB.setForId(9999, replace);
	EXPECT_EQ(60, listB.get(0)->note);
}

TEST(EventListTest, testSort)
{
	EventListStub list;
	Event b(480, EventType::NOTE);
	Event a(0, EventType::NOTE);
	b.id = 14;
	a.id = 20;
	list.pushBackWithoutSort(b, 14);
	list.pushBackWithoutSort(a, 20);

	list.sort();

	EXPECT_EQ((tick_t)0, list.get(0)->tick);
	EXPECT_EQ((tick_t)480, list.get(1)->tick);
	EXPECT_EQ(0, list.findIndexFromId(20));
	EXPECT_EQ(1, list.findIndexFromId(14));
}

TEST(EventListTest, testClear)
{
	Event::List list;
	Event b(480, EventType::NOTE);
	Event a(0, EventType::NOTE);
	list.add(b, 14);
	list.add(a, 20);

	EXPECT_EQ(2, list.size());
	EXPECT_EQ(20, list.get(0)->id);

	list.clear();

	EXPECT_EQ(0, list.size());
}

TEST(EventListTest, testAddWithoutInternalId)
{
	Event::List list;
	Event a(1, EventType::NOTE);
	Event b(0, EventType::NOTE);
	int idOfA = list.add(a);
	int idOfB = list.add(b);

	// bよりaのほうがtickが大きいので, 並べ替えが起きるはず
	EXPECT_EQ(idOfB, list.get(0)->id);
	EXPECT_EQ(idOfA, list.get(1)->id);
	EXPECT_TRUE(idOfA != idOfB);
}

TEST(EventListTest, testAddWithInternalId)
{
	Event::List list;
	Event a(0, EventType::NOTE);
	Event b(0, EventType::NOTE);
	int idOfA = list.add(a, 100);
	int idOfB = list.add(b, 2);
	EXPECT_EQ(100, idOfA);
	EXPECT_EQ(2, idOfB);
	EXPECT_EQ(100, list.get(0)->id);
	EXPECT_EQ(2, list.get(1)->id);
}

TEST(EventListTest, testRemoveAt)
{
	Event::List list;
	Event a(0, EventType::NOTE);
	Event b(0, EventType::NOTE);
	list.add(a, 100);
	list.add(b, 2);
	EXPECT_EQ(100, list.get(0)->id);
	EXPECT_EQ(2, list.get(1)->id);
	EXPECT_EQ(2, list.size());

	list.removeAt(0);

	EXPECT_EQ(1, list.size());
	EXPECT_EQ(2, list.get(0)->id);
}

TEST(EventListTest, testSize)
{
	Event::List list;
	EXPECT_EQ(0, list.size());
	Event event(0, EventType::NOTE);
	list.add(event);
	EXPECT_EQ(1, list.size());
}

TEST(EventListTest, testGetAndSetElement)
{
	Event::List list;
	Event a(0, EventType::NOTE);
	Event b(0, EventType::NOTE);
	list.add(a, 100);
	list.add(b, 2);
	EXPECT_EQ(100, list.get(0)->id);
	EXPECT_EQ(2, list.get(1)->id);

	Event c(480, EventType::NOTE);
	c.id = 99;
	list.set(1, c);

	EXPECT_EQ(100, list.get(0)->id);
	EXPECT_EQ(2, list.get(1)->id);
	EXPECT_EQ((tick_t)480, list.get(1)->tick);
}

TEST(EventListTest, testIterator)
{
	Event::List list;
	Event::ListIterator iterator = list.iterator();
	EXPECT_TRUE(false == iterator.hasNext());

	Event singerEvent(0, EventType::SINGER);
	singerEvent.singerHandle = Handle(HandleType::SINGER);
	list.add(singerEvent, 1);

	Event crescendoEvent(240, EventType::ICON);
	crescendoEvent.iconDynamicsHandle = Handle(HandleType::DYNAMICS);
	crescendoEvent.iconDynamicsHandle.iconId = "$05020001";
	list.add(crescendoEvent, 2);

	iterator = list.iterator();
	EXPECT_TRUE(iterator.hasNext());
	EXPECT_EQ((tick_t)0, iterator.next()->tick);
	EXPECT_TRUE(iterator.hasNext());
	EXPECT_EQ((tick_t)240, iterator.next()->tick);
	EXPECT_TRUE(false == iterator.hasNext());
}

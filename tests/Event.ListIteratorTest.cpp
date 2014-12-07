#include "Util.hpp"
#include "../include/libvsq/Event.hpp"

using namespace std;
using namespace vsq;

TEST(EventListIteratorTest, test)
{
	Event::List list;
	Event::ListIterator iterator(&list);
	EXPECT_TRUE(false == iterator.hasNext());

	Event a(1920, EventType::NOTE);
	Event b(480, EventType::ICON);
	list.add(a, 1);
	list.add(b, 2);

	iterator = Event::ListIterator(&list);
	EXPECT_TRUE(iterator.hasNext());
	Event* eventA = iterator.next();
	EXPECT_EQ((tick_t)480, eventA->tick);
	EXPECT_EQ(2, eventA->id);
	EXPECT_EQ(EventType::ICON, eventA->type());
	EXPECT_TRUE(iterator.hasNext());
	iterator.remove();
	EXPECT_TRUE(iterator.hasNext());
	Event* eventB = iterator.next();
	EXPECT_EQ((tick_t)1920, eventB->tick);
	EXPECT_EQ(1, eventB->id);
	EXPECT_EQ(EventType::NOTE, eventB->type());
	EXPECT_TRUE(false == iterator.hasNext());

	EXPECT_EQ(1, list.size());
	EXPECT_EQ((tick_t)1920, list.get(0)->tick);
	EXPECT_EQ(1, list.get(0)->id);
}

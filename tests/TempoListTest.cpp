#include "Util.hpp"
#include "../include/libvsq/TempoList.hpp"

using namespace std;
using namespace vsq;

/**
 * push, get, set, size のテスト
 */
TEST(TempoListTest, test)
{
	TempoList list;
	EXPECT_EQ(0, list.size());
	list.push(Tempo(0, 500000));
	list.push(Tempo(480, 525000));
	list.updateTempoInfo();

	EXPECT_EQ(2, list.size());
	EXPECT_EQ((tick_t)0, list.get(0).tick);
	EXPECT_EQ(500000, list.get(0).tempo);
	EXPECT_EQ(0.0, list.get(0).time());
	EXPECT_EQ((tick_t)480, list.get(1).tick);
	EXPECT_EQ(525000, list.get(1).tempo);
	EXPECT_EQ(0.5, list.get(1).time());
}

TEST(TempoListTest, testIterator)
{
	TempoList list;
	EXPECT_EQ(0, list.size());
	list.push(Tempo(0, 500000));
	list.push(Tempo(480, 525000));
	list.updateTempoInfo();

	TempoList::Iterator i = list.iterator();
	EXPECT_TRUE(i.hasNext());
	Tempo item = i.next();
	EXPECT_EQ((tick_t)0, item.tick);
	EXPECT_EQ(500000, item.tempo);
	EXPECT_EQ(0.0, item.time());
	EXPECT_TRUE(i.hasNext());
	item = i.next();
	EXPECT_EQ((tick_t)480, item.tick);
	EXPECT_EQ(525000, item.tempo);
	EXPECT_EQ(0.5, item.time());
	EXPECT_TRUE(i.hasNext() == false);
}

TEST(TempoListTest, testSort)
{
	TempoList list;
	list.push(Tempo(480, 525000));
	list.push(Tempo(0, 500000));

	list.sort();

	EXPECT_EQ((tick_t)0, list.get(0).tick);
	EXPECT_EQ(500000, list.get(0).tempo);
	EXPECT_EQ((tick_t)480, list.get(1).tick);
	EXPECT_EQ(525000, list.get(1).tempo);
}

TEST(TempoListTest, testTickFromSec)
{
	TempoList list;
	list.push(Tempo(480, 480000));
	list.push(Tempo(0, 500000));
	list.updateTempoInfo();

	EXPECT_EQ(0.0, list.tickFromTime(0.0));
	EXPECT_EQ(480.0, list.tickFromTime(0.5));
	EXPECT_EQ(384.0, list.tickFromTime(0.4));
	EXPECT_EQ(680.0, list.tickFromTime(0.7));
}

TEST(TempoListTest, testUpdateTempoInfo)
{
	TempoList list;
	list.updateTempoInfo();
	EXPECT_EQ(1, list.size());
	EXPECT_EQ((tick_t)0, list.get(0).tick);
	EXPECT_EQ(500000, list.get(0).tempo);
	EXPECT_EQ(0.0, list.get(0).time());

	list = TempoList();
	list.push(Tempo(480, 525000));
	list.push(Tempo(0, 500000));
	list.updateTempoInfo();
	EXPECT_EQ(2, list.size());
	EXPECT_EQ((tick_t)0, list.get(0).tick);
	EXPECT_EQ(500000, list.get(0).tempo);
	EXPECT_EQ(0.0, list.get(0).time());
	EXPECT_EQ((tick_t)480, list.get(1).tick);
	EXPECT_EQ(525000, list.get(1).tempo);
	EXPECT_EQ(0.5, list.get(1).time());
}

TEST(TempoListTest, testTimeFromTick)
{
	TempoList list;
	list.push(Tempo(480, 480000));
	list.push(Tempo(0, 500000));
	list.updateTempoInfo();

	EXPECT_EQ(0.0, list.timeFromTick(0));
	EXPECT_EQ(0.5, list.timeFromTick(480));
	EXPECT_EQ(0.4, list.timeFromTick(384));
	EXPECT_EQ(0.7, list.timeFromTick(680));
}

TEST(TempoListTest, testGetTempoAt)
{
	TempoList list;
	list.push(Tempo(480, 480000));
	list.push(Tempo(0, 500000));
	list.updateTempoInfo();

	EXPECT_EQ(500000, list.tempoAt(0));
	EXPECT_EQ(500000, list.tempoAt(479));
	EXPECT_EQ(480000, list.tempoAt(480));
}

TEST(TempoListTest, testClear)
{
	TempoList list;
	list.push(Tempo(480, 480000));
	EXPECT_EQ(1, list.size());
	list.clear();
	EXPECT_EQ(0, list.size());
}

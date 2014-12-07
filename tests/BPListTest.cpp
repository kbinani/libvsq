#include "Util.hpp"
#include "../include/libvsq/BPList.hpp"
#include "../include/libvsq/TextStream.hpp"

using namespace std;
using namespace vsq;

TEST(BPListTest, construct)
{
	BPList list("foo", 63, -10, 1000);
	EXPECT_EQ(string("foo"), list.name());
	EXPECT_EQ(63, list.defaultValue());
	EXPECT_EQ(-10, list.minimum());
	EXPECT_EQ(1000, list.maximum());
	EXPECT_EQ(0, list.maxUsedId());
}

TEST(BPListTest, testGetterAndSetterName)
{
	BPList list("foo", 63, -10, 1000);
	EXPECT_EQ(string("foo"), list.name());
	list.name("bar");
	EXPECT_EQ(string("bar"), list.name());
}

TEST(BPListTest, testmaxUsedId)
{
	BPList list("foo", 63, -10, 1000);
	list.add(0, 1);
	EXPECT_EQ(1, list.maxUsedId());
}

TEST(BPListTest, testGetterAndSetterDefaultValue)
{
	BPList list("foo", 63, -10, 1000);
	EXPECT_EQ(63, list.defaultValue());
	list.defaultValue(62);
	EXPECT_EQ(62, list.defaultValue());
}

TEST(BPListTest, testRenumberIds)
{
	BPList list("foo", 63, -10, 1000);
	list.addWithId(0, 1, 10000);
	list.addWithId(1, 2, 10001);
	list.renumberIds();

	EXPECT_TRUE(10000 != list.get(0).id);
	EXPECT_EQ(1, list.get(0).value);
	EXPECT_EQ((tick_t)0, list.keyTickAt(0));

	EXPECT_TRUE(10001 != list.get(1).id);
	EXPECT_EQ(2, list.get(1).value);
	EXPECT_EQ((tick_t)1, list.keyTickAt(1));
}

TEST(BPListTest, testGetData)
{
	BPList list("foo", 63, -10, 1000);
	list.addWithId(0, 101, 10000);
	list.addWithId(480, 103, 10001);
	string expected = "0=101,480=103";
	string actual = list.data();
	EXPECT_EQ(expected, actual);
}

TEST(BPListTest, testSetData)
{
	BPList list("foo", 63, -10, 1000);
	string value = "0=-11,240=50,480=1001";
	list.data(value);
	string expected = "0=-10,240=50,480=1000";
	string actual = list.data();
	EXPECT_EQ(expected, actual);
}

TEST(BPListTest, testGetterAndSetterMaximum)
{
	BPList list("foo", 63, -10, 1000);
	EXPECT_EQ(1000, list.maximum());
	list.maximum(1001);
	EXPECT_EQ(1001, list.maximum());
}

TEST(BPListTest, testGetterAndSetterMinimum)
{
	BPList list("foo", 63, -10, 1000);
	EXPECT_EQ(-10, list.minimum());
	list.minimum(1);
	EXPECT_EQ(1, list.minimum());
}

TEST(BPListTest, testRemove)
{
	BPList list("foo", 63, -10, 1000);
	list.add(0, 11);
	list.add(240, 13);
	list.add(480, 17);

	list.remove(240);
	EXPECT_EQ(2, list.size());
	string expected = "0=11,480=17";
	string actual = list.data();
	EXPECT_EQ(expected, actual);

	list.remove(100);
	EXPECT_EQ(2, list.size());
	actual = list.data();
	EXPECT_EQ(expected, actual);
}

TEST(BPListTest, testRemoveElementAt)
{
	BPList list("foo", 63, -10, 1000);
	list.add(0, 11);
	list.add(240, 13);
	list.add(480, 17);

	list.removeElementAt(1);
	string expected = "0=11,480=17";
	string actual = list.data();
	EXPECT_EQ(expected, actual);
}

TEST(BPListTest, testIsContainsKey)
{
	BPList list("foo", 63, -10, 1000);
	EXPECT_TRUE(false == list.isContainsKey(480));
	list.add(480, 1);
	EXPECT_TRUE(list.isContainsKey(480));
}

TEST(BPListTest, testMove)
{
	// 移動先にデータ点がない場合
	BPList listA("foo", 63, -10, 1000);
	listA.addWithId(0, 11, 1);
	listA.addWithId(240, 13, 2);
	listA.addWithId(480, 17, 3);
	listA.move(240, 481, 19);

	string expected = "0=11,480=17,481=19";
	EXPECT_EQ(expected, listA.data());
	EXPECT_EQ(3, listA.size());

	EXPECT_EQ(11, listA.get(0).value);
	EXPECT_EQ((tick_t)0, listA.keyTickAt(0));
	EXPECT_EQ(1, listA.get(0).id);

	EXPECT_EQ(17, listA.get(1).value);
	EXPECT_EQ((tick_t)480, listA.keyTickAt(1));
	EXPECT_EQ(3, listA.get(1).id);

	EXPECT_EQ(19, listA.get(2).value);
	EXPECT_EQ((tick_t)481, listA.keyTickAt(2));
	EXPECT_EQ(2, listA.get(2).id);

	// 移動先にデータがある場合
	BPList listB("foo", 63, -10, 1000);
	listB.addWithId(0, 11, 1);
	listB.addWithId(240, 13, 2);
	listB.addWithId(480, 17, 3);
	listB.move(240, 480, 19);

	expected = "0=11,480=19";
	EXPECT_EQ(2, listB.size());
	EXPECT_EQ(expected, listB.data());

	EXPECT_EQ(11, listB.get(0).value);
	EXPECT_EQ((tick_t)0, listB.keyTickAt(0));
	EXPECT_EQ(1, listB.get(0).id);

	EXPECT_EQ(19, listB.get(1).value);
	EXPECT_EQ((tick_t)480, listB.keyTickAt(1));
	EXPECT_EQ(2, listB.get(1).id);
}

TEST(BPListTest, testGetValue)
{
	BPList list("foo", 63, -10, 1000);
	list.add(480, 1);
	list.add(1920, 2);
	EXPECT_EQ(1, list.get(0).value);
	EXPECT_EQ(2, list.get(1).value);
}

TEST(BPListTest, testGet)
{
	BPList list("foo", 63, -10, 1000);
	list.add(480, 11);
	list.add(1920, 12);
	EXPECT_EQ(1, list.get(0).id);
	EXPECT_EQ(11, list.get(0).value);
	EXPECT_EQ(2, list.get(1).id);
	EXPECT_EQ(12, list.get(1).value);
}

TEST(BPListTest, testKeyTickAt)
{
	BPList list("foo", 63, -10, 1000);
	list.add(480, 11);
	list.add(1920, 12);
	EXPECT_EQ((tick_t)480, list.keyTickAt(0));
	EXPECT_EQ((tick_t)1920, list.keyTickAt(1));
}

TEST(BPListTest, testFindValueFromId)
{
	BPList list("foo", 63, -10, 1000);
	int idA = list.add(480, 11);
	int idB = list.add(1920, 12);
	EXPECT_EQ(11, list.findValueFromId(idA));
	EXPECT_EQ(12, list.findValueFromId(idB));
}

TEST(BPListTest, testFindElement)
{
	BPList list("foo", 63, -10, 1000);
	int idA = list.add(480, 11);
	int idB = list.add(1920, 12);
	BPListSearchResult resultA = list.findElement(idA);
	BPListSearchResult resultB = list.findElement(idB);
	EXPECT_EQ((tick_t)480, resultA.tick);
	EXPECT_EQ(0, resultA.index);
	EXPECT_EQ(idA, resultA.point.id);
	EXPECT_EQ(11, resultA.point.value);
	EXPECT_EQ((tick_t)1920, resultB.tick);
	EXPECT_EQ(1, resultB.index);
	EXPECT_EQ(idB, resultB.point.id);
	EXPECT_EQ(12, resultB.point.value);
}

TEST(BPListTest, testSetValueForId)
{
	BPList list("foo", 63, -10, 1000);
	int idA = list.add(480, 11);
	list.add(1920, 12);
	list.setValueForId(idA, 13);
	EXPECT_EQ(13, list.findValueFromId(idA));
}

TEST(BPListTest, testPrint)
{
	BPList list("foo", 63, -10, 1000);
	string header = "[BPList]";

	TextStream stream;
	list.print(stream, 0, header);
	string expected =
		"[BPList]\n";
	EXPECT_EQ(expected, stream.toString());
	stream.close();

	stream = TextStream();
	list.add(480, 11);
	list.add(1920, 12);
	list.print(stream, 0, header);
	expected =
		"[BPList]\n"
		"480=11\n"
		"1920=12\n";
	EXPECT_EQ(expected, stream.toString());
	stream.close();

	stream = TextStream();
	list.print(stream, 1921, header);
	expected =
		"[BPList]\n"
		"1921=12\n";
	EXPECT_EQ(expected, stream.toString());
}

TEST(BPListTest, testAppendFromText)
{
	BPList list("foo", 63, -10, 1000);
	TextStream stream;
	stream.writeLine("0=11");
	stream.writeLine("340=13");
	stream.writeLine("480=17");
	stream.writeLine("[foooo]");
	stream.writeLine("481=19");
	stream.setPointer(-1);
	string lastLine = list.appendFromText(stream);
	string expected = "0=11,340=13,480=17";
	EXPECT_EQ(expected, list.data());
	EXPECT_EQ(string("[foooo]"), lastLine);
}

TEST(BPListTest, testSize)
{
	BPList list("foo", 63, -10, 1000);
	EXPECT_EQ(0, list.size());
	list.add(480, 11);
	EXPECT_EQ(1, list.size());
}

TEST(BPListTest, testKeyTickIterator)
{
	BPList list("foo", 63, -10, 1000);
	list.add(480, 11);
	list.add(1920, 12);
	BPList::KeyTickIterator iterator = list.keyTickIterator();
	EXPECT_TRUE(iterator.hasNext());
	EXPECT_EQ((tick_t)480, iterator.next());
	EXPECT_TRUE(iterator.hasNext());
	EXPECT_EQ((tick_t)1920, iterator.next());
	EXPECT_TRUE(false == iterator.hasNext());
}

TEST(BPListTest, testAdd)
{
	BPList list("foo", 63, -10, 1000);
	EXPECT_EQ(0, list.size());
	int idA = list.add(480, 11);
	EXPECT_EQ(1, list.size());
	EXPECT_EQ(1, idA);
	EXPECT_EQ(11, list.get(0).value);

	// 同じtickに値をaddすると, データ点は増えずに値が上書きされる
	idA = list.add(480, 12);
	EXPECT_EQ(1, list.size());
	EXPECT_EQ(1, idA);
	EXPECT_EQ(12, list.get(0).value);

	// 既存の点より小さいtickに値をaddすると, 並び替えが起こる
	int idB = list.add(240, 99);
	EXPECT_EQ(2, list.size());
	EXPECT_TRUE(idA != idB);
	EXPECT_EQ((tick_t)240, list.keyTickAt(0));
	EXPECT_EQ(idB, list.get(0).id);
	EXPECT_EQ(99, list.get(0).value);
	EXPECT_EQ((tick_t)480, list.keyTickAt(1));
	EXPECT_EQ(idA, list.get(1).id);
	EXPECT_EQ(12, list.get(1).value);
}

TEST(BPListTest, testAddWithId)
{
	BPList list("foo", 63, -10, 1000);
	int idA = list.addWithId(480, 11, 3);
	EXPECT_EQ(3, idA);
	EXPECT_EQ(11, list.get(0).value);
	EXPECT_EQ(3, list.maxUsedId());

	// 同じtickに値をaddすると, データ点は増えずに値が上書きされる
	idA = list.addWithId(480, 12, 4);
	EXPECT_EQ(4, idA);
	EXPECT_EQ(12, list.get(0).value);
	EXPECT_EQ(4, list.maxUsedId());

	// 既存の点より小さいtickに値をaddすると, 並び替えが起こる
	int idB = list.addWithId(240, 99, 5);
	EXPECT_EQ(5, idB);
	EXPECT_EQ(5, list.maxUsedId());
	EXPECT_EQ((tick_t)240, list.keyTickAt(0));
	EXPECT_EQ(5, list.get(0).id);
	EXPECT_EQ(99, list.get(0).value);
	EXPECT_EQ((tick_t)480, list.keyTickAt(1));
	EXPECT_EQ(4, list.get(1).id);
	EXPECT_EQ(12, list.get(1).value);
}

TEST(BPListTest, testRemoveWithId)
{
	BPList list("foo", 63, -10, 1000);
	list.addWithId(480, 11, 1);
	list.addWithId(1920, 12, 2);

	list.removeWithId(1);
	string expected = "1920=12";
	EXPECT_EQ(expected, list.data());

	// 存在しないIDを削除しようとする
	list.removeWithId(1);
	EXPECT_EQ(expected, list.data());
}

TEST(BPListTest, testGetValueAtWithoutLastIndex)
{
	BPList list("foo", 63, -10, 1000);
	list.add(480, 11);
	list.add(1920, 12);
	EXPECT_EQ(63, list.getValueAt(479));
	EXPECT_EQ(11, list.getValueAt(480));
	EXPECT_EQ(12, list.getValueAt(2000));
}

TEST(BPListTest, testGetValueAtWithLastIndex)
{
	BPList list("foo", 63, -10, 1000);
	list.add(480, 11);
	list.add(1920, 12);
	int index = 0;
	EXPECT_EQ(63, list.getValueAt(479, index));
	EXPECT_EQ(0, index);
	EXPECT_EQ(11, list.getValueAt(480, index));
	EXPECT_EQ(0, index);
	EXPECT_EQ(12, list.getValueAt(2000, index));
	EXPECT_EQ(1, index);
	EXPECT_EQ(63, list.getValueAt(479, index));
	EXPECT_EQ(0, index);
}

TEST(BPListTest, testClone)
{
	BPList list("foo", 63, -10, 1000);
	list.add(480, 1);
	list.add(1920, 2);
	BPList copy = list.clone();
	EXPECT_EQ(string("foo"), copy.name());
	EXPECT_EQ(63, copy.defaultValue());
	EXPECT_EQ(-10, copy.minimum());
	EXPECT_EQ(1000, copy.maximum());
	EXPECT_EQ(2, copy.size());
	EXPECT_EQ(1, copy.get(0).id);
	EXPECT_EQ(1, copy.get(0).value);
	EXPECT_EQ((tick_t)480, copy.keyTickAt(0));
	EXPECT_EQ(2, copy.get(1).id);
	EXPECT_EQ(2, copy.get(1).value);
	EXPECT_EQ((tick_t)1920, copy.keyTickAt(1));
}

TEST(BPListKeyTickIteratorTest, test)
{
	BPList list("foo", 63, -10, 1000);
	BPList::KeyTickIterator iterator = BPList::KeyTickIterator(&list);
	EXPECT_TRUE(false == iterator.hasNext());
	list.add(480, 1);
	list.add(1920, 2);

	iterator = BPList::KeyTickIterator(&list);
	EXPECT_TRUE(iterator.hasNext());
	EXPECT_EQ((tick_t)480, iterator.next());
	EXPECT_TRUE(iterator.hasNext());
	EXPECT_EQ((tick_t)1920, iterator.next());
	EXPECT_TRUE(false == iterator.hasNext());

	iterator = BPList::KeyTickIterator(&list);
	EXPECT_TRUE(iterator.hasNext());
	EXPECT_EQ((tick_t)480, iterator.next());
	iterator.remove();
	EXPECT_EQ((tick_t)1920, iterator.next());
	EXPECT_TRUE(false == iterator.hasNext());
}

#include "Util.hpp"
#include "../include/libvsq/VibratoBPList.hpp"

using namespace std;
using namespace vsq;

TEST(VibratoBPListTest, testConstructWithString)
{
	VibratoBPList list = VibratoBPList("2", "1.0,0.0", "128,1");
	EXPECT_EQ(2, list.size());
	EXPECT_EQ(string("0=1,1=128"), list.data());
}

TEST(VibratoBPListTest, testConstructWithArray)
{
	vector<double> x;
	x.push_back(1.0);
	x.push_back(0.0);
	vector<int> y;
	y.push_back(128);
	y.push_back(1);
	VibratoBPList list = VibratoBPList(x, y);
	EXPECT_EQ(2, list.size());
	EXPECT_EQ(string("0=1,1=128"), list.data());
}

TEST(VibratoBPListTest, testGetValueAt)
{
	VibratoBPList list = VibratoBPList("2", "0.0,1.0", "1,128");
	EXPECT_EQ(1, list.getValueAt(0.0, 64));
	EXPECT_EQ(1, list.getValueAt(0.99999, 64));
	EXPECT_EQ(128, list.getValueAt(1.0, 64));
	EXPECT_EQ(64, list.getValueAt(-0.0000001, 64));
}

TEST(VibratoBPListTest, testGetCount)
{
	VibratoBPList list = VibratoBPList("3", "0.0,0.4,1.0", "1,64,128");
	EXPECT_EQ(3, list.size());
}

TEST(VibratoBPListTest, testGet)
{
	VibratoBPList list = VibratoBPList("3", "0.4,0.0,1.0", "64,1,128");
	EXPECT_EQ(0.0, list.get(0).x);
	EXPECT_EQ(1, list.get(0).y);
	EXPECT_EQ(0.4, list.get(1).x);
	EXPECT_EQ(64, list.get(1).y);
	EXPECT_EQ(1.0, list.get(2).x);
	EXPECT_EQ(128, list.get(2).y);
}

TEST(VibratoBPListTest, testSet)
{
	VibratoBPList list = VibratoBPList("3", "0.0,0.4,1.0", "1,64,128");
	list.set(1, VibratoBP(0.2, 32));
	EXPECT_EQ(0.2, list.get(1).x);
	EXPECT_EQ(32, list.get(1).y);
}

TEST(VibratoBPListTest, testGetData)
{
	VibratoBPList list = VibratoBPList("3", "0.0,0.4,1.0", "1,64,128");
	EXPECT_EQ(string("0=1,0.4=64,1=128"), list.data());
	list = VibratoBPList(vector<double>(), vector<int>());
	EXPECT_EQ(string(""), list.data());
}

TEST(VibratoBPListTest, testSetData)
{
	VibratoBPList list = VibratoBPList(vector<double>(), vector<int>());
	list.data("0.4=64,0=1,1=128");
	EXPECT_EQ(3, list.size());
	EXPECT_EQ(0.0, list.get(0).x);
	EXPECT_EQ(1, list.get(0).y);
	EXPECT_EQ(0.4, list.get(1).x);
	EXPECT_EQ(64, list.get(1).y);
	EXPECT_EQ(1.0, list.get(2).x);
	EXPECT_EQ(128, list.get(2).y);
}

TEST(VibratoBPListTest, testClone)
{
	std::vector<double> xList;
	xList.push_back(0.0);
	xList.push_back(1.0);
	std::vector<int> yList;
	yList.push_back(1);
	yList.push_back(128);
	VibratoBPList list(xList, yList);
	VibratoBPList copy = list.clone();
	EXPECT_EQ(string("0=1,1=128"), copy.data());
}

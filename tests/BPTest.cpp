#include "Util.hpp"
#include "../include/libvsq/BP.hpp"

using namespace std;
using namespace vsq;

TEST(BPTest, testConstruct)
{
	int value = 64;
	int id = 1;
	BP point(value, id);
	EXPECT_EQ(64, point.value);
	EXPECT_EQ(1, point.id);
}

TEST(BPTest, testClone)
{
	int value = 64;
	int id = 1;
	BP point(value, id);
	BP copy = point.clone();
	EXPECT_EQ(64, copy.value);
	EXPECT_EQ(1, copy.id);
}

#include "Util.hpp"
#include "../include/libvsq/VibratoBP.hpp"

using namespace std;
using namespace vsq;

TEST(VibratoBPTest, testConstruct)
{
	VibratoBP point;
	EXPECT_EQ(0.0, point.x);
	EXPECT_EQ(0, point.y);
}

TEST(VibratoBPTest, testConstructWithCoordinate)
{
	VibratoBP point = VibratoBP(2.0, 3);
	EXPECT_EQ(2.0, point.x);
	EXPECT_EQ(3, point.y);
}

TEST(VibratoBPTest, testCompareTo)
{
	VibratoBP a = VibratoBP(2.0, 3);
	VibratoBP b = VibratoBP(2.0, 10);
	EXPECT_EQ(0, a.compareTo(b));

	VibratoBP c = VibratoBP(1.0, 3);
	EXPECT_EQ(1, b.compareTo(c));
	EXPECT_EQ(-1, c.compareTo(b));
}

TEST(VibratoBPTest, testCompare)
{
	VibratoBP a = VibratoBP(2.0, 3);
	VibratoBP b = VibratoBP(2.0, 10);
	EXPECT_EQ(false, VibratoBP::compare(a, b));

	VibratoBP c = VibratoBP(1.0, 3);
	EXPECT_EQ(false, VibratoBP::compare(b, c));
	EXPECT_EQ(true, VibratoBP::compare(c, b));
}

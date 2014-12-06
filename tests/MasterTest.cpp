#include "../include/libvsq/Master.hpp"
#include "../include/libvsq/TextStream.hpp"
#include "Util.hpp"

using namespace std;
using namespace vsq;

TEST(MasterTest, testConstructWithoutArgument)
{
	Master master;
	EXPECT_EQ(1, master.preMeasure);
}

TEST(MasterTest, testConstructWithPreMeasure)
{
	Master master(10);
	EXPECT_EQ(10, master.preMeasure);
}

TEST(MasterTest, testClone)
{
	Master master(15);
	Master copy = master.clone();
	EXPECT_EQ(15, copy.preMeasure);
}

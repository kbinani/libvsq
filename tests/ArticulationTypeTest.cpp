#include "Util.hpp"
#include "../include/libvsq/ArticulationType.hpp"

using namespace vsq;

TEST(ArticulationTypeTest, test)
{
	ArticulationType e = ArticulationType::CRESCENDO;
	EXPECT_NE(e, ArticulationType::DYNAFF);
	EXPECT_EQ(e, ArticulationType::CRESCENDO);
}

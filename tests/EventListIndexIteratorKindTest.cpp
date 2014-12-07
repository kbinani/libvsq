#include "Util.hpp"
#include "../include/libvsq/EventListIndexIteratorKind.hpp"

using namespace vsq;

TEST(EventListIndexIteratorKindTest, test)
{
	EXPECT_EQ(1, (int)EventListIndexIteratorKind::SINGER);
	EXPECT_EQ(2, (int)EventListIndexIteratorKind::NOTE);
	EXPECT_EQ(4, (int)EventListIndexIteratorKind::CRESCENDO);
	EXPECT_EQ(8, (int)EventListIndexIteratorKind::DECRESCENDO);
	EXPECT_EQ(16, (int)EventListIndexIteratorKind::DYNAFF);
}

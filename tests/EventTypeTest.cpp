#include "Util.hpp"
#include "../include/libvsq/EventType.hpp"

using namespace std;
using namespace vsq;

TEST(EventTypeTest, testToString)
{
	EXPECT_EQ(string("Singer"), EventTypeUtil::toString(EventType::SINGER));
	EXPECT_EQ(string("Anote"), EventTypeUtil::toString(EventType::NOTE));
	EXPECT_EQ(string("Aicon"), EventTypeUtil::toString(EventType::ICON));
	EXPECT_EQ(string("Unknown"), EventTypeUtil::toString(EventType::UNKNOWN));
}

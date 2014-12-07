#include "Util.hpp"
#include "../include/libvsq/HandleType.hpp"

TEST(HandleTypeTest, test)
{
	EXPECT_EQ(0, (int)vsq::HandleType::UNKNOWN);
	EXPECT_EQ(1, (int)vsq::HandleType::LYRIC);
	EXPECT_EQ(2, (int)vsq::HandleType::VIBRATO);
	EXPECT_EQ(3, (int)vsq::HandleType::SINGER);
	EXPECT_EQ(4, (int)vsq::HandleType::NOTE_HEAD);
	EXPECT_EQ(5, (int)vsq::HandleType::DYNAMICS);
}

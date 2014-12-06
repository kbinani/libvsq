#include "Util.hpp"
#include "../include/libvsq/PlayMode.hpp"

using namespace std;
using namespace vsq;

TEST(PlayModeTest, test)
{
	EXPECT_EQ(-1, (int)PlayMode::OFF);
	EXPECT_EQ(0, (int)PlayMode::PLAY_AFTER_SYNTH);
	EXPECT_EQ(1, (int)PlayMode::PLAY_WITH_SYNTH);
}

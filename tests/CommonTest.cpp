#include "Util.hpp"
#include "../include/libvsq/Common.hpp"
#include "../include/libvsq/TextStream.hpp"

using namespace std;
using namespace vsq;

void constructFromArguments()
{
	Common common("__foo__", 3, 4, 5, DynamicsMode::STANDARD, PlayMode::PLAY_AFTER_SYNTH);
	EXPECT_EQ(string("__foo__"), common.name);
	EXPECT_EQ(string("3,4,5"), common.color);
	EXPECT_EQ(DynamicsMode::STANDARD, common.dynamicsMode);
	EXPECT_EQ(PlayMode::PLAY_AFTER_SYNTH, common.playMode());
}

TEST(CommonTest, clone)
{
	Common common("__foo__", 3, 4, 5, DynamicsMode::STANDARD, PlayMode::PLAY_AFTER_SYNTH);
	Common copy = common.clone();
	EXPECT_EQ(string("__foo__"), copy.name);
	EXPECT_EQ(string("3,4,5"), copy.color);
	EXPECT_EQ(DynamicsMode::STANDARD, copy.dynamicsMode);
	EXPECT_EQ(PlayMode::PLAY_AFTER_SYNTH, copy.playMode());
}

TEST(CommonTest, lastPlayMode)
{
	Common common;
	common.playMode(PlayMode::PLAY_AFTER_SYNTH);
	common.playMode(PlayMode::OFF);
	EXPECT_EQ(PlayMode::PLAY_AFTER_SYNTH, common.lastPlayMode());
}

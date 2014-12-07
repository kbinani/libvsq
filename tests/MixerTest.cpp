#include "Util.hpp"
#include "../include/libvsq/Mixer.hpp"
#include "../include/libvsq/TextStream.hpp"

using namespace vsq;

TEST(MixerTest, testConstruct)
{
	Mixer mixer(1, 2, 3, 4);
	EXPECT_EQ(1, mixer.masterFeder);
	EXPECT_EQ(2, mixer.masterPanpot);
	EXPECT_EQ(3, mixer.masterMute);
	EXPECT_EQ(4, mixer.outputMode);
	EXPECT_EQ((std::size_t)0, mixer.slave.size());
}

TEST(MixerTest, testClone)
{
	Mixer mixer(1, 2, 3, 4);
	mixer.slave.push_back(MixerItem(5, 6, 7, 8));
	mixer.slave.push_back(MixerItem(9, 10, 11, 12));

	Mixer copy = mixer.clone();
	EXPECT_EQ((std::size_t)2, copy.slave.size());
	EXPECT_EQ(1, copy.masterFeder);
	EXPECT_EQ(2, copy.masterPanpot);
	EXPECT_EQ(3, copy.masterMute);
	EXPECT_EQ(4, copy.outputMode);
	EXPECT_EQ(5, copy.slave[0].feder);
	EXPECT_EQ(6, copy.slave[0].panpot);
	EXPECT_EQ(7, copy.slave[0].mute);
	EXPECT_EQ(8, copy.slave[0].solo);
	EXPECT_EQ(9, copy.slave[1].feder);
	EXPECT_EQ(10, copy.slave[1].panpot);
	EXPECT_EQ(11, copy.slave[1].mute);
	EXPECT_EQ(12, copy.slave[1].solo);
}

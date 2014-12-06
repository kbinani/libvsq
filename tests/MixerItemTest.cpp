#include "Util.hpp"
#include "../include/libvsq/MixerItem.hpp"

using namespace vsq;

TEST(MixerItemTest, testConstruct)
{
	MixerItem mixerEntry(1, 2, 3, 4);
	EXPECT_EQ(1, mixerEntry.feder);
	EXPECT_EQ(2, mixerEntry.panpot);
	EXPECT_EQ(3, mixerEntry.mute);
	EXPECT_EQ(4, mixerEntry.solo);
}

TEST(MixerItemTest, testClone)
{
	MixerItem mixerEntry(1, 2, 3, 4);
	MixerItem copy = mixerEntry.clone();
	EXPECT_EQ(1, copy.feder);
	EXPECT_EQ(2, copy.panpot);
	EXPECT_EQ(3, copy.mute);
	EXPECT_EQ(4, copy.solo);
}

#include "Util.hpp"
#include "../include/libvsq/Mixer.hpp"
#include "../include/libvsq/TextStream.hpp"

using namespace vsq;

class MixerTest : public CppUnit::TestCase
{
public:
	void testConstruct()
	{
		Mixer mixer(1, 2, 3, 4);
		CPPUNIT_ASSERT_EQUAL(1, mixer.masterFeder);
		CPPUNIT_ASSERT_EQUAL(2, mixer.masterPanpot);
		CPPUNIT_ASSERT_EQUAL(3, mixer.masterMute);
		CPPUNIT_ASSERT_EQUAL(4, mixer.outputMode);
		CPPUNIT_ASSERT_EQUAL((std::size_t)0, mixer.slave.size());
	}

	void testClone()
	{
		Mixer mixer(1, 2, 3, 4);
		mixer.slave.push_back(MixerItem(5, 6, 7, 8));
		mixer.slave.push_back(MixerItem(9, 10, 11, 12));

		Mixer copy = mixer.clone();
		CPPUNIT_ASSERT_EQUAL((std::size_t)2, copy.slave.size());
		CPPUNIT_ASSERT_EQUAL(1, copy.masterFeder);
		CPPUNIT_ASSERT_EQUAL(2, copy.masterPanpot);
		CPPUNIT_ASSERT_EQUAL(3, copy.masterMute);
		CPPUNIT_ASSERT_EQUAL(4, copy.outputMode);
		CPPUNIT_ASSERT_EQUAL(5, copy.slave[0].feder);
		CPPUNIT_ASSERT_EQUAL(6, copy.slave[0].panpot);
		CPPUNIT_ASSERT_EQUAL(7, copy.slave[0].mute);
		CPPUNIT_ASSERT_EQUAL(8, copy.slave[0].solo);
		CPPUNIT_ASSERT_EQUAL(9, copy.slave[1].feder);
		CPPUNIT_ASSERT_EQUAL(10, copy.slave[1].panpot);
		CPPUNIT_ASSERT_EQUAL(11, copy.slave[1].mute);
		CPPUNIT_ASSERT_EQUAL(12, copy.slave[1].solo);
	}

	CPPUNIT_TEST_SUITE(MixerTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(MixerTest);

#include "Util.hpp"
#include "../include/libvsq/MixerItem.hpp"

using namespace vsq;

class MixerItemTest : public CppUnit::TestCase
{
public:
	void testConstruct()
	{
		MixerItem mixerEntry(1, 2, 3, 4);
		CPPUNIT_ASSERT_EQUAL(1, mixerEntry.feder);
		CPPUNIT_ASSERT_EQUAL(2, mixerEntry.panpot);
		CPPUNIT_ASSERT_EQUAL(3, mixerEntry.mute);
		CPPUNIT_ASSERT_EQUAL(4, mixerEntry.solo);
	}

	void testClone()
	{
		MixerItem mixerEntry(1, 2, 3, 4);
		MixerItem copy = mixerEntry.clone();
		CPPUNIT_ASSERT_EQUAL(1, copy.feder);
		CPPUNIT_ASSERT_EQUAL(2, copy.panpot);
		CPPUNIT_ASSERT_EQUAL(3, copy.mute);
		CPPUNIT_ASSERT_EQUAL(4, copy.solo);
	}

	CPPUNIT_TEST_SUITE(MixerItemTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(MixerItemTest);

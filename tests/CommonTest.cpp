#include "Util.hpp"
#include "../include/libvsq/Common.hpp"
#include "../include/libvsq/TextStream.hpp"

using namespace std;
using namespace vsq;

class CommonTest : public CppUnit::TestCase
{
public:
	void constructFromArguments()
	{
		Common common("__foo__", 3, 4, 5, DynamicsMode::STANDARD, PlayMode::PLAY_AFTER_SYNTH);
		CPPUNIT_ASSERT_EQUAL(string("__foo__"), common.name);
		CPPUNIT_ASSERT_EQUAL(string("3,4,5"), common.color);
		CPPUNIT_ASSERT_EQUAL(DynamicsMode::STANDARD, common.dynamicsMode);
		CPPUNIT_ASSERT_EQUAL(PlayMode::PLAY_AFTER_SYNTH, common.playMode());
	}

	void clone()
	{
		Common common("__foo__", 3, 4, 5, DynamicsMode::STANDARD, PlayMode::PLAY_AFTER_SYNTH);
		Common copy = common.clone();
		CPPUNIT_ASSERT_EQUAL(string("__foo__"), copy.name);
		CPPUNIT_ASSERT_EQUAL(string("3,4,5"), copy.color);
		CPPUNIT_ASSERT_EQUAL(DynamicsMode::STANDARD, copy.dynamicsMode);
		CPPUNIT_ASSERT_EQUAL(PlayMode::PLAY_AFTER_SYNTH, copy.playMode());
	}

	void lastPlayMode()
	{
		Common common;
		common.playMode(PlayMode::PLAY_AFTER_SYNTH);
		common.playMode(PlayMode::OFF);
		CPPUNIT_ASSERT_EQUAL(PlayMode::PLAY_AFTER_SYNTH, common.lastPlayMode());
	}

	CPPUNIT_TEST_SUITE(CommonTest);
	CPPUNIT_TEST(constructFromArguments);
	CPPUNIT_TEST(clone);
	CPPUNIT_TEST(lastPlayMode);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(CommonTest);

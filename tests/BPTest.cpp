#include "Util.hpp"
#include "../include/libvsq/BP.hpp"

using namespace std;
using namespace vsq;

class BPTest : public CppUnit::TestCase
{
public:
	void testConstruct()
	{
		int value = 64;
		int id = 1;
		BP point(value, id);
		CPPUNIT_ASSERT_EQUAL(64, point.value);
		CPPUNIT_ASSERT_EQUAL(1, point.id);
	}

	void testClone()
	{
		int value = 64;
		int id = 1;
		BP point(value, id);
		BP copy = point.clone();
		CPPUNIT_ASSERT_EQUAL(64, copy.value);
		CPPUNIT_ASSERT_EQUAL(1, copy.id);
	}

	CPPUNIT_TEST_SUITE(BPTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(BPTest);

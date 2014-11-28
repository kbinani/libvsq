#include "Util.hpp"
#include "../EventType.hpp"

using namespace std;
using namespace vsq;

class EventTypeTest : public CppUnit::TestCase
{
public:
	void testToString()
	{
		CPPUNIT_ASSERT_EQUAL(string("Singer"), EventTypeUtil::toString(EventType::SINGER));
		CPPUNIT_ASSERT_EQUAL(string("Anote"), EventTypeUtil::toString(EventType::NOTE));
		CPPUNIT_ASSERT_EQUAL(string("Aicon"), EventTypeUtil::toString(EventType::ICON));
		CPPUNIT_ASSERT_EQUAL(string("Unknown"), EventTypeUtil::toString(EventType::UNKNOWN));
	}

	CPPUNIT_TEST_SUITE(EventTypeTest);
	CPPUNIT_TEST(testToString);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(EventTypeTest);

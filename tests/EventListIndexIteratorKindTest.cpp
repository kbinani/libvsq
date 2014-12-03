#include "Util.hpp"
#include "../EventListIndexIteratorKind.hpp"

using namespace VSQ_NS;

class EventListIndexIteratorKindTest : public CppUnit::TestCase
{
public:
	void test()
	{
		CPPUNIT_ASSERT_EQUAL(1, (int)EventListIndexIteratorKind::SINGER);
		CPPUNIT_ASSERT_EQUAL(2, (int)EventListIndexIteratorKind::NOTE);
		CPPUNIT_ASSERT_EQUAL(4, (int)EventListIndexIteratorKind::CRESCENDO);
		CPPUNIT_ASSERT_EQUAL(8, (int)EventListIndexIteratorKind::DECRESCENDO);
		CPPUNIT_ASSERT_EQUAL(16, (int)EventListIndexIteratorKind::DYNAFF);
	}

	CPPUNIT_TEST_SUITE(EventListIndexIteratorKindTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(EventListIndexIteratorKindTest);

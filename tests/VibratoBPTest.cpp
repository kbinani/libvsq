#include "Util.hpp"
#include "../include/libvsq/VibratoBP.hpp"

using namespace std;
using namespace vsq;

class VibratoBPTest : public CppUnit::TestCase
{
public:
	void testConstruct()
	{
		VibratoBP point;
		CPPUNIT_ASSERT_EQUAL(0.0, point.x);
		CPPUNIT_ASSERT_EQUAL(0, point.y);
	}

	void testConstructWithCoordinate()
	{
		VibratoBP point = VibratoBP(2.0, 3);
		CPPUNIT_ASSERT_EQUAL(2.0, point.x);
		CPPUNIT_ASSERT_EQUAL(3, point.y);
	}

	void testCompareTo()
	{
		VibratoBP a = VibratoBP(2.0, 3);
		VibratoBP b = VibratoBP(2.0, 10);
		CPPUNIT_ASSERT_EQUAL(0, a.compareTo(b));

		VibratoBP c = VibratoBP(1.0, 3);
		CPPUNIT_ASSERT_EQUAL(1, b.compareTo(c));
		CPPUNIT_ASSERT_EQUAL(-1, c.compareTo(b));
	}

	void testCompare()
	{
		VibratoBP a = VibratoBP(2.0, 3);
		VibratoBP b = VibratoBP(2.0, 10);
		CPPUNIT_ASSERT_EQUAL(false, VibratoBP::compare(a, b));

		VibratoBP c = VibratoBP(1.0, 3);
		CPPUNIT_ASSERT_EQUAL(false, VibratoBP::compare(b, c));
		CPPUNIT_ASSERT_EQUAL(true, VibratoBP::compare(c, b));
	}

	CPPUNIT_TEST_SUITE(VibratoBPTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testConstructWithCoordinate);
	CPPUNIT_TEST(testCompareTo);
	CPPUNIT_TEST(testCompare);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(VibratoBPTest);

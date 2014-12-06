#include "../include/libvsq/Master.hpp"
#include "../include/libvsq/TextStream.hpp"
#include "Util.hpp"

using namespace std;
using namespace vsq;

class MasterTest : public CppUnit::TestCase
{
public:
	void testConstructWithoutArgument()
	{
		Master master;
		CPPUNIT_ASSERT_EQUAL(1, master.preMeasure);
	}

	void testConstructWithPreMeasure()
	{
		Master master(10);
		CPPUNIT_ASSERT_EQUAL(10, master.preMeasure);
	}

	void testClone()
	{
		Master master(15);
		Master copy = master.clone();
		CPPUNIT_ASSERT_EQUAL(15, copy.preMeasure);
	}

	CPPUNIT_TEST_SUITE(MasterTest);
	CPPUNIT_TEST(testConstructWithoutArgument);
	CPPUNIT_TEST(testConstructWithPreMeasure);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(MasterTest);

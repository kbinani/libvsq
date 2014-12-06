#include "Util.hpp"
#include "../include/libvsq/ArticulationType.hpp"

using namespace vsq;

class ArticulationTypeTest : public CppUnit::TestCase
{
public:
	void test()
	{
		ArticulationType e = ArticulationType::CRESCENDO;
		CPPUNIT_ASSERT(e != ArticulationType::DYNAFF);
		CPPUNIT_ASSERT(e == ArticulationType::CRESCENDO);
	}

	CPPUNIT_TEST_SUITE(ArticulationTypeTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(ArticulationTypeTest);

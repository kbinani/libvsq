#include "Util.hpp"
#include "../include/libvsq/EventWriteOption.hpp"

using namespace std;
using namespace vsq;

class EventWriteOptionTest : public CppUnit::TestCase
{
public:
	void testConstruct()
	{
		EventWriteOption option;
		CPPUNIT_ASSERT(option.length);
		CPPUNIT_ASSERT(option.note);
		CPPUNIT_ASSERT(option.dynamics);
		CPPUNIT_ASSERT(option.pmBendDepth);
		CPPUNIT_ASSERT(option.pmBendLength);
		CPPUNIT_ASSERT(option.pmbPortamentoUse);
		CPPUNIT_ASSERT(option.demDecGainRate);
		CPPUNIT_ASSERT(option.demAccent);
	}

	CPPUNIT_TEST_SUITE(EventWriteOptionTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(EventWriteOptionTest);

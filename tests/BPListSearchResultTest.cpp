#include "Util.hpp"
#include "../include/libvsq/BPListSearchResult.hpp"

using namespace std;
using namespace vsq;

class BPListSearchResultTest : public CppUnit::TestCase
{
public:
	void testConstruct()
	{
		BPListSearchResult result;
		CPPUNIT_ASSERT_EQUAL((tick_t)0, result.tick);
		CPPUNIT_ASSERT_EQUAL(0, result.index);
	}

	CPPUNIT_TEST_SUITE(BPListSearchResultTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(BPListSearchResultTest);

#include "Util.hpp"
#include "../SMFReader.hpp"
#include "../FileInputStream.hpp"

using namespace std;
using namespace vsq;

class SMFReaderTest : public CppUnit::TestCase
{
public:
	void testRead()
	{
		FileInputStream stream(TestUtil::getFixtureRootPath() + "/VSQFileWriterTest/expected/expected.vsq");
		SMFReader reader;
		vector<vector<MidiEvent> > actual;
		int format, timeFormat;
		reader.read(&stream, actual, format, timeFormat);
		stream.close();

		CPPUNIT_ASSERT_EQUAL((size_t)2, actual.size());

		// 1トラック(Masterトラック)
		CPPUNIT_ASSERT_EQUAL((size_t)4, actual[0].size());

		// 2トラック(メタイベント8個+NRPNイベント90個+メタイベント[Track End]1個)
		CPPUNIT_ASSERT_EQUAL((size_t)99, actual[1].size());

		CPPUNIT_ASSERT_EQUAL(1, format);
		CPPUNIT_ASSERT_EQUAL(480, timeFormat);
	}

	CPPUNIT_TEST_SUITE(SMFReaderTest);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(SMFReaderTest);

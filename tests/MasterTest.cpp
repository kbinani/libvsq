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

	void testConstructFromStream()
	{
		{
			TextStream stream;
			stream.writeLine("PreMeasure=12");
			stream.setPointer(-1);
			std::string lastLine = "";
			Master master(stream, lastLine);
			CPPUNIT_ASSERT_EQUAL(12, master.preMeasure);
			CPPUNIT_ASSERT_EQUAL(string("PreMeasure=12"), lastLine);
		}
		{
			TextStream stream;
			stream.writeLine("PreMeasure=12");
			stream.writeLine("[Common]");
			stream.setPointer(-1);
			std::string lastLine = "";
			Master master(stream, lastLine);
			CPPUNIT_ASSERT_EQUAL(12, master.preMeasure);
			CPPUNIT_ASSERT_EQUAL(string("[Common]"), lastLine);
		}
	}

	void testWrite()
	{
		TextStream stream;
		Master master(14);
		master.write(stream);
		std::string expected =
			"[Master]\n"
			"PreMeasure=14\n";
		CPPUNIT_ASSERT_EQUAL(expected, stream.toString());
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
	CPPUNIT_TEST(testConstructFromStream);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(MasterTest);

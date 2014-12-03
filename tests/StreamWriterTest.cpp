#include "Util.hpp"
#include "../include/libvsq/StreamWriter.hpp"
#include "../include/libvsq/FileOutputStream.hpp"

using namespace std;
using namespace vsq;

class StreamWriterTest : public CppUnit::TestCase
{
public:
	void test()
	{
		StreamWriter writer("foo.txt");
		writer.write("foo");
		writer.writeLine("bar");
		writer.close();

		std::ostringstream expected;
		expected << "foobar" << (char)0x0A;

		std::ostringstream actual;
		FILE* file = fopen("foo.txt", "rb");
		int c;
		while ((c = fgetc(file)) >= 0) {
			actual << (char)c;
		}
		fclose(file);

		CPPUNIT_ASSERT_EQUAL(expected.str(), actual.str());
	}

	void testWithException()
	{
		CPPUNIT_ASSERT_THROW(StreamWriter(""), TextOutputStream::IOException);
	}

	void testConstructByStream()
	{
		FileOutputStream stream("foo.txt");
		StreamWriter writer(&stream);
		writer.write("foo");
		writer.writeLine("bar");
		writer.close();

		std::ostringstream expected;
		expected << "foobar" << (char)0x0A;

		std::ostringstream actual;
		FILE* file = fopen("foo.txt", "rb");
		int c;
		while ((c = fgetc(file)) >= 0) {
			actual << (char)c;
		}
		fclose(file);

		CPPUNIT_ASSERT_EQUAL(expected.str(), actual.str());
	}

	CPPUNIT_TEST_SUITE(StreamWriterTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testWithException);
	CPPUNIT_TEST(testConstructByStream);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(StreamWriterTest);

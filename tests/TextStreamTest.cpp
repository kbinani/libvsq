#include "Util.hpp"
#include "../include/libvsq/TextStream.hpp"

using namespace std;
using namespace vsq;

class TextStreamTest : public CppUnit::TestCase
{
public:
	void testConstruct()
	{
		TextStream stream;
		CPPUNIT_ASSERT(false == stream.ready());
		CPPUNIT_ASSERT_EQUAL(string(""), stream.toString());
		CPPUNIT_ASSERT(0 > stream.getPointer());
	}

	void testReadLine()
	{
		TextStream stream;
		stream.write("hel\nlo");
		stream.setPointer(-1);
		CPPUNIT_ASSERT(stream.ready());
		CPPUNIT_ASSERT_EQUAL(string("hel"), stream.readLine());
		CPPUNIT_ASSERT(stream.ready());
		string actual = stream.readLine();
		CPPUNIT_ASSERT_EQUAL(string("lo"), actual);
		CPPUNIT_ASSERT(false == stream.ready());
	}

	void testWrite()
	{
		TextStream stream;
		stream.write("foo");
		CPPUNIT_ASSERT_EQUAL(2, stream.getPointer());
		stream.setPointer(-1);
		CPPUNIT_ASSERT_EQUAL(string("foo"), stream.readLine());
	}

	void testWriteLine()
	{
		TextStream stream;
		stream.writeLine("foo");
		CPPUNIT_ASSERT_EQUAL(3, stream.getPointer());
		stream.setPointer(-1);
		CPPUNIT_ASSERT_EQUAL(string("foo"), stream.readLine());
	}

	void testClose()
	{
		TextStream stream;
		stream.writeLine("foo");
		// エラーが起きなければ良しとする
		stream.close();
	}

	void testToString()
	{
		TextStream stream;
		stream.writeLine("foo");
		stream.writeLine("bar");
		CPPUNIT_ASSERT_EQUAL(string("foo\nbar\n"), stream.toString());
	}

	CPPUNIT_TEST_SUITE(TextStreamTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testReadLine);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST(testWriteLine);
	CPPUNIT_TEST(testClose);
	CPPUNIT_TEST(testToString);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(TextStreamTest);

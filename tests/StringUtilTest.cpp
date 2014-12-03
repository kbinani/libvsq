#include "Util.hpp"
#include "../include/libvsq/StringUtil.hpp"

using namespace std;
using namespace vsq;

class StringUtilTest : public CppUnit::TestFixture
{
public:
	void explode()
	{
		vector<string> splitted = StringUtil::explode(",", "aaa,bbb,ccc", 2);
		CPPUNIT_ASSERT_EQUAL((vector<string>::size_type)2, splitted.size());
		CPPUNIT_ASSERT_EQUAL(string("aaa"), splitted[0]);
		CPPUNIT_ASSERT_EQUAL(string("bbb,ccc"), splitted[1]);
	}

	void explodeNonLimit()
	{
		vector<string> splitted = StringUtil::explode("_|_", "aaa_|__|_bb_|_");
		CPPUNIT_ASSERT_EQUAL((vector<string>::size_type)4, splitted.size());
		CPPUNIT_ASSERT_EQUAL(string("aaa"), splitted[0]);
		CPPUNIT_ASSERT_EQUAL(string(""), splitted[1]);
		CPPUNIT_ASSERT_EQUAL(string("bb"), splitted[2]);
		CPPUNIT_ASSERT_EQUAL(string(""), splitted[3]);
	}

	void explodeWithEscape()
	{
		vector<string> splitted = StringUtil::explode(",", "a,b__,c,d", string::npos, "__");
		CPPUNIT_ASSERT_EQUAL(3, (int)splitted.size());
		CPPUNIT_ASSERT_EQUAL(string("a"), splitted[0]);
		CPPUNIT_ASSERT_EQUAL(string("b__,c"), splitted[1]);
		CPPUNIT_ASSERT_EQUAL(string("d"), splitted[2]);
	}

	void explodeDelimiterNotFound()
	{
		vector<string> splitted = StringUtil::explode("|", "aaa,bbb,ccc");
		CPPUNIT_ASSERT_EQUAL((string::size_type)1, splitted.size());
		CPPUNIT_ASSERT_EQUAL(string("aaa,bbb,ccc"), splitted[0]);
	}

	void testReplace()
	{
		string actual = StringUtil::replace("abc", "a", "A");
		string expected = "Abc";
		CPPUNIT_ASSERT_EQUAL(expected, actual);

		actual = StringUtil::replace("abc", "a", "a");
		expected = "abc";
		CPPUNIT_ASSERT_EQUAL(expected, actual);
	}

	void testParseInt()
	{
		CPPUNIT_ASSERT_EQUAL(1, StringUtil::parseInt<int>("1"));
		CPPUNIT_ASSERT_EQUAL(10, StringUtil::parseInt<int>("A", 16));
		try {
			StringUtil::parseInt<int>("A");
			CPPUNIT_FAIL("期待した例外がスローされない");
		} catch (StringUtil::IntegerParseException& e) {
			// 成功
		}
		try {
			StringUtil::parseInt<int>("W", 16);
			CPPUNIT_FAIL("期待した例外がスローされない");
		} catch (StringUtil::IntegerParseException& e) {
			// 成功
		}
	}

	void testParseFloat()
	{
		CPPUNIT_ASSERT_EQUAL(1.0, StringUtil::parseFloat<double>("1.0"));
		CPPUNIT_ASSERT_EQUAL(-1.0f, StringUtil::parseFloat<float>("-1.0"));
		try {
			StringUtil::parseFloat<double>("A");
			CPPUNIT_FAIL("期待した例外がスローされない");
		} catch (StringUtil::FloatParseException& e) {
			// 成功
		}
	}

	void testToString()
	{
		CPPUNIT_ASSERT_EQUAL(string("10"), StringUtil::toString(10));
		CPPUNIT_ASSERT_EQUAL(string("1A"), StringUtil::toString(0x1A, 16));
	}

	void testToStringWithFormat()
	{
		CPPUNIT_ASSERT_EQUAL(string("0.500"), StringUtil::toString(0.5, "%.3f"));
	}

	void testToLower()
	{
		std::string actual = StringUtil::toLower("FooBar");
		CPPUNIT_ASSERT_EQUAL(string("foobar"), actual);
	}

	void testRepeat()
	{
		string fixture = "ab";
		string actual = StringUtil::repeat(fixture, 4);
		CPPUNIT_ASSERT_EQUAL(string("abababab"), actual);
	}

	void trim()
	{
		CPPUNIT_ASSERT_EQUAL(string("hoge"), StringUtil::trim("hoge "));
		CPPUNIT_ASSERT_EQUAL(string("hoge"), StringUtil::trim("\nhoge\r"));
	}

	CPPUNIT_TEST_SUITE(StringUtilTest);
	CPPUNIT_TEST(explode);
	CPPUNIT_TEST(explodeNonLimit);
	CPPUNIT_TEST(explodeWithEscape);
	CPPUNIT_TEST(explodeDelimiterNotFound);
	CPPUNIT_TEST(testReplace);
	CPPUNIT_TEST(testParseInt);
	CPPUNIT_TEST(testParseFloat);
	CPPUNIT_TEST(testToString);
	CPPUNIT_TEST(testRepeat);
	CPPUNIT_TEST(testToLower);
	CPPUNIT_TEST(testToStringWithFormat);
	CPPUNIT_TEST(trim);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(StringUtilTest);

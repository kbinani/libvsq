#include "Util.hpp"
#include "../include/libvsq/StringUtil.hpp"

using namespace std;
using namespace vsq;

TEST(StringUtilTest, testexplode)
{
	vector<string> splitted = StringUtil::explode(",", "aaa,bbb,ccc", 2);
	EXPECT_EQ((vector<string>::size_type)2, splitted.size());
	EXPECT_EQ(string("aaa"), splitted[0]);
	EXPECT_EQ(string("bbb,ccc"), splitted[1]);
}

TEST(StringUtilTest, testexplodeNonLimit)
{
	vector<string> splitted = StringUtil::explode("_|_", "aaa_|__|_bb_|_");
	EXPECT_EQ((vector<string>::size_type)4, splitted.size());
	EXPECT_EQ(string("aaa"), splitted[0]);
	EXPECT_EQ(string(""), splitted[1]);
	EXPECT_EQ(string("bb"), splitted[2]);
	EXPECT_EQ(string(""), splitted[3]);
}

TEST(StringUtilTest, testexplodeWithEscape)
{
	vector<string> splitted = StringUtil::explode(",", "a,b__,c,d", string::npos, "__");
	EXPECT_EQ(3, (int)splitted.size());
	EXPECT_EQ(string("a"), splitted[0]);
	EXPECT_EQ(string("b__,c"), splitted[1]);
	EXPECT_EQ(string("d"), splitted[2]);
}

TEST(StringUtilTest, testexplodeDelimiterNotFound)
{
	vector<string> splitted = StringUtil::explode("|", "aaa,bbb,ccc");
	EXPECT_EQ((string::size_type)1, splitted.size());
	EXPECT_EQ(string("aaa,bbb,ccc"), splitted[0]);
}

TEST(StringUtilTest, testtestReplace)
{
	string actual = StringUtil::replace("abc", "a", "A");
	string expected = "Abc";
	EXPECT_EQ(expected, actual);

	actual = StringUtil::replace("abc", "a", "a");
	expected = "abc";
	EXPECT_EQ(expected, actual);
}

TEST(StringUtilTest, testtestParseInt)
{
	EXPECT_EQ(1, StringUtil::parseInt<int>("1"));
	EXPECT_EQ(10, StringUtil::parseInt<int>("A", 16));
	try {
		StringUtil::parseInt<int>("A");
		GTEST_FAIL(); // 期待した例外がスローされない.
	} catch (StringUtil::IntegerParseException& e) {
		// 成功
	}
	try {
		StringUtil::parseInt<int>("W", 16);
		GTEST_FAIL(); // 期待した例外がスローされない.
	} catch (StringUtil::IntegerParseException& e) {
		// 成功
	}
}

TEST(StringUtilTest, testtestParseFloat)
{
	EXPECT_EQ(1.0, StringUtil::parseFloat<double>("1.0"));
	EXPECT_EQ(-1.0f, StringUtil::parseFloat<float>("-1.0"));
	try {
		StringUtil::parseFloat<double>("A");
		GTEST_FAIL(); // 期待した例外がスローされない.
	} catch (StringUtil::FloatParseException& e) {
		// 成功
	}
}

TEST(StringUtilTest, testtestToString)
{
	EXPECT_EQ(string("10"), StringUtil::toString(10));
	EXPECT_EQ(string("1A"), StringUtil::toString(0x1A, 16));
}

TEST(StringUtilTest, testtestToStringWithFormat)
{
	EXPECT_EQ(string("0.500"), StringUtil::toString(0.5, "%.3f"));
}

TEST(StringUtilTest, testtestToLower)
{
	std::string actual = StringUtil::toLower("FooBar");
	EXPECT_EQ(string("foobar"), actual);
}

TEST(StringUtilTest, testtestRepeat)
{
	string fixture = "ab";
	string actual = StringUtil::repeat(fixture, 4);
	EXPECT_EQ(string("abababab"), actual);
}

TEST(StringUtilTest, testtrim)
{
	EXPECT_EQ(string("hoge"), StringUtil::trim("hoge "));
	EXPECT_EQ(string("hoge"), StringUtil::trim("\nhoge\r"));
}

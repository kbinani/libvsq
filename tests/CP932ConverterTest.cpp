#include "Util.hpp"
#include "../include/libvsq/CP932Converter.hpp"

using namespace std;
using namespace vsq;

TEST(CP932ConverterTest, testConvertFromUTF8)
{
	ostringstream fixture;
	// "aあ\t\n"
	fixture << (char)0x61 << (char)0xE3 << (char)0x81 << (char)0x82 << (char)0x09 << (char)0x0A;
	string actual = CP932Converter::convertFromUTF8(fixture.str());
	ostringstream expected;
	expected << (char)0x61 << (char)0x82 << (char)0xA0 << "\t\n";
	EXPECT_EQ(expected.str(), actual);
}

TEST(CP932ConverterTest, testConvertToUTF8)
{
	{
		ostringstream fixture;
		fixture << (char)0x61 << (char)0x82 << (char)0xA0 << "\t\n";
		string actual = CP932Converter::convertToUTF8(fixture.str());
		ostringstream expected;
		expected << (char)0x61 << (char)0xE3 << (char)0x81 << (char)0x82 << (char)0x09 << (char)0x0A;
		EXPECT_EQ(expected.str(), actual);
	}

	{
		ostringstream fixture;
		fixture << (char)0x5b << (char)0x4d << (char)0x69 << (char)0x78 << (char)0x65 << (char)0x72 << (char)0x5d;
		string actual = CP932Converter::convertToUTF8(fixture.str());
		EXPECT_EQ(fixture.str(), actual);
	}
}

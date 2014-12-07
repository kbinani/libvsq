#include "Util.hpp"
#include "../include/libvsq/BitConverter.hpp"

using namespace vsq;
using namespace std;

TEST(BitConverterTest, testGetBytesUInt16BE)
{
	vector<char> bytes = BitConverter::getBytesUInt16BE(0x1234);
	EXPECT_EQ((size_t)2, bytes.size());
	EXPECT_EQ((char)0x12, bytes[0]);
	EXPECT_EQ((char)0x34, bytes[1]);
}

TEST(BitConverterTest, testGetBytesUInt32BE)
{
	vector<char> bytes = BitConverter::getBytesUInt32BE(0x12345678);
	EXPECT_EQ((size_t)4, bytes.size());
	EXPECT_EQ((char)0x12, bytes[0]);
	EXPECT_EQ((char)0x34, bytes[1]);
	EXPECT_EQ((char)0x56, bytes[2]);
	EXPECT_EQ((char)0x78, bytes[3]);
}

TEST(BitConverterTest, testMakeUInt32LE)
{
	char bytes[4] = { 0x12, 0x34, 0x56, 0x78 };
	EXPECT_EQ((uint32_t)0x78563412, BitConverter::makeUInt32LE(bytes));
}

TEST(BitConverterTest, testMakeUInt32BE)
{
	char bytes[4] = { 0x12, 0x34, 0x56, 0x78 };
	EXPECT_EQ((uint32_t)0x12345678, BitConverter::makeUInt32BE(bytes));
}

TEST(BitConverterTest, testMakeInt16LE)
{
	{
		char bytes[2] = { 0x12, 0x34 };
		EXPECT_EQ((int16_t)13330, BitConverter::makeInt16LE(bytes));
	}
	{
		char bytes[2] = { 0x34, (char)0xF1 };
		EXPECT_EQ((int16_t) - 3788, BitConverter::makeInt16LE(bytes));
	}
}

TEST(BitConverterTest, testMakeUInt16BE)
{
	char bytes[2] = { 0x12, 0x34 };
	EXPECT_EQ((uint16_t)0x1234, BitConverter::makeUInt16BE(bytes));
}

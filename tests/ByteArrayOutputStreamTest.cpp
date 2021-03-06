﻿#include "Util.hpp"
#include "../include/libvsq/ByteArrayOutputStream.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include <sstream>

using namespace std;
using namespace vsq;

class ByteArrayOutputStreamStub : public ByteArrayOutputStream
{
public:
	static int getUnitBufferLength()
	{
		return UNIT_BUFFER_LENGTH;
	}
};

TEST(ByteArrayOutputStreamTest, test)
{
	ByteArrayOutputStream stream;
	EXPECT_EQ((int64_t)0, stream.getPointer());
	stream.write(0);
	stream.write(1);
	EXPECT_EQ((int64_t)2, stream.getPointer());
	char bufferA[] = { 0, 0, 127, 0, 0 };
	stream.write(bufferA, 2, 1);
	char bufferB[] = { (char)128, (char)255 };
	stream.write(bufferB, 0, 2);
	EXPECT_EQ((int64_t)5, stream.getPointer());
	string actual = stream.toString();
	ostringstream expected;
	expected << (char)0 << (char)1 << (char)127 << (char)128 << (char)255;
	EXPECT_EQ(expected.str(), actual);

	stream.seek(0);
	stream.write(64);
	EXPECT_EQ((int64_t)1, stream.getPointer());
	expected.str("");
	expected.setstate(ostringstream::goodbit);
	expected << (char)64 << (char)1 << (char)127 << (char)128 << (char)255;
	EXPECT_EQ(expected.str(), stream.toString());

	stream.close();
}

TEST(ByteArrayOutputStreamTest, testSeek)
{
	ByteArrayOutputStream stream;

	// 負の値を指定した場合, 0になる
	stream.seek(-100);
	EXPECT_EQ((int64_t)0, stream.getPointer());

	// 初期に確保されるバッファーより後ろにseekしても, 正しくseekできる
	stream.seek(ByteArrayOutputStreamStub::getUnitBufferLength() + 10);
	stream.write((int)'a');

	string actualString = stream.toString();
	vector<char> actual;
	actual.insert(actual.begin(), actualString.begin(), actualString.end());

	vector<char> expected;
	expected.insert(expected.begin(), ByteArrayOutputStreamStub::getUnitBufferLength() + 10, '\0');
	expected.insert(expected.end(), 'a');
	EXPECT_TRUE(expected == actual);
}

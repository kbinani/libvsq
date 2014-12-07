#include "../include/libvsq/FileOutputStream.hpp"
#include "Util.hpp"
#include <stdio.h>

using namespace std;
using namespace vsq;

TEST(FileOutputStreamTest, test)
{
	FileOutputStream stream("hoge.bin");
	EXPECT_EQ((int64_t)0, stream.getPointer());
	stream.write(0x10);
	EXPECT_EQ((int64_t)1, stream.getPointer());
	char buffer[3];
	buffer[0] = 0x20;
	buffer[1] = 0x30;
	buffer[2] = 0x40;
	stream.write(buffer, 1, 1);
	EXPECT_EQ((int64_t)2, stream.getPointer());

	stream.seek(0);
	EXPECT_EQ((int64_t)0, stream.getPointer());
	stream.write(buffer, 0, 3);
	EXPECT_EQ((int64_t)3, stream.getPointer());
	stream.close();

	FILE* file = fopen("hoge.bin", "rb");
	EXPECT_EQ(0x20, fgetc(file));
	EXPECT_EQ(0x30, fgetc(file));
	EXPECT_EQ(0x40, fgetc(file));
	EXPECT_EQ(EOF, fgetc(file));
	fclose(file);
}

TEST(FileOutputStreamTest, testWithException)
{
	EXPECT_THROW(FileOutputStream stream(""), FileOutputStream::IOException);
}

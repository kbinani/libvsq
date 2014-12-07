#include "Util.hpp"
#include "../include/libvsq/FileInputStream.hpp"

using namespace std;
using namespace vsq;

TEST(FileInputStreamTest, test)
{
	FileInputStream stream("FileInputStreamTest/fixture/data.bin");
	EXPECT_EQ((int64_t)0, stream.getPointer());
	EXPECT_EQ(0x00, stream.read());
	EXPECT_EQ((int64_t)1, stream.getPointer());
	stream.seek(0x05);
	EXPECT_EQ(0x05, stream.read());
	EXPECT_EQ((int64_t)0x06, stream.getPointer());

	stream.seek(0x10);
	char buffer[10] = { 0 };
	EXPECT_EQ(5, stream.read(buffer, 5, 5));
	EXPECT_EQ((char)0, buffer[0]);
	EXPECT_EQ((char)0, buffer[1]);
	EXPECT_EQ((char)0, buffer[2]);
	EXPECT_EQ((char)0, buffer[3]);
	EXPECT_EQ((char)0, buffer[4]);
	EXPECT_EQ((char)0x10, buffer[5]);
	EXPECT_EQ((char)0x11, buffer[6]);
	EXPECT_EQ((char)0x12, buffer[7]);
	EXPECT_EQ((char)0x13, buffer[8]);
	EXPECT_EQ((char)0x14, buffer[9]);

	stream.seek(0x2F);
	EXPECT_EQ(1, stream.read(buffer, 0, 2));
	EXPECT_EQ((char)0x2F, buffer[0]);

	stream.seek(0x2F);
	EXPECT_EQ(0x2F, stream.read());
	EXPECT_TRUE(stream.read() < 0);

	stream.close();
}

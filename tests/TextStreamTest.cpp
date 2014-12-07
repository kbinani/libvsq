#include "Util.hpp"
#include "../include/libvsq/TextStream.hpp"

using namespace std;
using namespace vsq;

TEST(TextStreamTest, testConstruct)
{
	TextStream stream;
	EXPECT_TRUE(false == stream.ready());
	EXPECT_EQ(string(""), stream.toString());
	EXPECT_TRUE(0 > stream.getPointer());
}

TEST(TextStreamTest, testReadLine)
{
	TextStream stream;
	stream.write("hel\nlo");
	stream.setPointer(-1);
	EXPECT_TRUE(stream.ready());
	EXPECT_EQ(string("hel"), stream.readLine());
	EXPECT_TRUE(stream.ready());
	string actual = stream.readLine();
	EXPECT_EQ(string("lo"), actual);
	EXPECT_TRUE(false == stream.ready());
}

TEST(TextStreamTest, testWrite)
{
	TextStream stream;
	stream.write("foo");
	EXPECT_EQ(2, stream.getPointer());
	stream.setPointer(-1);
	EXPECT_EQ(string("foo"), stream.readLine());
}

TEST(TextStreamTest, testWriteLine)
{
	TextStream stream;
	stream.writeLine("foo");
	EXPECT_EQ(3, stream.getPointer());
	stream.setPointer(-1);
	EXPECT_EQ(string("foo"), stream.readLine());
}

TEST(TextStreamTest, testClose)
{
	TextStream stream;
	stream.writeLine("foo");
	// エラーが起きなければ良しとする
	stream.close();
}

TEST(TextStreamTest, testToString)
{
	TextStream stream;
	stream.writeLine("foo");
	stream.writeLine("bar");
	EXPECT_EQ(string("foo\nbar\n"), stream.toString());
}

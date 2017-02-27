#include "Util.hpp"
#include "../include/libvsq/StreamWriter.hpp"
#include "../include/libvsq/FileOutputStream.hpp"

using namespace std;
using namespace vsq;

TEST(StreamWriterTest, test)
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

	EXPECT_EQ(expected.str(), actual.str());
	
	remove("foo.txt");
}

TEST(StreamWriterTest, testWithException)
{
	EXPECT_THROW(StreamWriter(""), TextOutputStream::IOException);
}

TEST(StreamWriterTest, testConstructByStream)
{
	StreamWriter writer(new FileOutputStream("foo.txt"));
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

	EXPECT_EQ(expected.str(), actual.str());
	
	remove("foo.txt");
}

#include "../FileOutputStream.hpp"
#include "Util.hpp"
#include <stdio.h>

using namespace std;
using namespace vsq;

class FileOutputStreamTest : public CppUnit::TestCase
{
public:
	void test()
	{
		FileOutputStream stream("hoge.bin");
		CPPUNIT_ASSERT_EQUAL((int64_t)0, stream.getPointer());
		stream.write(0x10);
		CPPUNIT_ASSERT_EQUAL((int64_t)1, stream.getPointer());
		char buffer[3];
		buffer[0] = 0x20;
		buffer[1] = 0x30;
		buffer[2] = 0x40;
		stream.write(buffer, 1, 1);
		CPPUNIT_ASSERT_EQUAL((int64_t)2, stream.getPointer());

		stream.seek(0);
		CPPUNIT_ASSERT_EQUAL((int64_t)0, stream.getPointer());
		stream.write(buffer, 0, 3);
		CPPUNIT_ASSERT_EQUAL((int64_t)3, stream.getPointer());
		stream.close();

		FILE* file = fopen("hoge.bin", "rb");
		CPPUNIT_ASSERT_EQUAL(0x20, fgetc(file));
		CPPUNIT_ASSERT_EQUAL(0x30, fgetc(file));
		CPPUNIT_ASSERT_EQUAL(0x40, fgetc(file));
		CPPUNIT_ASSERT_EQUAL(EOF, fgetc(file));
		fclose(file);
	}

	void testWithException()
	{
		CPPUNIT_ASSERT_THROW(FileOutputStream stream(""), FileOutputStream::IOException);
	}

	CPPUNIT_TEST_SUITE(FileOutputStreamTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testWithException);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(FileOutputStreamTest);

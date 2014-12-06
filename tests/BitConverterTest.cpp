#include "Util.hpp"
#include "../include/libvsq/BitConverter.hpp"

using namespace vsq;
using namespace std;

class BitConverterTest : public CppUnit::TestCase
{
public:
	void testGetBytesUInt16BE()
	{
		vector<char> bytes = BitConverter::getBytesUInt16BE(0x1234);
		CPPUNIT_ASSERT_EQUAL((size_t)2, bytes.size());
		CPPUNIT_ASSERT_EQUAL((char)0x12, bytes[0]);
		CPPUNIT_ASSERT_EQUAL((char)0x34, bytes[1]);
	}

	void testGetBytesUInt32BE()
	{
		vector<char> bytes = BitConverter::getBytesUInt32BE(0x12345678);
		CPPUNIT_ASSERT_EQUAL((size_t)4, bytes.size());
		CPPUNIT_ASSERT_EQUAL((char)0x12, bytes[0]);
		CPPUNIT_ASSERT_EQUAL((char)0x34, bytes[1]);
		CPPUNIT_ASSERT_EQUAL((char)0x56, bytes[2]);
		CPPUNIT_ASSERT_EQUAL((char)0x78, bytes[3]);
	}

	void testMakeUInt32LE()
	{
		char bytes[4] = { 0x12, 0x34, 0x56, 0x78 };
		CPPUNIT_ASSERT_EQUAL((uint32_t)0x78563412, BitConverter::makeUInt32LE(bytes));
	}

	void testMakeUInt32BE()
	{
		char bytes[4] = { 0x12, 0x34, 0x56, 0x78 };
		CPPUNIT_ASSERT_EQUAL((uint32_t)0x12345678, BitConverter::makeUInt32BE(bytes));
	}

	void testMakeInt16LE()
	{
		{
			char bytes[2] = { 0x12, 0x34 };
			CPPUNIT_ASSERT_EQUAL((int16_t)13330, BitConverter::makeInt16LE(bytes));
		}
		{
			char bytes[2] = { 0x34, (char)0xF1 };
			CPPUNIT_ASSERT_EQUAL((int16_t) - 3788, BitConverter::makeInt16LE(bytes));
		}
	}

	void testMakeUInt16BE()
	{
		char bytes[2] = { 0x12, 0x34 };
		CPPUNIT_ASSERT_EQUAL((uint16_t)0x1234, BitConverter::makeUInt16BE(bytes));
	}

	CPPUNIT_TEST_SUITE(BitConverterTest);
	CPPUNIT_TEST(testGetBytesUInt16BE);
	CPPUNIT_TEST(testGetBytesUInt32BE);
	CPPUNIT_TEST(testMakeUInt32LE);
	CPPUNIT_TEST(testMakeUInt32BE);
	CPPUNIT_TEST(testMakeInt16LE);
	CPPUNIT_TEST(testMakeUInt16BE);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(BitConverterTest);
